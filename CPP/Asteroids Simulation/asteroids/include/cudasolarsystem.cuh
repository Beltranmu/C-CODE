#ifndef __CUDASOLARSYSTEM_CUH__
#define __CUDASOLARSYSTEM_CUH__ 1

#include <chrono>

const int kMaxRepetition = 18000;

struct AsteroidBeltDeviceData {
  double* dev_x;
  double* dev_y;
  double* dev_z;
  float* dev_vx;
  float* dev_vy;
  float* dev_vz;
  float* dev_ax;
  float* dev_ay;
  float* dev_az;
  float* dev_fx;
  float* dev_fy;
  float* dev_fz;
  float* dev_m;
  float* dev_t;
  int* dev_outputData;
  int* dev_outputSize;
};


struct MeasureTime {

  //Demo
  std::chrono::steady_clock::time_point start;
  std::chrono::steady_clock::time_point end;

  //Frame
  std::chrono::steady_clock::time_point startFrame[kMaxRepetition];
  std::chrono::steady_clock::time_point endFrame[kMaxRepetition];


  //QuadTree
  std::chrono::steady_clock::time_point starQTUpdate[kMaxRepetition];
  std::chrono::steady_clock::time_point startQTQuery[kMaxRepetition];
  std::chrono::steady_clock::time_point endQT[kMaxRepetition];

  //CUDA-OPENCL 
  std::chrono::steady_clock::time_point startF[kMaxRepetition];
  std::chrono::steady_clock::time_point startA[kMaxRepetition];
  std::chrono::steady_clock::time_point startV[kMaxRepetition];
  std::chrono::steady_clock::time_point startP[kMaxRepetition];
  std::chrono::steady_clock::time_point endP[kMaxRepetition];
  std::chrono::steady_clock::time_point startUpdate[kMaxRepetition];
  std::chrono::steady_clock::time_point endUpdate[kMaxRepetition];

  std::chrono::microseconds durationF;
  std::chrono::microseconds durationA;
  std::chrono::microseconds durationV;
  std::chrono::microseconds durationP;
  std::chrono::microseconds durationUdpate;
  std::chrono::microseconds durationFrame;
  std::chrono::microseconds durationTotal;


};



struct TestData {
  float* dev_a;

  double* dev_x;
  double* dev_y;
  double* dev_z;
  float* dev_vx;
  float* dev_vy;
  float* dev_vz;
  float* dev_ax;
  float* dev_ay;
  float* dev_az;
  float* dev_fx;
  float* dev_fy;
  float* dev_fz;
  float* dev_m;
  int* dev_outputData;
  int* dev_outputSize;
};

#endif