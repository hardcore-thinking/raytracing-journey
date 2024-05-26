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
#include "ConstantMedium.hpp"

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

void SimpleLight(Camera& cam) {
	HittableList world;

	auto perText = std::make_shared<NoiseTexture>(4);
	world.Add(std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, std::make_shared<Lambertian>(perText)));
	world.Add(std::make_shared<Sphere>(Point3(0, 2, 0), 2, std::make_shared<Lambertian>(perText)));

	auto diffLight = std::make_shared<DiffuseLight>(Color(4, 4, 4));
	world.Add(std::make_shared<Sphere>(Point3(0, 7, 0), 2, diffLight));
	world.Add(std::make_shared<Quad>(Point3(3, 1, -2), Vec3(2, 0, 0), Vec3(0, 2, 0), diffLight));

	cam.Render(world);
}

void CornellBox(Camera& cam) {
	HittableList world;

	auto red   = std::make_shared<Lambertian>(Color(0.65, 0.05, 0.05));
	auto white = std::make_shared<Lambertian>(Color(0.73, 0.73, 0.73));
	auto green = std::make_shared<Lambertian>(Color(0.12, 0.45, 0.15));
	auto blue  = std::make_shared<Lambertian>(Color(0.09, 0.14, 0.19));
	auto light = std::make_shared<DiffuseLight>(Color(15, 15, 15));
	// auto diel = std::make_shared<Dielectric>(0.095);
	// auto perl  = std::make_shared<Lambertian>(std::make_shared<NoiseTexture>(1));

	world.Add(std::make_shared<Quad>(Point3(555,   0,    0), Vec3(   0, 555, 0), Vec3(  0,   0,  555), green));      // left
	world.Add(std::make_shared<Quad>(Point3(  0,   0,    0), Vec3(   0, 555, 0), Vec3(  0,   0,  555), red));        // right
	world.Add(std::make_shared<Quad>(Point3(343, 554,  332), Vec3(-130,   0, 0), Vec3(  0,   0, -105), light));      // light
	world.Add(std::make_shared<Quad>(Point3(  0,   0,    0), Vec3( 555,   0, 0), Vec3(  0,   0,  555), white));      // bottom
	world.Add(std::make_shared<Quad>(Point3(555, 555,  555), Vec3(-555,   0, 0), Vec3(  0,   0, -555), white));      // top
	world.Add(std::make_shared<Quad>(Point3(  0,   0,  555), Vec3( 555,   0, 0), Vec3(  0, 555,    0), white));      // back
	world.Add(std::make_shared<Quad>(Point3(  0,   0,    0), Vec3( 555,   0, 0), Vec3(  0, 555,    0), blue, true)); // front

	std::shared_ptr<Hittable> box1 = Box(Point3(0, 0, 0), Point3(165, 330, 165), white);
	box1 = std::make_shared<RotateY>(box1, 15);
	box1 = std::make_shared<Translate>(box1, Vec3(265, 0, 295));
	world.Add(box1);

	std::shared_ptr<Hittable> box2 = Box(Point3(0, 0, 0), Point3(165, 165, 165), white);
	box2 = std::make_shared<RotateY>(box2, -18);
	box2 = std::make_shared<Translate>(box2, Vec3(130, 0, 65));
	world.Add(box2);

	cam.Render(world);
}

void CornellSmoke(Camera& cam) {
	HittableList world;

	auto red = std::make_shared<Lambertian>(Color(0.65, 0.05, 0.05));
	auto white = std::make_shared<Lambertian>(Color(0.73, 0.73, 0.73));
	auto green = std::make_shared<Lambertian>(Color(0.12, 0.45, 0.15));
	auto light = std::make_shared<DiffuseLight>(Color(7, 7, 7));

	world.Add(std::make_shared<Quad>(Point3(555,   0,   0), Vec3(  0, 555, 0), Vec3(0,   0, 555), green));      // left
	world.Add(std::make_shared<Quad>(Point3(  0,   0,   0), Vec3(  0, 555, 0), Vec3(0,   0, 555), red));        // right
	world.Add(std::make_shared<Quad>(Point3(113, 554, 127), Vec3(330,   0, 0), Vec3(0,   0, 305), light));      // light
	world.Add(std::make_shared<Quad>(Point3(  0, 555,   0), Vec3(555,   0, 0), Vec3(0,   0, 555), white));      // bottom
	world.Add(std::make_shared<Quad>(Point3(  0,   0,   0), Vec3(555,   0, 0), Vec3(0,   0, 555), white));      // top
	world.Add(std::make_shared<Quad>(Point3(  0,   0, 555), Vec3(555,   0, 0), Vec3(0, 555,   0), white));      // back
	world.Add(std::make_shared<Quad>(Point3(  0,   0,   0), Vec3(555,   0, 0), Vec3(0, 555,   0), white, true)); // front

	std::shared_ptr<Hittable> box1 = Box(Point3(0, 0, 0), Point3(165, 330, 165), white);
	box1 = std::make_shared<RotateY>(box1, 15);
	box1 = std::make_shared<Translate>(box1, Vec3(265, 0, 295));

	std::shared_ptr<Hittable> box2 = Box(Point3(0, 0, 0), Point3(165, 165, 165), white);
	box2 = std::make_shared<RotateY>(box2, -18);
	box2 = std::make_shared<Translate>(box2, Vec3(130, 0, 65));

	std::shared_ptr<Hittable> box3 = Box(Point3(0, 0, 0), Point3(555, 555, 555), white);

	world.Add(std::make_shared<ConstantMedium>(box1, 0.01, Color(0, 0, 0)));
	world.Add(std::make_shared<ConstantMedium>(box2, 0.01, Color(1, 1, 1)));

	cam.Render(world);
}

int main() {
	Camera cam;

	cam.aspectRatio = 1.0;
	cam.imageWidth = 400;
	cam.samplesPerPixel = 128;
	cam.maxDepth = 16;

	cam.vFOV = 40;
	cam.lookFrom = Point3(278, 278, -800);
	cam.lookAt = Point3(278, 278, 0);
	cam.vUp = Vec3(0, 1, 0);
	cam.background = Color(0, 0, 0);

	cam.defocusAngle = 0;

	int select = 8;

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

		case 6:
			SimpleLight(cam);
			break;

		case 7:
			CornellBox(cam);
			break;

		case 8:
			CornellSmoke(cam);
			break;

		default:
			break;
	}
}