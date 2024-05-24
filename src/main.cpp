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
#include "Quad.hpp"

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

void Earth(Camera& cam) {
	auto earthTexture = std::make_shared<ImageTexture>("earthmap.jpg");
	auto earthSurface = std::make_shared<Lambertian>(earthTexture);
	auto globe = std::make_shared<Sphere>(Point3(0, 0, 0), 2, earthSurface);

	cam.Render(HittableList(globe));
}

void PerlinSpheres(Camera& cam) {
	HittableList world;

	auto pertext = std::make_shared<NoiseTexture>(2);
	world.Add(std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, std::make_shared<Lambertian>(pertext)));
	world.Add(std::make_shared<Sphere>(Point3(0, 2, 0), 2, std::make_shared<Lambertian>(pertext)));

	cam.Render(world);
}

void Quads(Camera& cam) {
	HittableList world;

	// Materials
	auto leftRed     = std::make_shared<Lambertian>(Color(1.0, 0.2, 0.2));
	auto backGreen   = std::make_shared<Lambertian>(Color(0.2, 1.0, 0.2));
	auto rightBlue   = std::make_shared<Lambertian>(Color(0.2, 0.2, 1.0));
	auto upperOrange = std::make_shared<Lambertian>(Color(1.0, 0.5, 0.0));
	auto lowerTeal   = std::make_shared<Lambertian>(Color(0.2, 0.8, 0.8));

	// Quads
	world.Add(std::make_shared<Quad>(Point3(-3, -2,  5), Vec3(0,  0, -4), Vec3(0,  4,  0), leftRed));
	world.Add(std::make_shared<Quad>(Point3(-2, -2,  0), Vec3(4,  0,  0), Vec3(0,  4,  0), backGreen));
	world.Add(std::make_shared<Quad>(Point3( 3, -2,  1), Vec3(0,  0,  4), Vec3(0,  4,  0), rightBlue));
	world.Add(std::make_shared<Quad>(Point3(-2,  3,  1), Vec3(4,  0,  0), Vec3(0,  0,  4), upperOrange));
	world.Add(std::make_shared<Quad>(Point3(-2, -3,  5), Vec3(4,  0,  0), Vec3(0,  0, -4), lowerTeal));

	cam.Render(world);
}

int main() {
	Camera cam;

	cam.aspectRatio = 16.0 / 9.0;
	cam.imageWidth = 800;
	cam.samplesPerPixel = 400;
	cam.maxDepth = 200;

	cam.vFOV = 80;
	cam.lookFrom = Point3(0, 0, 9);
	cam.lookAt = Point3(0, 0, 0);
	cam.vUp = Vec3(0, 1, 0);

	cam.defocusAngle = 0;
	
	//cam.defocusAngle = 0.6;
	//cam.focusDist = 10.0;

	int select = 5;

	switch (select) {
		case 1:
			BouncingSperes(cam);
			break;
	
		case 2:
			CheckeredSpheres(cam);
			break;

		case 3:
			Earth(cam);
			break;

		case 4:
			PerlinSpheres(cam);
			break;
			
		case 5:
			Quads(cam);
			break;

		default:
			break;
	}
}