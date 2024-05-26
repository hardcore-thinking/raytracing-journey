#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <fstream>
#include <thread>
#include <thread>
#include <sstream>
#include <chrono>

#include "Hittable.hpp"
#include "Color.hpp"
#include "Vec3.hpp"
#include "Material.hpp"

class Camera {
	public:
		double aspectRatio  = 1.0; // Ratio of image width over height
		int imageWidth      = 100; // Rendered image width in pixel count
		int samplesPerPixel = 10;  // Count of random samples for each pixel
		int maxDepth        = 10;  // Maximum number of ray bounces into scene
		Color background    = {};  // Scene background color

		double vFOV     = 90;              // Vertical view angle (field of view)
		Point3 lookFrom = Point3(0, 0, 0); // Point camera is looking from
		Point3 lookAt   = Point3(0, 0, 0); // Point camera is looking at
		Vec3 vUp        = Vec3(0, 1, 0);   // Camera-relative "up" direction

		double defocusAngle = 0.0;  // Variation angle for rays through each pixel
		double focusDist    = 10.0; // Distance from camera looFrom point to plane of perfect focus

		void Render(Hittable const& world);

	private:
		int _imageHeight          = 0;   // Renderer image height
		double _pixelSamplesScale = 0.0; // Color scale factor for a sum of pixel samples
		Point3 _center            = {};  // Camera center
		Point3 _pixel00Loc        = {};  // Location of pixel 0, 0
		Vec3 _pixelDeltaU         = {};  // Offset to pixel to the right
		Vec3 _pixelDeltaV         = {};  // Offset to picel below
		Vec3 _u                   = {};  // Camera frame basis vector u
		Vec3 _v                   = {};  // Camera frame basis vector v
		Vec3 _w                   = {};  // Camera frame basis vector w
		Vec3 _defocusDiskU        = {};  // Defocus disk horizontal radius
		Vec3 _defocusDiskV        = {};  // Defocus disk vertical radius

		void Initialize();
		Ray GetRay(int i, int j) const;
		Vec3 SampleSquare() const;
		Point3 DefocusDiskSample() const;
		Color RayColor(Ray const& r, int depth, Hittable const& world) const;
};

#endif // CAMERA_HPP