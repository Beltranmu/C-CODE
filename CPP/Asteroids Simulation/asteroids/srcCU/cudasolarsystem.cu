#pragma once

#include "cuda_runtime.h"
#include "device_launch_parameters.h"


#include <stdio.h>
#include "cudasolarsystem.cuh"


                                       
__device__ __constant__ double kMSun = 1988470000000000000000000000000.0; // 1.98847 * pow(10.0, 30);
__device__ __constant__ double kG = 0.0000000000667543; //6.67543 * pow(10.0, -11);
__device__ __constant__ double kAU = 149600000000.0; // 1.496 * pow(10.0, 11);         // m^3
const int kMaxNumberAsteroid = 200;
__device__ __constant__ int kMaxNumberAsteroidDev = kMaxNumberAsteroid;
const int kNumberThreadPerBlock = 1024;



__global__ void addKernel2(double* a) {
  int i = threadIdx.x;
  a[i] *= -1;;
}

                                    
// m1, mSun, ¿d?, ->F
__global__ void forceKernel(float* m, float* fx, float* fy, float* fz, 
                            double* x, double* y, double* z, const int size,
                            const int* asteroidID, const int* numberAsteroid){
  
  int i = threadIdx.x;
  int j = blockIdx.x;
  i = i + j * blockDim.x;
  if (size > i) {

    // Sun force
    double distance = x[i] * x[i] + y[i] * y[i] + z[i] * z[i];
    if (distance == 0) { distance = 0.01f; }
    float moduleF= 1.0f/-sqrt(distance);
    if (distance <= 100) { moduleF *= -1.0f; }
    float dirX = x[i] * moduleF;
    float dirY = y[i] * moduleF;
    float dirZ = z[i] * moduleF;

    float force = kG * ((kMSun * m[i]) / distance);

    fx[i] = force * dirX * 10000000.0f;
    fy[i] = force * dirY * 10000000.0f;
    fz[i] = force * dirZ * 10000000.0f;


    // other Asteroids
    int n = numberAsteroid[i];
    int offsetData = kMaxNumberAsteroidDev * i;
    for (int a = 0; a < n; ++a) {

      int asteroid = asteroidID[offsetData + a];
      float tmpX = x[asteroid] - x[i];
      float tmpY = y[asteroid] - y[i];
      float tmpZ = z[asteroid] - z[i];


      distance = tmpX * tmpX + tmpY * tmpY + tmpZ * tmpZ;
      if (distance == 0) { distance = 0.01f; }
      moduleF = 1/sqrt(distance);
      if (distance <= 100) { moduleF *= -1.0f; }

      dirX = tmpX * moduleF;
      dirY = tmpY * moduleF;
      dirZ = tmpZ * moduleF;

      force = kG * ((m[i] * m[asteroid]) / distance);


      fx[i] += force * dirX;
      fy[i] += force * dirY;
      fz[i] += force * dirZ;

    }
  }

}

// m1, F, ->A
__global__ void accelerationKernel(float* m, float *fx, float* fy, float* fz, 
                                   float *ax, float* ay, float* az, const int size){
  int i = threadIdx.x;
  int j = blockIdx.x;
  i = i + j * blockDim.x;
  float mInv = 1 / m[i];
  if (size > i) {
    ax[i] = fx[i] *mInv;
    ay[i] = fy[i] *mInv;
    az[i] = fz[i] *mInv;
  }
}

// V, A, t
__global__ void velocityKernel(float* vx, float* vy, float* vz, 
                               float* ax, float* ay, float* az,
                               float* t, const int size){
  int i = threadIdx.x;
  int j = blockIdx.x;
  i = i + j * blockDim.x;
  if (size > i) {
    float velX = vx[i] + ax[i] * t[0];
    float velY = vy[i] + ay[i] * t[0];
    float velZ = vz[i] + az[i] * t[0];

    vx[i] = velX;
    vy[i] = velY;
    vz[i] = velZ;
  }
}

// P, V, A, t
__global__ void positionKernel(double* x, double* y, double* z, float* vx, 
                               float* vy, float* vz, float* ax, float* ay, 
                               float* az, float* t, const int size){
  int i = threadIdx.x;
  int j = blockIdx.x;
  i = i + j * blockDim.x;
  if (size > i) {
    float time = t[0];

    float pX = x[i] + vx[i] * time + (ax[i] * time * time * 0.5f);
    float pY = y[i] + vy[i] * time + (ay[i] * time * time * 0.5f);
    float pZ = z[i] + vz[i] * time + (az[i] * time * time * 0.5f);

    x[i] = pX;
    y[i] = pY;
    z[i] = pZ;
  }

}


cudaError_t initAsteroidBeltCuda(AsteroidBeltDeviceData& asteroidDataGPU, int size) {


  std::chrono::steady_clock::time_point start, end;
  std::chrono::microseconds duration;
  start = std::chrono::high_resolution_clock::now();

  cudaError_t cudaStatus = cudaSetDevice(0);
  if (cudaSuccess != cudaStatus) {
    fprintf(stderr, "cudaSetDevice failed!  Do you have a CUDA-capable GPU installed?");
    goto Error;
  }


  //Allocation of the device memory
  cudaStatus = cudaMalloc((void**)&asteroidDataGPU.dev_t, sizeof(float));
  if (cudaSuccess != cudaStatus) {
    fprintf(stderr, "cudaMalloc failed!");
    goto Error;
  }

  cudaStatus = cudaMalloc((void**)&asteroidDataGPU.dev_m, size * sizeof(float));
  if (cudaStatus != cudaSuccess) {
    fprintf(stderr, "cudaMalloc failed!");
    goto Error;
  }

  cudaStatus = cudaMalloc((void**)&asteroidDataGPU.dev_x, size * sizeof(double));
  if (cudaStatus != cudaSuccess) {
    fprintf(stderr, "cudaMalloc failed!");
    goto Error;
  }

  cudaStatus = cudaMalloc((void**)&asteroidDataGPU.dev_y, size * sizeof(double));
  if (cudaStatus != cudaSuccess) {
    fprintf(stderr, "cudaMalloc failed!");
    goto Error;
  }

  cudaStatus = cudaMalloc((void**)&asteroidDataGPU.dev_z, size * sizeof(double));
  if (cudaStatus != cudaSuccess) {
    fprintf(stderr, "cudaMalloc failed!");
    goto Error;
  }

  cudaStatus = cudaMalloc((void**)&asteroidDataGPU.dev_vx, size * sizeof(float));
  if (cudaStatus != cudaSuccess) {
    fprintf(stderr, "cudaMalloc failed!");
    goto Error;
  }

  cudaStatus = cudaMalloc((void**)&asteroidDataGPU.dev_vy, size * sizeof(float));
  if (cudaStatus != cudaSuccess) {
    fprintf(stderr, "cudaMalloc failed!");
    goto Error;
  }

  cudaStatus = cudaMalloc((void**)&asteroidDataGPU.dev_vz, size * sizeof(float));
  if (cudaStatus != cudaSuccess) {
    fprintf(stderr, "cudaMalloc failed!");
    goto Error;
  }

  cudaStatus = cudaMalloc((void**)&asteroidDataGPU.dev_ax, size * sizeof(float));
  if (cudaStatus != cudaSuccess) {
    fprintf(stderr, "cudaMalloc failed!");
    goto Error;
  }

  cudaStatus = cudaMalloc((void**)&asteroidDataGPU.dev_ay, size * sizeof(float));
  if (cudaStatus != cudaSuccess) {
    fprintf(stderr, "cudaMalloc failed!");
    goto Error;
  }

  cudaStatus = cudaMalloc((void**)&asteroidDataGPU.dev_az, size * sizeof(float));
  if (cudaStatus != cudaSuccess) {
    fprintf(stderr, "cudaMalloc failed!");
    goto Error;
  }

  cudaStatus = cudaMalloc((void**)&asteroidDataGPU.dev_fx, size * sizeof(float));
  if (cudaStatus != cudaSuccess) {
    fprintf(stderr, "cudaMalloc failed!");
    goto Error;
  }

  cudaStatus = cudaMalloc((void**)&asteroidDataGPU.dev_fy, size * sizeof(float));
  if (cudaStatus != cudaSuccess) {
    fprintf(stderr, "cudaMalloc failed!");
    goto Error;
  }

  cudaStatus = cudaMalloc((void**)&asteroidDataGPU.dev_fz, size * sizeof(float));
  if (cudaStatus != cudaSuccess) {
    fprintf(stderr, "cudaMalloc failed!");
    goto Error;
  }

  cudaStatus = cudaMalloc((void**)&asteroidDataGPU.dev_outputData, size * kMaxNumberAsteroid * sizeof(int));
  if (cudaStatus != cudaSuccess) {
    fprintf(stderr, "cudaMalloc failed!");
    goto Error;
  }

  cudaStatus = cudaMalloc((void**)&asteroidDataGPU.dev_outputSize, size * sizeof(int));
  if (cudaStatus != cudaSuccess) {
    fprintf(stderr, "cudaMalloc failed!");
    goto Error;
  }



Error:
  end = std::chrono::high_resolution_clock::now();
  duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  printf("Allocation CudaMemory: %f microseconds. \n", duration.count() / 1000000.0f);


  return cudaStatus;

}



cudaError_t updateAsteroidBeltCuda(double* x, double* y, double* z,
                                   float* vx, float* vy, float* vz,
                                   float* m,  float* t, int size,
                                   int* outputData, int* outputSize,
                                   AsteroidBeltDeviceData* asteroidDataGPU, MeasureTime* chrono,
                                  int id)
{
  

  chrono->startUpdate[id] = std::chrono::high_resolution_clock::now();

  cudaError_t cudaStatus;
  int numberBlocks = (int)ceilf((float)size / kNumberThreadPerBlock);
  // Choose which GPU to run on, change this on a multi-GPU system.
  cudaStatus = cudaSetDevice(0);
  if (cudaSuccess != cudaStatus) {
    fprintf(stderr, "cudaSetDevice failed!  Do you have a CUDA-capable GPU installed?");
    goto Error;
  }

  
  // Copy input vectors from host memory to GPU buffers.
  cudaStatus = cudaMemcpy(asteroidDataGPU->dev_t, t, sizeof(float), cudaMemcpyHostToDevice);
  if (cudaSuccess != cudaStatus) {
    fprintf(stderr, "cudaMemcpy failed!");
    goto Error;
  }

  cudaStatus = cudaMemcpy(asteroidDataGPU->dev_m, m, size * sizeof(float), cudaMemcpyHostToDevice);
  if (cudaStatus != cudaSuccess) {
    fprintf(stderr, "cudaMemcpy failed!");
    goto Error;
  }

  cudaStatus = cudaMemcpy(asteroidDataGPU->dev_x, x, size * sizeof(double), cudaMemcpyHostToDevice);
  if (cudaStatus != cudaSuccess) {
    fprintf(stderr, "cudaMemcpy failed!");
    goto Error;
  }

  cudaStatus = cudaMemcpy(asteroidDataGPU->dev_y, y, size * sizeof(double), cudaMemcpyHostToDevice);
  if (cudaStatus != cudaSuccess) {
    fprintf(stderr, "cudaMemcpy failed!");
    goto Error;
  }

  cudaStatus = cudaMemcpy(asteroidDataGPU->dev_z, z, size * sizeof(double), cudaMemcpyHostToDevice);
  if (cudaStatus != cudaSuccess) {
    fprintf(stderr, "cudaMemcpy failed!");
    goto Error;
  }

  cudaStatus = cudaMemcpy(asteroidDataGPU->dev_vx, vx, size * sizeof(float), cudaMemcpyHostToDevice);
  if (cudaStatus != cudaSuccess) {
    fprintf(stderr, "cudaMemcpy failed!");
    goto Error;
  }

  cudaStatus = cudaMemcpy(asteroidDataGPU->dev_vy, vy, size * sizeof(float), cudaMemcpyHostToDevice);
  if (cudaStatus != cudaSuccess) {
    fprintf(stderr, "cudaMemcpy failed!");
    goto Error;
  }

  cudaStatus = cudaMemcpy(asteroidDataGPU->dev_vz, vz, size * sizeof(float), cudaMemcpyHostToDevice);
  if (cudaStatus != cudaSuccess) {
    fprintf(stderr, "cudaMemcpy failed!");
    goto Error;
  }

  cudaStatus = cudaMemcpy(asteroidDataGPU->dev_outputData, outputData, size * kMaxNumberAsteroid *  sizeof(int), cudaMemcpyHostToDevice);
  if (cudaStatus != cudaSuccess) {
    fprintf(stderr, "cudaMemcpy failed!");
    goto Error;
  }
  cudaStatus = cudaMemcpy(asteroidDataGPU->dev_outputSize, outputSize, size * sizeof(int), cudaMemcpyHostToDevice);
  if (cudaStatus != cudaSuccess) {
    fprintf(stderr, "cudaMemcpy failed!");
    goto Error;
  }

  chrono->startF[id] = std::chrono::high_resolution_clock::now();

  
  
  //Force Kernel
  {
    // Launch a kernel on the GPU with one thread for each element.
    forceKernel << <numberBlocks, kNumberThreadPerBlock >> > (asteroidDataGPU->dev_m, 
    asteroidDataGPU->dev_fx, asteroidDataGPU->dev_fy, asteroidDataGPU->dev_fz,
    asteroidDataGPU->dev_x, asteroidDataGPU->dev_y, asteroidDataGPU->dev_z, size,
    asteroidDataGPU->dev_outputData, asteroidDataGPU->dev_outputSize);


    // Check for any errors launching the kernel
    cudaStatus = cudaGetLastError();
    if (cudaSuccess != cudaStatus) {
      fprintf(stderr, "forceKernel launch failed: %s\n", cudaGetErrorString(cudaStatus));
      goto Error;
    }
    // cudaDeviceSynchronize waits for the kernel to finish, and returns
    // any errors encountered during the launch.
    cudaStatus = cudaDeviceSynchronize();
    if (cudaSuccess != cudaStatus) {
      fprintf(stderr, "cudaDeviceSynchronize returned error code %d after launching forceKernel!\n", cudaStatus);
      goto Error;
    }
    
  }

  chrono->startA[id] = std::chrono::high_resolution_clock::now();
  
  //Acceleration Kernel
  {
    // Launch a kernel on the GPU with one thread for each element.
    accelerationKernel << <numberBlocks, kNumberThreadPerBlock >> > (asteroidDataGPU->dev_m, 
      asteroidDataGPU->dev_fx, asteroidDataGPU->dev_fy, asteroidDataGPU->dev_fz, 
      asteroidDataGPU->dev_ax, asteroidDataGPU->dev_ay, asteroidDataGPU->dev_az, size);
    // Check for any errors launching the kernel
    cudaStatus = cudaGetLastError();
    if (cudaSuccess != cudaStatus) {
      fprintf(stderr, "accelerationKernel launch failed: %s\n", cudaGetErrorString(cudaStatus));
      goto Error;
    }
    // cudaDeviceSynchronize waits for the kernel to finish, and returns
    // any errors encountered during the launch.
    cudaStatus = cudaDeviceSynchronize();
    if (cudaSuccess != cudaStatus) {
      fprintf(stderr, "cudaDeviceSynchronize returned error code %d after launching accelerationKernel!\n", cudaStatus);
      goto Error;
    }
  }

  chrono->startV[id] = std::chrono::high_resolution_clock::now();
  //Velocity Kernel
  {
    // Launch a kernel on the GPU with one thread for each element.
    velocityKernel << <numberBlocks, kNumberThreadPerBlock >> > (
      asteroidDataGPU->dev_vx, asteroidDataGPU->dev_vy, asteroidDataGPU->dev_vz, 
      asteroidDataGPU->dev_ax, asteroidDataGPU->dev_ay, asteroidDataGPU->dev_az, 
      asteroidDataGPU->dev_t, size);
    // Check for any errors launching the kernel
    cudaStatus = cudaGetLastError();
    if (cudaSuccess != cudaStatus) {
      fprintf(stderr, "velocityKernel launch failed: %s\n", cudaGetErrorString(cudaStatus));
      goto Error;
    }
    // cudaDeviceSynchronize waits for the kernel to finish, and returns
    // any errors encountered during the launch.
    cudaStatus = cudaDeviceSynchronize();
    if (cudaSuccess != cudaStatus) {
      fprintf(stderr, "cudaDeviceSynchronize returned error code %d after launching velocityKernel!\n", cudaStatus);
      goto Error;
    }
  }
  chrono->startP[id] = std::chrono::high_resolution_clock::now();

  //Position Kernel
  {
    // Launch a kernel on the GPU with one thread for each element.
    positionKernel << <numberBlocks, kNumberThreadPerBlock >> > (
      asteroidDataGPU->dev_x, asteroidDataGPU->dev_y, asteroidDataGPU->dev_z,
      asteroidDataGPU->dev_vx, asteroidDataGPU->dev_vy, asteroidDataGPU->dev_vz, 
      asteroidDataGPU->dev_ax, asteroidDataGPU->dev_ay, asteroidDataGPU->dev_az, 
      asteroidDataGPU->dev_t, size);
    // Check for any errors launching the kernel
    cudaStatus = cudaGetLastError();
    if (cudaSuccess != cudaStatus) {
      fprintf(stderr, "positionKernel launch failed: %s\n", cudaGetErrorString(cudaStatus));
      goto Error;
    }
    // cudaDeviceSynchronize waits for the kernel to finish, and returns
    // any errors encountered during the launch.
    cudaStatus = cudaDeviceSynchronize();
    if (cudaSuccess != cudaStatus) {
      fprintf(stderr, "cudaDeviceSynchronize returned error code %d after launching positionKernel!\n", cudaStatus);
      goto Error;
    }
  }
  chrono->endP[id] = std::chrono::high_resolution_clock::now();

  

  //Copy back the information from GPU buffer to Host
  cudaStatus = cudaMemcpy(t, asteroidDataGPU->dev_t, sizeof(float), cudaMemcpyDeviceToHost);
  if (cudaSuccess != cudaStatus) {
    fprintf(stderr, "cudaMemcpy failed!");
    goto Error;
  }

  cudaStatus = cudaMemcpy(x, asteroidDataGPU->dev_x, size * sizeof(double), cudaMemcpyDeviceToHost);
  if (cudaStatus != cudaSuccess) {
    fprintf(stderr, "cudaMemcpy failed!");
    goto Error;
  }

  cudaStatus = cudaMemcpy(y, asteroidDataGPU->dev_y, size * sizeof(double), cudaMemcpyDeviceToHost);
  if (cudaStatus != cudaSuccess) {
    fprintf(stderr, "cudaMemcpy failed!");
    goto Error;
  }

  cudaStatus = cudaMemcpy(z, asteroidDataGPU->dev_z, size * sizeof(double), cudaMemcpyDeviceToHost);
  if (cudaStatus != cudaSuccess) {
    fprintf(stderr, "cudaMemcpy failed!");
    goto Error;
  }

  cudaStatus = cudaMemcpy(vx, asteroidDataGPU->dev_vx, size * sizeof(float), cudaMemcpyDeviceToHost);
  if (cudaStatus != cudaSuccess) {
    fprintf(stderr, "cudaMemcpy failed!");
    goto Error;
  }

  cudaStatus = cudaMemcpy(vy, asteroidDataGPU->dev_vy, size * sizeof(float), cudaMemcpyDeviceToHost);
  if (cudaStatus != cudaSuccess) {
    fprintf(stderr, "cudaMemcpy failed!");
    goto Error;
  }

  cudaStatus = cudaMemcpy(vz, asteroidDataGPU->dev_vz, size * sizeof(float), cudaMemcpyDeviceToHost);
  if (cudaStatus != cudaSuccess) {
    fprintf(stderr, "cudaMemcpy failed!");
    goto Error;
  }

  chrono->endUpdate[id] = std::chrono::high_resolution_clock::now();
 
Error:


  return cudaStatus;
}



void endAsteroidBeltCuda(AsteroidBeltDeviceData& asteroidDataGPU) {

  cudaFree(asteroidDataGPU.dev_t);
  cudaFree(asteroidDataGPU.dev_m);

  cudaFree(asteroidDataGPU.dev_x);
  cudaFree(asteroidDataGPU.dev_y);
  cudaFree(asteroidDataGPU.dev_z);

  cudaFree(asteroidDataGPU.dev_vx);
  cudaFree(asteroidDataGPU.dev_vy);
  cudaFree(asteroidDataGPU.dev_vz);

  cudaFree(asteroidDataGPU.dev_ax);
  cudaFree(asteroidDataGPU.dev_ay);
  cudaFree(asteroidDataGPU.dev_az);

  cudaFree(asteroidDataGPU.dev_fx);
  cudaFree(asteroidDataGPU.dev_fy);
  cudaFree(asteroidDataGPU.dev_fz);

  cudaFree(asteroidDataGPU.dev_outputData);
  cudaFree(asteroidDataGPU.dev_outputSize);
}


