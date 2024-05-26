#include <iostream>
#include <cstdlib>

#include "cuda_runtime.h"
#include "device_launch_parameters.h"

__global__ void HelloGPU() {
	unsigned int tx = threadIdx.x;
	unsigned int ty = threadIdx.y;
	unsigned int tz = threadIdx.z;

	unsigned int bix = blockIdx.x;
	unsigned int biy = blockIdx.y;
	unsigned int biz = blockIdx.z;

	unsigned int bdx = blockDim.x;
	unsigned int bdy = blockDim.y;
	unsigned int bdz = blockDim.z;

	printf("Currently running:\n"
		   " > threadIdx  = (x: %4u, y: %4u, z: %4u)\n"
		   " > blockIdx   = (x: %4u, y: %4u, z: %4u)\n"
		   " > blockDim   = (x: %4u, y: %4u, z: %4u)\n", tx, ty, tz, bix, biy, biz, bdx, bdy, bdz);
}

int main() {
	HelloGPU<<<1, 16>>>();

	cudaDeviceSynchronize();

	return EXIT_SUCCESS;
}