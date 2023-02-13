#define PROGRAM_FILE "../../src/clsolarsystem.cl"
#define KERNEL_FUNC "updateAsteroidCL"


#include "solarsystem.h"
#include <thread>

#include "cuda_runtime.h"



const char* kProgramFile = "../../src/clsolarsystem.cl\0";


cudaError_t initAsteroidBeltCuda(AsteroidBeltDeviceData& asteroidDataGPU, int size);


cudaError_t updateAsteroidBeltCuda(double* x, double* y, double* z, float* vx, float* vy, float* vz,
  float* m, float* t, int size, int* outputData, int* outputSize, AsteroidBeltDeviceData* asteroidDataGPU,
   MeasureTime* chrono, int id);
void endAsteroidBeltCuda(AsteroidBeltDeviceData& asteroidDataGPU);









SolarSystem::SolarSystem() {
  currentAsteroidsNumber = kMinAsteroids;
  //currentAsteroidsNumber = 100000;

  centerSSX = 960;
  centerSSY = 540;
 
  timeStep_ = 2.0f;

  indexQueryToShow = 0;

  platform = nullptr;

  dev_t = nullptr;
  dev_m = nullptr;

  dev_x = nullptr;
  dev_y = nullptr;
  dev_z = nullptr;

  dev_vx = nullptr;
  dev_vy = nullptr;
  dev_vz = nullptr;

  dev_ax = nullptr;
  dev_ay = nullptr;
  dev_az = nullptr;

  dev_fx = nullptr;
  dev_fy = nullptr;
  dev_fz = nullptr;

  dev_sizeOutput = nullptr;
  dev_outputData = nullptr;
  

}


void SolarSystem::initSolarSystem(uint32_t screenCenterX, uint32_t screenCenterY){

  quadTree.init(0,0, 1.2f*kAU, 1.2f*kAU);

 
  asteroidData.x = (double*)malloc(kMaxAsteroids * sizeof(double));
  asteroidData.y = (double*)malloc(kMaxAsteroids * sizeof(double));
  asteroidData.z = (double*)malloc(kMaxAsteroids * sizeof(double));



  asteroidData.vx = (float*)malloc(kMaxAsteroids * sizeof(float));
  asteroidData.vy = (float*)malloc(kMaxAsteroids * sizeof(float));
  asteroidData.vz = (float*)malloc(kMaxAsteroids * sizeof(float));

  asteroidData.m = (float*)malloc(kMaxAsteroids * sizeof(float));
  asteroidData.r = (float*)malloc(kMaxAsteroids * sizeof(float));

  //Allocate mem for the querey of the quadtree
  const int maxFA = kMaxAsteroidPerAsteroid;
  
  outputAsteroids = (int*)malloc(maxFA * kMaxAsteroids* sizeof(int));


  

  initAsteroidBelt();




  //CUDA
#ifdef CUDASIM


  
#ifdef DEBUG
  int* count = new int;

  cudaError_t deviceStatus = cudaGetDeviceCount(count);
  if (*count == 0) {
    printf("No CUDA devices found\n");
    delete count;
    return;
  }

  cudaDeviceProp* deviceProperties = new cudaDeviceProp;

  for (int i = 0; i < *count; i++) {
    deviceStatus = cudaGetDeviceProperties(deviceProperties, i);
    if (deviceStatus != cudaSuccess) {
      printf("An error occurred in accessing properties of device %d \n", i);
      delete count;
      return;
    }

    printf("The properties of device  %d are\n", i);
    printf("Name: %s\n", deviceProperties->name);
    printf("Core Clock Rate: %d Mhz\n", deviceProperties->clockRate / 1000);
    printf("Number of Compute Units: %d\n", deviceProperties->multiProcessorCount);
    printf("Max Threads per Compute Unit: %d\n", deviceProperties->maxThreadsPerMultiProcessor);
    printf("Warp Size: %d\n", deviceProperties->warpSize);
    printf("Total Memory: %zd MB \n", deviceProperties->totalGlobalMem / 1048576);
    printf("Level 2 Cache Size: %d KB \n", deviceProperties->l2CacheSize / 1024);

  }

  delete count;
  delete deviceProperties;
#endif



  cudaError_t cudaStatus = initAsteroidBeltCuda(asteroidDataGPU, kMaxAsteroids);


#else
  
  //OPENCL
  {
    //Set Up OPENCL


    /* Identify a platform */
    {
      CLStatus = clGetPlatformIDs(1, &platform, NULL);
      if (CLStatus < 0) {
        perror("Couldm't identify a platform");
        exit(1);
      }

      //Access a device
      //GPU
      CLStatus = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
      if (CL_DEVICE_NOT_FOUND == CLStatus) {
        //Try CPU
        CLStatus = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 1, &device, NULL);
      }

      if (CLStatus < 0) {
        perror("Couldn't access any device");
        exit(1);
      }
    }


    context = clCreateContext(NULL, 1, &device, NULL, NULL, &CLStatus);
    if (CLStatus < 0) {
      perror("Couldn't create a context");
      exit(1);
    }

    /* Build program */

    //program = build_program(context, device, PROGRAM_FILE);
    {
      FILE* program_handle;
      char* program_buffer;
      char* program_log;
      size_t program_size, log_size;


      /*Read program file and place content into buffer */

      program_handle = fopen(kProgramFile, "r");
      if (NULL == program_handle) {
        perror("Couldn't find the program file");
        exit(1);
      }

      fseek(program_handle, 0, SEEK_END);
      program_size = ftell(program_handle);
      rewind(program_handle);
      program_buffer = (char*)malloc(program_size + 1);
      program_buffer[program_size] = '\0';

      fread(program_buffer, sizeof(char), program_size, program_handle);
      fclose(program_handle);

      /*Create program from file

      Creates a program from the source code in the add_numbers.cl file.
      Specifically, the code reads the file's content into a char array
      called program_buffer, and then calls clCreateProgramWithSource.
      */
      program = clCreateProgramWithSource(context, 1,
        (const char**)&program_buffer, &program_size, &CLStatus);

      if (CLStatus < 0) {
        perror("Couldn't create the program");
        exit(1);
      }

      free(program_buffer);

      /*Build program

      The fourth parameter accepts options that configure the compilation.
      These are similar to the flags used by gcc. For example, you can
      define a macro with the option -DMACRO=VALUE and turn off optimization
      with -cl-opt-disable.
      */

      CLStatus = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
      if (CLStatus < 0) {
        // Find size of log and print to std outpur
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG,
          0, NULL, &log_size);

        program_log = (char*)malloc(log_size + 1);
        program_log[log_size] = '\0';
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG,
          log_size + 1, program_log, NULL);
        printf("%s\n", program_log);

        free(program_log);
        exit(1);
      }
    }







    /* Create a command queue

    Does not support profiling or out-of-order-execution
    */
    queue = clCreateCommandQueue(context, device, 0, &CLStatus);
    if (CLStatus < 0) {
      perror("Couldn't create a command queue");
      exit(1);
    };

    /* Create a kernel */
    {
      forceKernel = clCreateKernel(program, "updateAsteroidForce", &CLStatus);
      if (CLStatus < 0) {
        perror("Couldn't create a updateAsteroidForce");
        exit(1);
      };

      accelerationKernel = clCreateKernel(program, "updateAsteroidAcceleration", &CLStatus);
      if (CLStatus < 0) {
        perror("Couldn't create a updateAsteroidAcceleration");
        exit(1);
      };

      velocityKernel = clCreateKernel(program, "updateAsteroidVelocity", &CLStatus);
      if (CLStatus < 0) {
        perror("Couldn't create a updateAsteroidVelocity");
        exit(1);
      };

      positionKernel = clCreateKernel(program, "updateAsteroidPosition", &CLStatus);
      if (CLStatus < 0) {
        perror("Couldn't create a updateAsteroidPosition");
        exit(1);
      };
    }


    global_size = currentAsteroidsNumber;
    local_size = 1;
    num_groups = global_size / local_size;

    //Create the buffers
    {
      //Mass & time
      massBuffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
        kMaxAsteroids * sizeof(float), asteroidData.m, &CLStatus);

      timeBuffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
        sizeof(float), &timeStep_, &CLStatus);

      //Position
      positionXBuffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
        kMaxAsteroids * sizeof(double), asteroidData.x, &CLStatus);

      positionYBuffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
        kMaxAsteroids * sizeof(double), asteroidData.y, &CLStatus);

      positionZBuffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
        kMaxAsteroids * sizeof(double), asteroidData.z, &CLStatus);

      //Velocity
      velocityXBuffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
        kMaxAsteroids * sizeof(float), asteroidData.vx, &CLStatus);

      velocityYBuffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
        kMaxAsteroids * sizeof(float), asteroidData.vy, &CLStatus);

      velocityZBuffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
        kMaxAsteroids * sizeof(float), asteroidData.vz, &CLStatus);

      //Acceleration
      accelerationXBuffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
        kMaxAsteroids * sizeof(float), asteroidData.vz, &CLStatus);

      accelerationYBuffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
        kMaxAsteroids * sizeof(float), asteroidData.vz, &CLStatus);

      accelerationZBuffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
        kMaxAsteroids * sizeof(float), asteroidData.vz, &CLStatus);


      //Force
      forceXBuffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
        kMaxAsteroids * sizeof(float), asteroidData.vz, &CLStatus);

      forceYBuffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
        kMaxAsteroids * sizeof(float), asteroidData.vz, &CLStatus);

      forceZBuffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
        kMaxAsteroids * sizeof(float), asteroidData.vz, &CLStatus);


      outputDataBuffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
        kMaxAsteroidPerAsteroid * kMaxAsteroids * sizeof(int), outputAsteroids, &CLStatus);

      outputSizeBuffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
        kMaxAsteroids * sizeof(int), indexOutputAsteroids, &CLStatus);


      if (CLStatus < 0) {
        perror("Couldn't create a buffer");
        exit(1);
      };
    }


    //Bind kernel arguments
    {
      //Force Kernel
      CLStatus = clSetKernelArg(forceKernel, 0, sizeof(cl_mem), &massBuffer); // <=====INPUT
      CLStatus |= clSetKernelArg(forceKernel, 4, sizeof(cl_mem), &positionXBuffer); // <=====INPUT
      CLStatus |= clSetKernelArg(forceKernel, 5, sizeof(cl_mem), &positionYBuffer); // <=====INPUT
      CLStatus |= clSetKernelArg(forceKernel, 6, sizeof(cl_mem), &positionZBuffer); // <=====INPUT
      CLStatus |= clSetKernelArg(forceKernel, 7, sizeof(cl_mem), &outputDataBuffer); // <=====INPUT
      CLStatus |= clSetKernelArg(forceKernel, 8, sizeof(cl_mem), &outputSizeBuffer); // <=====INPUT
      CLStatus |= clSetKernelArg(forceKernel, 1, sizeof(cl_mem), &forceXBuffer); // <=====OUTPUT
      CLStatus |= clSetKernelArg(forceKernel, 2, sizeof(cl_mem), &forceYBuffer); // <=====OUTPUT
      CLStatus |= clSetKernelArg(forceKernel, 3, sizeof(cl_mem), &forceZBuffer); // <=====OUTPUT


      //Acceleration Kernel
      CLStatus = clSetKernelArg(accelerationKernel, 0, sizeof(cl_mem), &massBuffer); // <=====INPUT
      CLStatus |= clSetKernelArg(accelerationKernel, 1, sizeof(cl_mem), &forceXBuffer); // <=====INPUT
      CLStatus |= clSetKernelArg(accelerationKernel, 2, sizeof(cl_mem), &forceYBuffer); // <=====INPUT
      CLStatus |= clSetKernelArg(accelerationKernel, 3, sizeof(cl_mem), &forceZBuffer); // <=====INPUT
      CLStatus |= clSetKernelArg(accelerationKernel, 4, sizeof(cl_mem), &accelerationXBuffer); // <=====OUTPUT
      CLStatus |= clSetKernelArg(accelerationKernel, 5, sizeof(cl_mem), &accelerationYBuffer); // <=====OUTPUT
      CLStatus |= clSetKernelArg(accelerationKernel, 6, sizeof(cl_mem), &accelerationZBuffer); // <=====OUTPUT


      //Velocity Kernel
      CLStatus = clSetKernelArg(velocityKernel, 6, sizeof(cl_mem), &timeBuffer); // <=====INPUT
      CLStatus |= clSetKernelArg(velocityKernel, 3, sizeof(cl_mem), &accelerationXBuffer); // <=====INPUT
      CLStatus |= clSetKernelArg(velocityKernel, 4, sizeof(cl_mem), &accelerationYBuffer); // <=====INPUT
      CLStatus |= clSetKernelArg(velocityKernel, 5, sizeof(cl_mem), &accelerationZBuffer); // <=====INPUT
      CLStatus |= clSetKernelArg(velocityKernel, 0, sizeof(cl_mem), &velocityXBuffer); // <=====OUTPUT
      CLStatus |= clSetKernelArg(velocityKernel, 1, sizeof(cl_mem), &velocityYBuffer); // <=====OUTPUT
      CLStatus |= clSetKernelArg(velocityKernel, 2, sizeof(cl_mem), &velocityZBuffer); // <=====OUTPUT

      //Position Kernel
      CLStatus = clSetKernelArg(positionKernel, 9, sizeof(cl_mem), &timeBuffer); // <=====INPUT
      CLStatus |= clSetKernelArg(positionKernel, 3, sizeof(cl_mem), &velocityXBuffer); // <=====INPUT
      CLStatus |= clSetKernelArg(positionKernel, 4, sizeof(cl_mem), &velocityYBuffer); // <=====INPUT
      CLStatus |= clSetKernelArg(positionKernel, 5, sizeof(cl_mem), &velocityZBuffer); // <=====INPUT
      CLStatus |= clSetKernelArg(positionKernel, 6, sizeof(cl_mem), &accelerationXBuffer); // <=====INPUT
      CLStatus |= clSetKernelArg(positionKernel, 7, sizeof(cl_mem), &accelerationYBuffer); // <=====INPUT
      CLStatus |= clSetKernelArg(positionKernel, 8, sizeof(cl_mem), &accelerationZBuffer); // <=====INPUT
      CLStatus |= clSetKernelArg(positionKernel, 0, sizeof(cl_mem), &positionXBuffer); // <=====OUTPUT
      CLStatus |= clSetKernelArg(positionKernel, 1, sizeof(cl_mem), &positionYBuffer); // <=====OUTPUT
      CLStatus |= clSetKernelArg(positionKernel, 2, sizeof(cl_mem), &positionZBuffer); // <=====OUTPUT
    }

  }


#endif 


  

#ifdef CUDASIM
  printf("CUDA\n");
#else
  printf("OpenCL\n");
#endif


  centerSSX = (float)screenCenterX;
  centerSSY = (float)screenCenterY;

 

  sun.setRadius(10.0f);
  sun.setOrigin(10.0f, 10.0f);
  sun.setFillColor(sf::Color::Yellow);
  sun.setOutlineColor(sf::Color::Yellow);
  sun.setPosition(centerSSX, centerSSY);


  float r = 405.0f;
  innerRadi.setRadius(r);
  innerRadi.setOrigin(r, r);
  innerRadi.setFillColor(sf::Color::Transparent);
  innerRadi.setOutlineColor(sf::Color::Cyan);
  innerRadi.setOutlineThickness(1.0f);
  innerRadi.setPosition(centerSSX, centerSSY);
  
  r = 495.0f;
  outerRadi.setRadius(r);
  outerRadi.setOrigin(r, r);
  outerRadi.setFillColor(sf::Color::Transparent);
  outerRadi.setOutlineColor(sf::Color::Blue);
  outerRadi.setOutlineThickness(1.0f);
  outerRadi.setPosition(centerSSX, centerSSY);

  
  
}


void initPositionWorker(Asteroid2& asteroidData, time_t seed, int init, int end) {

  srand((seed * init + end) * init);
  for (int i = init; i < end; ++i) {
    float alfai = ((float)(rand() % 3600))/10.0f;
    float betai = ((float)(rand() % 3600))/10.0f;
    float alfa = (alfai * 3.141592f) / 180.0f;
    float beta = (betai * 3.141592f) / 180.0f;
    float radM = ((float)(rand() % 1000))*0.001f;
    float cBeta = cosf(beta);
    double innerRadi = kInnerTubeRadi * radM;
    asteroidData.x[i] = (kDistanceCenterToCenterTorus + innerRadi * cBeta) * cosf(alfa);
    asteroidData.y[i] = (kDistanceCenterToCenterTorus + innerRadi * cBeta) * sinf(alfa);
    asteroidData.z[i] = kInnerTubeRadi * sin(beta);
  }

}

void initMassRadiusWorker(Asteroid2& asteroidData, time_t seed, int init, int end) {

  srand((seed - end + init) * end);

  for (int i = init; i < end; i++) {
    int type = rand() % 100;

    int minR = 0;
    int maxR = 0;
    float density = 0.0f;
    // C Type
    if (type < kPercentageC) {
      minR = kMinRC;
      maxR = kMaxRC;
      density = kDensityC;
    }
    // S Type
    else if (type > kPercentageC && type < kPercentageC + kPercentageS) {
      minR = kMinRS;
      maxR = kMaxRS;
      density = kDensityS;
    }
    // M Type
    else {
      minR = kMinRM;
      maxR = kMaxRM;
      density = kDensityM;
    }

    //Mass and radius
    float r = float(rand() % (minR - maxR) + kMinRC);
    asteroidData.r[i] = r;
    asteroidData.m[i] = 4.0f / 3.0f * 3.141592f * r * r * r * density;
  }
}

void initVelocityWorker(Asteroid2& asteroidData, time_t seed, int init, int end) {

  srand(seed * init * end * end);

  //Initial Velocity
  for (int i = init; i < end; ++i) {

    float dx = asteroidData.x[i];
    float dy = asteroidData.y[i];

    float distance = (dx * dx) + (dy * dy);
    distance = sqrt(distance);

    float cosAsteroid = dx / (distance);
    float sinAsteroid = dy / (distance);

    double mv = kG * kMSun / distance;
    mv *= 0.1;

    asteroidData.vx[i] =  mv* sinAsteroid;
    asteroidData.vy[i] = -mv * cosAsteroid;
    asteroidData.vz[i] = 0.0f;
  }

}

void SolarSystem::initAsteroidBelt() {

  //Populate the vector

  const int kNThreadsPosition = 10;
  const int kNThreadsMR = 10;
  const int kNThreadsVelocity = 10;

  
  std::thread* positionThreads = new std::thread[kNThreadsPosition];
  std::thread* mrThreads = new std::thread[kNThreadsPosition];
  std::thread* velocityThreads = new std::thread[kNThreadsPosition];

  std::chrono::steady_clock::time_point start, end, end1;
  std::chrono::microseconds duration;
  start = std::chrono::high_resolution_clock::now();


  int stepPosition = kMaxAsteroids / kNThreadsPosition;
  for (int i = 0; i < kNThreadsPosition; i++) {
    if (i != (kNThreadsPosition - 1)) {
      positionThreads[i] = std::thread(initPositionWorker, asteroidData, seed, i * stepPosition, (i + 1) * stepPosition);
    }
    else {
      positionThreads[i] = std::thread(initPositionWorker, asteroidData, seed, i * stepPosition, (i + 1) * stepPosition + 
        kMaxAsteroids % kNThreadsPosition);
    }
  }

  stepPosition = kMaxAsteroids / kNThreadsMR;
  for (int i = 0; i < kNThreadsMR; i++) {
    if (i != (kNThreadsMR - 1)) {
      mrThreads[i] = std::thread(initMassRadiusWorker, asteroidData, seed, i * stepPosition, (i+1)*stepPosition);
    }
    else {
      mrThreads[i] = std::thread(initMassRadiusWorker, asteroidData, seed, i * stepPosition, (i+1)*stepPosition + 
        kMaxAsteroids % kNThreadsMR);
    }
  }

  for (int i = 0; i < kNThreadsPosition; i++) {
    positionThreads[i].join();
  }
  for (int i = 0; i < kNThreadsMR; i++) {
    mrThreads[i].join();
  }

  end = std::chrono::steady_clock::now();

  stepPosition = kMaxAsteroids / kNThreadsVelocity;
  for (int i = 0; i < kNThreadsVelocity; i++) {
    if (i != (kNThreadsVelocity - 1)){
      velocityThreads[i] = std::thread(initVelocityWorker, asteroidData, seed, i * stepPosition, (i + 1) * stepPosition);
    }
    else {
      velocityThreads[i] = std::thread(initVelocityWorker, asteroidData, seed, i * stepPosition, (i + 1) * stepPosition+
        kMaxAsteroids % kNThreadsVelocity);
    }
  }

  for (int i = 0; i < kNThreadsVelocity; i++) {
    velocityThreads[i].join();
  }

  end1 = std::chrono::high_resolution_clock::now();
  duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  printf("Duration initialization positions and masses %f\n", duration.count() / 1000000.0f);
  duration = std::chrono::duration_cast<std::chrono::microseconds>(end1 - end);
  printf("Duration initialization velocities %f\n", duration.count() / 1000000.0f);



#ifdef CLSIM

  




#endif
 


}

void SolarSystem::resetCL() {

  clEnqueueWriteBuffer(queue, positionXBuffer, CL_TRUE, 0, kMaxAsteroids * sizeof(float),
    asteroidData.x, 0, NULL, NULL);
  clEnqueueWriteBuffer(queue, positionYBuffer, CL_TRUE, 0, kMaxAsteroids * sizeof(float),
    asteroidData.y, 0, NULL, NULL);
  clEnqueueWriteBuffer(queue, positionZBuffer, CL_TRUE, 0, kMaxAsteroids * sizeof(float),
    asteroidData.z, 0, NULL, NULL);

  clEnqueueWriteBuffer(queue, velocityXBuffer, CL_TRUE, 0, kMaxAsteroids * sizeof(float),
    asteroidData.vx, 0, NULL, NULL);
  clEnqueueWriteBuffer(queue, velocityYBuffer, CL_TRUE, 0, kMaxAsteroids * sizeof(float),
    asteroidData.vy, 0, NULL, NULL);
  clEnqueueWriteBuffer(queue, velocityZBuffer, CL_TRUE, 0, kMaxAsteroids * sizeof(float),
    asteroidData.vz, 0, NULL, NULL);
}


void SolarSystem::updateAsteroidBelt(int id) {

  chronoSS.starQTUpdate[id] = std::chrono::high_resolution_clock::now();

  quadTree.update(&asteroidData, currentAsteroidsNumber);

  chronoSS.startQTQuery[id] = std::chrono::high_resolution_clock::now();

  for (int i = 0; i < currentAsteroidsNumber; ++i) {
    indexOutputAsteroids[i] = 0;
    quadTree.query(asteroidData.x[i], asteroidData.y[i],
      kMaxWForceRange, kMaxHForceRange,
      (outputAsteroids + (i * kMaxAsteroidPerAsteroid)), &indexOutputAsteroids[i], &asteroidData, i);
  }

  chronoSS.endQT[id] = std::chrono::high_resolution_clock::now();

  // CUDA
#if CUDASIM

  cudaError_t cudaStatus;
  cudaStatus = updateAsteroidBeltCuda(asteroidData.x, asteroidData.y, asteroidData.z,
    asteroidData.vx, asteroidData.vy, asteroidData.vz,
    asteroidData.m, &timeStep_, currentAsteroidsNumber,
    outputAsteroids, indexOutputAsteroids,
    &asteroidDataGPU, &chronoSS, id);
  if (cudaStatus != cudaSuccess) {
    fprintf(stderr, "updateAsteroidBeltCuda failed!");

  }
  // cudaDeviceReset must be called before exiting in order for profiling and
  // tracing tools such as Nsight and Visual Profiler to show complete traces.




#else   //OPENCL
  chronoSS.startUpdate[id] = std::chrono::high_resolution_clock::now();
  global_size = currentAsteroidsNumber;


  clEnqueueWriteBuffer(queue, timeBuffer, CL_TRUE, 0, sizeof(float), &timeStep_,
    0, NULL, NULL);

  clEnqueueWriteBuffer(queue, outputDataBuffer, CL_TRUE, 0, 
                      kMaxAsteroidPerAsteroid * kMaxAsteroids * sizeof(int), outputAsteroids,
    0, NULL, NULL);

  clEnqueueWriteBuffer(queue, outputSizeBuffer, CL_TRUE, 0,
                      kMaxAsteroids * sizeof(int), indexOutputAsteroids,
    0, NULL, NULL);


  chronoSS.startF[id] = std::chrono::high_resolution_clock::now();
  CLStatus = clEnqueueNDRangeKernel(queue, forceKernel, 1, NULL, &global_size,
    &local_size, 0, NULL, NULL);
  if (CLStatus < 0) {
    perror("Couldn't enqueue the forceKernel");
    exit(1);
  }


  if (CLStatus < 0) {
    perror("Couldn't read force kernel output\n");
  }


  chronoSS.startA[id] = std::chrono::high_resolution_clock::now();
  CLStatus = clEnqueueNDRangeKernel(queue, accelerationKernel, 1, NULL, &global_size,
    &local_size, 0, NULL, NULL);
  if (CLStatus < 0) {
    perror("Couldn't enqueue the accelerationKernel");
    exit(1);
  }



  if (CLStatus < 0) {
    perror("Couldn't read force kernel output\n");
  }

  chronoSS.startV[id] = std::chrono::high_resolution_clock::now();
  CLStatus = clEnqueueNDRangeKernel(queue, velocityKernel, 1, NULL, &global_size,
    &local_size, 0, NULL, NULL);
  if (CLStatus < 0) {
    perror("Couldn't enqueue the velocityKernel");
    exit(1);
  }



  if (CLStatus < 0) {
    perror("Couldn't read force kernel output\n");
  }




  chronoSS.startP[id] = std::chrono::high_resolution_clock::now();
  CLStatus = clEnqueueNDRangeKernel(queue, positionKernel, 1, NULL, &global_size,
    &local_size, 0, NULL, NULL);

  if (CLStatus < 0) {
    perror("Couldn't enqueue the positionKernel");
    exit(1);
  }
  chronoSS.endP[id] = std::chrono::high_resolution_clock::now();
  CLStatus = clEnqueueReadBuffer(queue, positionXBuffer, CL_TRUE, 0,
    currentAsteroidsNumber * sizeof(double),
    asteroidData.x, 0, NULL, NULL);

  CLStatus = clEnqueueReadBuffer(queue, positionYBuffer, CL_TRUE, 0,
    currentAsteroidsNumber * sizeof(double),
    asteroidData.y, 0, NULL, NULL);

  CLStatus = clEnqueueReadBuffer(queue, positionZBuffer, CL_TRUE, 0,
    currentAsteroidsNumber * sizeof(double),
    asteroidData.z, 0, NULL, NULL);

  if (CLStatus < 0) {
    perror("Couldn't read force kernel output\n");
  }
  chronoSS.endUpdate[id] = std::chrono::high_resolution_clock::now();


#endif

}

void SolarSystem::drawSolarSystem(sf::RenderWindow& window_,const bool asteroidBelt , const bool qt , const bool drawQuery)
{
  window_.draw(sun);
  if (asteroidBelt) {
    window_.draw(innerRadi);
    window_.draw(outerRadi);
  }


  if (qt) {
    quadTree.draw(window_, kAU, centerSSX, centerSSY);
  }



  //Draw test queries
  if (drawQuery) {

    int j = indexQueryToShow;
    sf::RectangleShape r;
    float w = kMaxWForceRange * 450.0f /kAU;
    float h = kMaxHForceRange * 450.0f /kAU;

    
    r.setFillColor(sf::Color::Red);
    r.setOutlineColor(sf::Color::Red);
    r.setSize(sf::Vector2f(2.0f, 2.0f));
    r.setOrigin(0, 0);

    for (int i = 0; i < indexOutputAsteroids[j]; ++i) {

      float auxX = (450.0 * asteroidData.x[outputAsteroids[j * kMaxAsteroidPerAsteroid + i]] / kAU) + centerSSX;
      float auxY = (450.0 * asteroidData.y[outputAsteroids[j * kMaxAsteroidPerAsteroid + i]] / kAU) + centerSSY;
      r.setPosition(auxX, auxY);

      window_.draw(r);
    }
    
  }
}

SolarSystem::~SolarSystem() {

#if CUDASIM

  endAsteroidBeltCuda(asteroidDataGPU);
  if (cudaSuccess != cudaDeviceReset()) {
    fprintf(stderr, "cudaDeviceReset failed!");
  }
#else// OpenCL SIM

  clReleaseCommandQueue(queue);
  clReleaseProgram(program);
  clReleaseContext(context);

  clReleaseMemObject(forceXBuffer);
  clReleaseMemObject(forceYBuffer);
  clReleaseMemObject(forceZBuffer);
  clReleaseMemObject(accelerationXBuffer);
  clReleaseMemObject(accelerationYBuffer);
  clReleaseMemObject(accelerationZBuffer);
  clReleaseMemObject(velocityXBuffer);
  clReleaseMemObject(velocityYBuffer);
  clReleaseMemObject(velocityZBuffer);
  clReleaseMemObject(positionXBuffer);
  clReleaseMemObject(positionYBuffer);
  clReleaseMemObject(positionZBuffer);
  clReleaseMemObject(massBuffer);
  clReleaseMemObject(timeBuffer);




  

#endif
  if (nullptr != outputAsteroids) { free(outputAsteroids); }

  if (nullptr != asteroidData.x) { free(asteroidData.x); }
  if (nullptr != asteroidData.y) { free(asteroidData.y); }
  if (nullptr != asteroidData.z) { free(asteroidData.z); }

  if (nullptr != asteroidData.vx) { free(asteroidData.vx); }
  if (nullptr != asteroidData.vy) { free(asteroidData.vy); }
  if (nullptr != asteroidData.vz) { free(asteroidData.vz); }

  if (nullptr != asteroidData.m) { free(asteroidData.m); }
  if (nullptr != asteroidData.r) { free(asteroidData.r); }





}