#include "Camera.hpp"

#define RT_COMPUTE_USING_SINGLETHREADING 0
#define RT_COMPUTE_USING_MULTITHREADING  1
#define RT_COMPUTE_USING_THREADPOOL      2

#define RT_COMPUTING RT_COMPUTE_USING_THREADPOOL

#if defined RT_COMPUTING

#if RT_COMPUTING == RT_COMPUTE_USING_THREADPOOL
#include "ThreadPool.hpp"

#elif RT_COMPUTING == RT_COMPUTE_USING_MULTITHREADING
#include <mutex>
#endif

#endif

void Camera::Render(Hittable const& world) {
	Initialize();

	std::ofstream imageFile("./image.ppm", std::ios::beg);

	if (!imageFile.is_open()) {
		std::cerr << "Unable to open or create the output image file." << std::endl;
		return;
	}

	std::clog << "File opened or created successfully." << std::endl;

	imageFile << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

	size_t numberOfPixels = static_cast<size_t>(imageWidth * imageHeight);

	std::clog << "Creating image vector..." << std::endl;
	std::vector<Color> image(numberOfPixels);
	std::clog << "Image vector created" << std::endl;

	int lastLogLength = 0;

#if RT_COMPUTING == RT_COMPUTE_USING_SINGLETHREADING || not defined RT_COMPUTING
	const auto startComputing = std::chrono::steady_clock::now();
	std::stringstream colorStream;
	for (int i = 0; i < imageHeight; i++) {
		std::clog << "\rScanlines remaining: " << (imageHeight - i) << "     ";
		for (int j = 0; j < imageWidth; j++) {
			Color pixelColor(0, 0, 0);
			for (int sample = 0; sample < samplesPerPixel; sample++) {
				Ray r = GetRay(j, i);
				pixelColor += RayColor(r, maxDepth, world);
			}
			WriteColor(colorStream, pixelSamplesScale * pixelColor);

			colorStream >> pixelColor.e[0] >> pixelColor.e[1] >> pixelColor.e[2];

			image[i * imageWidth + j] = pixelColor;

			colorStream.clear();
		}
	}

#elif RT_COMPUTING == RT_COMPUTE_USING_MULTITHREADING
	// WITH MULTITHREADING
	size_t numberOfThreads = std::thread::hardware_concurrency();
	std::vector<std::thread> threads(numberOfThreads);

	size_t numberOfPixelsPerThread = numberOfPixels / numberOfThreads;
	size_t numberOfMissingPixels = numberOfPixels % numberOfThreads;

	std::clog << numberOfPixelsPerThread << " " << numberOfMissingPixels << std::endl;

	size_t pixelsOffset = 0;

	int scanlines = imageHeight;
	std::mutex scanlinesMutex;
	for (size_t i = 0; i < numberOfThreads; i++) {
		
		threads[i] = std::thread(
			[&](size_t currentThread, size_t offset, size_t excess) {
				std::stringstream colorStream;
				std::stringstream log;

				size_t const currentFirstPixel = currentThread * (numberOfPixelsPerThread + numberOfMissingPixels) + pixelsOffset;
				size_t const nextFirstPixel    = (currentThread + 1) * (numberOfPixelsPerThread + numberOfMissingPixels) + pixelsOffset;

				for (size_t j = currentFirstPixel; j < nextFirstPixel; j++) {
					int const currentLine   = j / imageWidth;
					int const currentColumn = j % imageWidth;

					if (j != 0 && currentColumn == 0) {
						{
							std::unique_lock<std::mutex> lock(scanlinesMutex);
							--scanlines;

							log << "\rThread " << i << "\tScanlines remaining: " << scanlines << "                  ";
							std::clog << log.str();
						}
					}

					Color pixelColor(0, 0, 0);
					for (int sample = 0; sample < samplesPerPixel; sample++) {
						Ray r = GetRay(currentColumn, currentLine);
						pixelColor += RayColor(r, maxDepth, world);
					}
					WriteColor(colorStream, pixelSamplesScale * pixelColor);

					colorStream >> pixelColor.e[0] >> pixelColor.e[1] >> pixelColor.e[2];

					image[j] = pixelColor;
					
					colorStream.clear();
				}
			}, i, pixelsOffset, numberOfMissingPixels);

		if (pixelsOffset == 0) pixelsOffset = numberOfPixelsPerThread - (numberOfPixelsPerThread - numberOfMissingPixels);
		numberOfMissingPixels = 0;
	}

	const auto startComputing = std::chrono::steady_clock::now();
	for (auto& thread : threads) {
		thread.join();
	}

#elif RT_COMPUTING == RT_COMPUTE_USING_THREADPOOL
	//const size_t numberOfThreads = std::thread::hardware_concurrency();
	const size_t numberOfThreads = 32;
	ThreadPool threadPool(numberOfThreads);

	constexpr size_t numberOfTasks = 500;

	std::vector<std::promise<void>> promises;
	promises.resize(numberOfTasks);

	size_t numberOfPixelsPerThread = numberOfPixels / numberOfTasks;
	size_t numberOfMissingPixels = numberOfPixels % numberOfTasks;

	std::clog << numberOfPixelsPerThread << " " << numberOfMissingPixels << std::endl;

	size_t pixelsOffset = 0;

	int scanlines = imageHeight;
	std::mutex scanlinesMutex;
	for (size_t i = 0; i < numberOfTasks; i++) {
		threadPool.enqueueTask(
			[&, i]() {
				std::stringstream colorStream;

				size_t const currentFirstPixel = i * (numberOfPixelsPerThread + numberOfMissingPixels) + pixelsOffset;
				size_t const nextFirstPixel = (i + 1) * (numberOfPixelsPerThread + numberOfMissingPixels) + pixelsOffset;

				for (size_t j = currentFirstPixel; j < nextFirstPixel; j++) {
					int const currentLine = j / imageWidth;
					int const currentColumn = j % imageWidth;

					if (j != 0 && currentColumn == 0) {
						{
							std::unique_lock<std::mutex> lock(scanlinesMutex);
							--scanlines;
							
							std::stringstream log;

							log << "\rScanlines remaining: " << scanlines;

							const int fill = log.str().size() - lastLogLength;
							lastLogLength = static_cast<int>(log.str().size());

							log << std::string(fill < 0 ? -fill : 0, ' ');

							std::clog << log.str();
						}
					}

					Color pixelColor(0, 0, 0);
					for (int sample = 0; sample < samplesPerPixel; sample++) {
						Ray r = GetRay(currentColumn, currentLine);
						pixelColor += RayColor(r, maxDepth, world);
					}
					WriteColor(colorStream, pixelSamplesScale * pixelColor);

					colorStream >> pixelColor.e[0] >> pixelColor.e[1] >> pixelColor.e[2];

					image[j] = pixelColor;

					colorStream.clear();
				}

				promises[i].set_value();
			}
		);

		if (pixelsOffset == 0) pixelsOffset = numberOfPixelsPerThread - (numberOfPixelsPerThread - numberOfMissingPixels);
		numberOfMissingPixels = 0;
	}

	const auto startComputing = std::chrono::steady_clock::now();
	for (std::promise<void>& promise : promises) {
		promise.get_future().wait();
	}

#endif

	std::clog << "\rScanlines remaining: 0" << std::endl;

	std::clog << " ¤ Writing results to image file..." << std::endl;
	for (auto const& pixel : image) {
		imageFile << pixel << std::endl;
	}

	const auto endComputing = std::chrono::steady_clock::now();
	const std::chrono::duration<double> elapsedComputing = endComputing - startComputing;

	imageFile.close();

	std::clog << " ¤ Done in " << elapsedComputing << "\a" << std::endl;
}

void Camera::Initialize() {
	// Calculate the image height, and ensure that it's at least 1.
	imageHeight = static_cast<int>(imageWidth / aspectRatio);
	imageHeight = (imageHeight < 1) ? 1 : imageHeight;

	pixelSamplesScale = 1.0 / samplesPerPixel;

	center = lookFrom;

	// Camera
	// auto focalLength = (lookFrom -  lookAt).Length();
	auto theta = DegreesToRadians(vfov);
	auto h = std::tan(theta / 2);
	auto viewportHeight = 2 * h * focusDist;
	auto viewportWidth = viewportHeight * (static_cast<double>(imageWidth) / imageHeight);
	
	w = UnitVector(lookFrom - lookAt);
	u = UnitVector(Cross(vUp, w));
	v = Cross(w, u);

	// Calculate the vectors across the horizontal and down the vertical viewport edges.
	auto viewportU = viewportWidth * u;
	auto viewportV = viewportHeight * (-v);

	// Calculate the horizontal and vertical delta vector from pixel to pixel
	pixelDeltaU = viewportU / imageWidth;
	pixelDeltaV = viewportV / imageHeight;

	// Calculate the location of the upper left pixel
	auto viewportUpperLeft = center - (focusDist * w) - viewportU / 2 - viewportV / 2;
	pixel00Loc = viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);

	// Calculate the camera defocus disk basis vectors
	auto defocusRadius = focusDist * std::tan(DegreesToRadians(defocusAngle / 2));
	defocusDiskU = u * defocusRadius;
	defocusDiskV = v * defocusRadius;
}

Ray Camera::GetRay(int i, int j) const {
	// Construct a camera ray originating from the defocus disk and directed at a randomly
	// sample point around the pixel location i, j

	auto offset = SampleSquare();
	auto pixelSample = pixel00Loc + ((i + offset.X()) * pixelDeltaU) + ((j + offset.Y()) * pixelDeltaV);

	auto rayOrigin = (defocusAngle <= 0) ? center : DefocusDiskSample();
	auto rayDirection = pixelSample - rayOrigin;

	return Ray(rayOrigin, rayDirection);
}

Vec3 Camera::SampleSquare() const {
	return Vec3(RandomDouble() - 0.5, RandomDouble() - 0.5, 0);
}

Point3 Camera::DefocusDiskSample() const {
	// Returns a random point in the camera defocus disk
	auto p = RandomInUnitDisk();
	return center + (p[0] * defocusDiskU) + (p[1] * defocusDiskV);
}

Color Camera::RayColor(Ray const& r, int depth, Hittable const& world) const {
	if (depth <= 0) {
		return Color(0, 0, 0);
	}

	HitRecord rec = {};
	if (world.Hit(r, Interval(0.001, infinity), rec)) {
		Ray scattered = {};
		Color attenuation = {};
		if (rec.mat->Scatter(r, rec, attenuation, scattered)) {
			return attenuation * RayColor(scattered, depth - 1, world);
		}
	}

	Vec3 unitDirection = UnitVector(r.Direction());
	auto a = 0.5 * (unitDirection.Y() + 1.0);
	return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 0.8);
}