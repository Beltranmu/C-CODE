#include "screenss.h"
#include <thread>


void ScreenSS::init(int limX, int limY, int size) {

  for (int i = 0; i < size; ++i) {
    //mAsteroids[i].position = sf::Vector2f(rand() % limX, rand() % limY);
    //mAsteroids[i].color.r = rand() % 255;
    //mAsteroids[i].color.g = rand() % 255;
    //mAsteroids[i].color.b = rand() % 255;
    mAsteroids[i].position = sf::Vector2f(0,0);
    mAsteroids[i].color.r = 255;
    mAsteroids[i].color.g = 255;
    mAsteroids[i].color.b = 255;
  }
}




void updateTexturePositionWorker2(int init, int end ,sf::VertexArray& mAsteroids, const int offset ) {

  init = init + offset;
  end = end + offset;
  for (int i = init; i < end; ++i) {
    mAsteroids[i].position.x = 10.0f;
    mAsteroids[i].position.y = 100.0f;
  }
}

void updateTexturePositionWorker1(const int init, const int end, sf::VertexArray* mAsteroids,
                                  double* x, double* y, const int ratioX,
                                  const float centerSX, const float centerSY,
                                  const double invRatio) {
  for (int i = init; i < end; ++i) {
    mAsteroids[0][i].position.x = (ratioX * x[i] * invRatio) + centerSX;
    mAsteroids[0][i].position.y = (ratioX * y[i] * invRatio) + centerSY;
  }
}


void ScreenSS::update(double* x, double* y, const int size,
                      float centerSX, float centerSY, int ratioX,
                      double invRatio, const int mSize) {


  const int kNThreadsPosition1 = 10;

  std::thread* currantAThreads = new std::thread[kNThreadsPosition1];
  int stepPositions = size / kNThreadsPosition1;

  for (int i = 0; i < kNThreadsPosition1; ++i) {
    
    if (i == kNThreadsPosition1 - 1) {
      currantAThreads[i] = std::thread(updateTexturePositionWorker1, i * stepPositions, (i + 1) * stepPositions
        + size % kNThreadsPosition1,
        &mAsteroids, x, y , ratioX,centerSX, centerSY, invRatio  );
    }
    else {
      currantAThreads[i] = std::thread(updateTexturePositionWorker1, i * stepPositions, (i + 1) * stepPositions,
        &mAsteroids, x, y, ratioX, centerSX, centerSY, invRatio);
    }
  }

  /*const int kNThreadsPosition2 = 10;
  int stepPosition2 = (mSize - size) / kNThreadsPosition2;
  std::thread* restAThreads = new std::thread[kNThreadsPosition2];
  int offset = size;

  for (int i = 0; i < kNThreadsPosition2; ++i) {
    restAThreads[i] = std::thread(updateTexturePositionWorker2, i * stepPosition2, (i + 1) * stepPosition2, mAsteroids, offset );
    /*if (i == kNThreadsPosition2 - 1) {
      restAThreads[i] = std::thread(updateTexturePositionWorker2, i * stepPositions2, (i + 1) * stepPositions2 +
        (mSize - size) % kNThreadsPosition2, mAsteroids, offset);
    }
  }  */
  
  for (int i = 0; i < kNThreadsPosition1; i++) {
    currantAThreads[i].join();
  }

  /*for (int i = 0; i < kNThreadsPosition2; i++) {
    restAThreads[i].join();
  }*/
}

ScreenSS::~ScreenSS() {}