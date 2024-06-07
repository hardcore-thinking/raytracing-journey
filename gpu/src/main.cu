#include <iostream>
#include <cstdio>
#include <cstdlib>

#include "cuda_runtime.h"
#include "device_launch_parameters.h"

// Single-precision a * x + y
__global__ void SAXPY(int n, float a, float* x, float* y) {
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	
	if (i < n) {
		y[i] = a * x[i] + y[i];
	}
}

int main() {
	int N = 1 << 20;

	float* x = (float*) malloc(N * sizeof(float));
	if (x == nullptr) {
		std::cerr << "[ERROR] Unable to allocate memory for the x vector on the host memory." << std::endl;
		goto exit;
	}

	float* y = (float*) malloc(N * sizeof(float));
	if (y == nullptr) {
		std::cerr << "[ERROR] Unable to allocate memory for the y vector on the host memory." << std::endl;
		goto exit;
	}
	
	float* d_x = nullptr;
	float* d_y = nullptr;

	if (cudaMalloc(&d_x, N * sizeof(float)) != cudaSuccess) {
		std::cerr << "[ERROR] Unable to allocate memory for the d_y vector on the device memory." << std::endl;
		goto exit;
	}

	if (cudaMalloc(&d_y, N * sizeof(float)) != cudaSuccess) {
		std::cerr << "[ERROR] Unable to allocate memory for the d_y vector on the device memory." << std::endl;
		goto exit;
	}

	for (int i = 0; i < N; i++) {
		x[i] = 1.0f;
		y[i] = 2.0f;
	}

	if (cudaMemcpy(d_x, x, N * sizeof(float), cudaMemcpyHostToDevice) != cudaSuccess) {
		std::cerr << "[ERROR] Unable to allocate memory for the y vector." << std::endl;
		goto exit;
	}

	if (cudaMemcpy(d_y, y, N * sizeof(float), cudaMemcpyHostToDevice) != cudaSuccess) {
		std::cerr << "[ERROR] Unable to allocate memory for the y vector." << std::endl;
		goto exit;
	}

	SAXPY <<<(N + 255) / 256, 256>>> (N, 2.0f, d_x, d_y);

	if (cudaMemcpy(y, d_y, N * sizeof(float), cudaMemcpyDeviceToHost) != cudaSuccess) {
		std::cerr << "[ERROR] Unable to allocate memory for the y vector." << std::endl;
		goto exit;
	}

	float maxError = 0.0f;
	for (int i = 0; i < N; i++) {
		maxError = std::max(maxError, std::abs(y[i] - 4.0f));
	}

	std::cout << "Max error: " << maxError << std::endl;

	exit:
		if (x != nullptr) {
			free(x);
		}

		if (y != nullptr) {
			free(y);
		}

		if (d_x != nullptr) {
			cudaFree(d_x);
		}

		if (d_y != nullptr) {
			cudaFree(d_y);
		}

	return EXIT_SUCCESS;
}