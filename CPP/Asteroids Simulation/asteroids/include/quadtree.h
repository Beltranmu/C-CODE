
#ifndef __QUADTREE_H__
#define __QUADTREE_H__ 1


#include "SFML/Graphics.hpp"
#include <vector>

#if CUDASIM
const int kMaxAsteroidPerAsteroid = 200;
#else
const int kMaxAsteroidPerAsteroid = 80;
#endif

struct Asteroid2 {
  double* x;
  double* y;
  double* z;

  float* m;

  float* vx;
  float* vy;
  float* vz;

  float* r;

};



class QuadTree
{
public:
  QuadTree();
  ~QuadTree();
  void init(const float x, const float y, const float w, const float h);
  void update(Asteroid2*, int size);
  void insertPoint(Asteroid2* a,  int astID);
  bool isInside(const sf::Vector2f& p);



  void subdivide();
  void draw(sf::RenderWindow& window, const double AU, float sx, float sy);
  void query(const float x, const float y, const float w, const float h, int* output, int* size, Asteroid2* asteroidsData, const int currentAsteroidID);
  bool isInside(const float x, const float y, const float w, const float h);
  bool isInside(const sf::Vector2f p, const float x, const float y, const float w, const float h);

  void clear();

  const static int kMaxCapacity_ = 20;
  int currentCapacity_;
  int asteroids_[kMaxCapacity_];
  QuadTree* topR_;
  QuadTree* topL_;
  QuadTree* botR_;
  QuadTree* botL_;


  float x_;
  float y_;
  float w_;
  float h_;



private:

};


#endif //__QUADTREE_H__