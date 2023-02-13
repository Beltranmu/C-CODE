

__global const double kMSun = 1988470000000000000000000000000.0; // 1.98847 * pow(10.0, 30);
__global const double kG = 0.0000000000667543; //6.67543 * pow(10.0, -11);
__global const int kMaxNumberAsteroidDev = 80;

__kernel void updateAsteroidCL(__global int* data1, __global int* data2, __global int* res, __global int* res2) {
  uint global_addr, local_addr, id,id2;
  global_addr = get_global_id(0);
  local_addr = get_local_id(0);
  id = global_addr + local_addr * get_local_size(0);
  id2 = (get_global_size(0) * get_local_size(0) )- 1;
 

  res[id] = data1[id] + data2[id];
  res2[id] = data1[id] - data2[id];
   
}



__kernel void updateAsteroidForce(__global float* m, __global float* fx, __global float* fy,
  __global float* fz, __global double* x, __global double* y, __global double* z,
__global int* asteroidID, __global int* numberAsteroid) {


  uint i = get_global_id(0);
  uint j = get_local_id(0);



  double distance = x[i] * x[i] + y[i] * y[i] + z[i] * z[i];
  float moduleF = sqrt(distance);

  float dirX = x[i] / -moduleF;
  float dirY = y[i] / -moduleF;
  float dirZ = z[i] / -moduleF;

  float force = kG * ((kMSun * m[i]) / distance);

  fx[i] = force * dirX * 8500000;
  fy[i] = force * dirY * 8500000;
  fz[i] = force * dirZ * 8500000;



  int n = numberAsteroid[i];
  int offsetData = kMaxNumberAsteroidDev * i;
  for (int a = 0; a < n; ++a) {

    int asteroid = asteroidID[offsetData + a];
    float tmpX = x[asteroid] - x[i];
    float tmpY = y[asteroid] - y[i];
    float tmpZ = z[asteroid] - z[i];


    distance = tmpX * tmpX + tmpY * tmpY + tmpZ * tmpZ;
    if (distance == 0) { distance = 0.01f; }
    moduleF = 1 / sqrt(distance);
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
__kernel void updateAsteroidAcceleration(__global float* m, __global float* fx, __global float* fy,
  __global float* fz, __global float* ax, __global float* ay, __global float* az) {

  uint i = get_global_id(0);

  ax[i] = fx[i] / m[i];
  ay[i] = fy[i] / m[i];
  az[i] = fz[i] / m[i];


}
__kernel void updateAsteroidVelocity(__global float* vx, __global float* vy, __global float* vz,
  __global float* ax, __global float* ay, __global float* az, __global float* t) {


  uint i = get_global_id(0);
  float time = t[0];

  float velX = vx[i] + ax[i] * time;
  float velY = vy[i] + ay[i] * time;
  float velZ = vz[i] + az[i] * time;

  vx[i] = velX;
  vy[i] = velY;
  vz[i] = velZ;


}


__kernel void updateAsteroidPosition(__global double* x, __global double* y, __global double* z,
  __global float* vx, __global float* vy, __global float* vz, __global float* ax, __global float* ay, __global float* az
  , __global float* t) {


  int i = get_global_id(0);
  float time = t[0];

  float pX = x[i] + vx[i] * time + (ax[i] * time * time / 2.0f);
  float pY = y[i] + vy[i] * time + (ay[i] * time * time / 2.0f);
  float pZ = z[i] + vz[i] * time + (az[i] * time * time / 2.0f);

  x[i] = pX;
  y[i] = pY;
  z[i] = pZ;

}