#include <iostream>
#include <memory>

#include "HittableList.hpp"
#include "Vec3.hpp"
#include "Sphere.hpp"
#include "Color.hpp"
#include "Utils.hpp"
#include "Camera.hpp"
#include "Material.hpp"

/*
int main() {
	// World
	HittableList world;

	auto material_ground = std::make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
	auto material_center = std::make_shared<Lambertian>(Color(0.1, 0.2, 0.5));
	auto material_left   = std::make_shared<Dielectric>(1.50);
	auto material_bubble = std::make_shared<Dielectric>(1.00 / 1.50);
	auto material_right  = std::make_shared<Metal>(Color(0.8, 0.6, 0.2), 0.7);

	world.add(std::make_shared<Sphere>(Point3( 0.0, -100.5, -1.0), 100.0, material_ground));
	world.add(std::make_shared<Sphere>(Point3( 0.0,    0.0, -1.2), 0.5,   material_center));
	world.add(std::make_shared<Sphere>(Point3(-1.0,    0.0, -1.0), 0.5,   material_left));
	world.add(std::make_shared<Sphere>(Point3(-1.0,    0.0, -1.0), 0.495, material_bubble));
	world.add(std::make_shared<Sphere>(Point3(-1.0,    0.0, -1.0), 0.49,  material_bubble));
	world.add(std::make_shared<Sphere>(Point3(-1.0,    0.0, -1.0), 0.485, material_bubble));
	world.add(std::make_shared<Sphere>(Point3( 1.0,    0.0, -1.0), 0.5,   material_right));

	auto R = std::cos(pi / 4);

	//auto material_left  = std::make_shared<Lambertian>(Color(0, 0, 1));
	//auto material_right = std::make_shared<Lambertian>(Color(1, 0, 0));

	//world.add(std::make_shared<Sphere>(Point3(-1, 0, -1), R, material_left));
	//world.add(std::make_shared<Sphere>(Point3( 1, 0, -1), R, material_right));

	Camera cam;

	cam.aspectRatio     = 16.0 / 9.0;
	cam.imageWidth      = 400;
	cam.samplesPerPixel = 100;
	cam.maxDepth        = 50;

	cam.vfov     = 20;
	cam.lookFrom = Point3(-2, 2, 1);
	cam.lookAt   = Point3(0, 0, -1);
	cam.vUp      = Vec3(0, 1, 0);

	cam.defocusAngle = 10.0;
	cam.focusDist = 3.4;

	cam.Render(world);

	std::clog << '\a';

	return EXIT_SUCCESS;
}
*/

int main() {
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

	Camera cam;

	cam.aspectRatio     = 16.0 / 9.0;
	cam.imageWidth      = 1920;
	cam.samplesPerPixel = 32;
	cam.maxDepth        = 32;

	cam.vfov     = 20;
	cam.lookFrom = Point3(13, 2, 3);
	cam.lookAt   = Point3(0, 0, 0);
	cam.vUp      = Vec3(0, 1, 0);

	cam.defocusAngle = 0.6;
	cam.focusDist    = 10.0;

	cam.Render(world);

	std::clog << '\a';

	return EXIT_SUCCESS;
}