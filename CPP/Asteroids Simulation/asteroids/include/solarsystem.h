#ifndef __SOLARSYSTEM_H__
#define __SOLARSYSTEM_H__ 1

#include <vector>
#include <stdint.h>
#include <cmath>
#include "SFML/Graphics.hpp"
#include "quadtree.h"
#include "CL/cl.hpp" 
#include <chrono>
#include "cudasolarsystem.cuh"


const unsigned int kMinAsteroids = 10000;    //    10 000
const unsigned int kMaxAsteroids = 1000000;  // 1 000 000

//kg/m^3
const unsigned int kDensityC = 1700;
const unsigned int kDensityS = 3000;
const unsigned int kDensityM = 6000;

//%
const unsigned int kPercentageC = 75;
const unsigned int kPercentageS = 15;
const unsigned int kPercentageM = 10;

//m
const unsigned int kMinRC = 75000;
const unsigned int kMaxRC = 225000;

const unsigned int kMinRS = 50000;
const unsigned int kMaxRS = 150000;

const unsigned int kMinRM = 25000;
const unsigned int kMaxRM = 100000;

//Const
const double kG = 6.67543 * pow(10.0, -11);       // m^3 kg^-1 s^-2.
const double kAU = 1.496 * pow(10.0, 11);         // m^3
const double kMSun = 1.98847 * pow(10.0, 30);     // kg


const double kDistanceCenterToCenterTorus = kAU;
const double kInnerTubeRadi = kAU * 0.1;
const double kMaxWForceRange = 5000.0 ; //m
const double kMaxHForceRange = 5000.0 ; //m


//v = G * Msun / r





class SolarSystem
{
public:
  SolarSystem();
  ~SolarSystem();


  void initSolarSystem(uint32_t screenCenterX, uint32_t screenCenterY);

  void initAsteroidBelt();

  void updateAsteroidBelt(int id);

  void resetCL();

  void drawSolarSystem(sf::RenderWindow& window_, const bool asteroidBelt, const bool qt, const bool showQuery);

  Asteroid2 asteroidData;
  MeasureTime chronoSS;

  int currentAsteroidsNumber;
 
  

  float centerSSX;
  float centerSSY;
  time_t seed;

  float timeStep_;

  float* fx;
  float* fy;
  float* fz;

  float* ax;
  float* ay;
  float* az;




  //QuadTree
  QuadTree quadTree;
  int* outputAsteroids;
  int indexOutputAsteroids[kMaxAsteroids];

  int indexQueryToShow;


private:

  sf::CircleShape sun;
  sf::CircleShape innerRadi;
  sf::CircleShape outerRadi;

  //CUDA
  bool onlyonce = true;
  TestData data_a;
  float* dev_a;
  float* a;
  double* b;

  AsteroidBeltDeviceData asteroidDataGPU;

  float* dev_t = 0;
  float* dev_m = 0;

  double* dev_x = 0;
  double* dev_y = 0;
  double* dev_z = 0;

  float* dev_vx = 0;
  float* dev_vy = 0;
  float* dev_vz = 0;

  float* dev_ax = 0;
  float* dev_ay = 0;
  float* dev_az = 0;

  float* dev_fx = 0;
  float* dev_fy = 0;
  float* dev_fz = 0;

  int* dev_sizeOutput = 0;
  int* dev_outputData = 0;


  //OPENCL
  cl_platform_id platform;
  cl_device_id device;
  cl_context context;
  cl_program program;
  cl_command_queue queue;

  cl_kernel forceKernel;
  cl_kernel accelerationKernel;
  cl_kernel velocityKernel;
  cl_kernel positionKernel;
  cl_int  CLStatus;
  
  cl_mem forceXBuffer, forceYBuffer, forceZBuffer;
  cl_mem accelerationXBuffer, accelerationYBuffer, accelerationZBuffer;
  cl_mem velocityXBuffer, velocityYBuffer, velocityZBuffer;
  cl_mem positionXBuffer, positionYBuffer, positionZBuffer;
  cl_mem massBuffer, timeBuffer;
  cl_mem outputDataBuffer, outputSizeBuffer;


  

  cl_int num_groups;


  cl_kernel testKernel;
  size_t local_size, global_size;


  //QuadTree



};








#endif // __SOLARSYSTEM_H__