#include <iostream>
#include <memory>

#include "HittableList.hpp"
#include "Vec3.hpp"
#include "Sphere.hpp"
#include "Color.hpp"
#include "Utils.hpp"
#include "Camera.hpp"
#include "Material.hpp"
#include "BVH.hpp"
#include "Texture.hpp"

void BouncingSperes(Camera& cam) {
	// World
	HittableList world;

	auto groundMaterial = std::make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
	world.Add(std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, groundMaterial));

	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			auto chooseMat = RandomDouble();
			Point3 center(a + 0.9 * RandomDouble(), 0.2, b + 0.9 * RandomDouble());

			if ((center - Point3(4, 0.2, 0)).Length() > 0.9) {
				std::shared_ptr<Material> sphereMaterial;

				if (chooseMat < 0.8) {
					// diffuse
					auto albedo = Color::Random() * Color::Random();
					sphereMaterial = std::make_shared<Lambertian>(albedo);
					auto center2 = center + Vec3(0, RandomDouble(0, 0.5), 0);
					world.Add(std::make_shared<Sphere>(center, center2, 0.2, sphereMaterial));
				}

				else if (chooseMat < 0.95) {
					// metal
					auto albedo = Color::Random(0.5, 1);
					auto fuzz = RandomDouble(0, 0.5);
					sphereMaterial = std::make_shared<Metal>(albedo, fuzz);
					world.Add(std::make_shared<Sphere>(center, 0.2, sphereMaterial));
				}

				else {
					// glass
					sphereMaterial = std::make_shared<Dielectric>(1.5);
					world.Add(std::make_shared<Sphere>(center, 0.2, sphereMaterial));
				}
			}
		}
	}

	auto material1 = std::make_shared<Dielectric>(1.5);
	world.Add(std::make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

	auto material2 = std::make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
	world.Add(std::make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

	auto material3 = std::make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
	world.Add(std::make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

	world = HittableList(std::make_shared<BVHNode>(world));

	cam.Render(world);

	std::clog << '\a';
}

void CheckeredSpheres(Camera& cam) {
	HittableList world;

	auto checker = std::make_shared<CheckerTexture>(0.32, Color(0.2, 0.3, 0.1), Color(0.9, 0.9, 0.9));

	world.Add(std::make_shared<Sphere>(Point3(0, -10, 0), 10, std::make_shared<Lambertian>(checker)));
	world.Add(std::make_shared<Sphere>(Point3(0,  10, 0), 10, std::make_shared<Lambertian>(checker)));
	
	cam.Render(world);
}

int main() {
	Camera cam;

	cam.aspectRatio = 16.0 / 9.0;
	cam.imageWidth = 4096;
	cam.samplesPerPixel = 256;
	cam.maxDepth = 256;

	cam.vFOV = 30;
	cam.lookFrom = Point3(13, 2, 3);
	cam.lookAt = Point3(0, 0, 0);
	cam.vUp = Vec3(0, 1, 0);

	cam.defocusAngle = 0;
	
	//cam.defocusAngle = 0.6;
	//cam.focusDist = 10.0;

	constexpr int select = 2;

	switch (select) {
		case 1:
			BouncingSperes(cam);
			break;
	
		case 2:
			CheckeredSpheres(cam);
			break;
	}
}