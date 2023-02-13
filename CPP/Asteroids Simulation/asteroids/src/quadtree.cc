#include "quadtree.h"


QuadTree::QuadTree()
{
  currentCapacity_ = 0;
  topR_ = nullptr;
  topL_ = nullptr;
  botR_ = nullptr;
  botL_ = nullptr;

  x_ = 0.0f;
  y_ = 0.0f;
  w_ = 0.0f;
  h_ = 0.0f;
  
}

QuadTree::~QuadTree() {


}

void CreateAsterodisdList() {


}

void QuadTree::init(const float x, const float y, const float w, const float h) {

  x_ = x;
  y_ = y;
  w_ = w;
  h_ = h;

}

void QuadTree::update(Asteroid2* a,  int size) {

 
  clear();


  for (int i = 0; i < size; ++i) {
    insertPoint(a, i);
  }

  
}

void QuadTree::insertPoint(Asteroid2* a, int astID) {

  sf::Vector2f p = { (float)a->x[astID], (float)a->y[astID] };
  if (!isInside(p)) {
    return;
  }
  //Can insert new point
  if (currentCapacity_ < kMaxCapacity_) {

    asteroids_[currentCapacity_] = astID;
    currentCapacity_++;
  }
  else {

    // Not subdivided yet
    if (nullptr == topL_) {
      subdivide();
    }

    topL_->insertPoint(a, astID);
    topR_->insertPoint(a, astID);
    botL_->insertPoint(a, astID);
    botR_->insertPoint(a, astID);
  }
}

bool QuadTree::isInside(const sf::Vector2f& p) {

  float x1 = x_ - w_;
  float x2 = x_ + w_;
  float y1 = y_ - h_;
  float y2 = y_ + h_;

  return (x1 <= p.x && p.x < x2&& y1 <= p.y && p.y < y2);

}

void QuadTree::subdivide() {

  //Create four Quadtree
  topL_ = new QuadTree();
  topR_ = new QuadTree();
  botL_ = new QuadTree();
  botR_ = new QuadTree();

  float w1 = w_ * 0.5f;
  float h1 = h_ * 0.5f;

  float x1 = x_ - w1;
  float x2 = x_ + w1;

  float y1 = y_ - h1;
  float y2 = y_ + h1;


  topL_->init(x1, y1, w1, h1);
  topR_->init(x2, y1, w1, h1);
  botL_->init(x1, y2, w1, h1);
  botR_->init(x2, y2, w1, h1);




}

void QuadTree::draw(sf::RenderWindow& window, const double AU, float sx, float sy) {

  sf::RectangleShape r;

  r.scale(1.0f, 1.0f);
  r.setFillColor(sf::Color::Transparent);
  r.setOutlineColor(sf::Color::Cyan);
  r.setOutlineThickness(2.0f);
  float w = 450 * w_ / AU;
  float h = 450 * h_ / AU;
  r.setSize(sf::Vector2f(2.0f*w, 2.0f*h));

  float x = 450 * x_ / AU;
  float y = 450 * y_ / AU;

  x = x  - w + sx;
  y = y  - h + sy;
  r.setPosition(x , y );



  window.draw(r);


  if (nullptr != topL_) {
    topL_->draw(window, AU, sx, sy);
  }

  if (nullptr != topR_) {
    topR_->draw(window, AU, sx, sy);
  }

  if (nullptr != botL_) {
    botL_->draw(window, AU, sx, sy);
  }

  if (nullptr != botR_) {
    botR_->draw(window, AU, sx, sy);
  }


}

bool QuadTree::isInside(const float x, const float y, const float w, const float h) {

  float x1_ = x_ - w_;
  float x2_ = x_ + w_;
  float y1_ = y_ - h_;
  float y2_ = y_ + h_;

  float x1 = x - w;
  float x2 = x + w;
  float y1 = y - h;
  float y2 = y + h;

  return !(x1_ > x2 || x2_ <x1 || y1_ > y2 || y2_ < y1);
}

bool QuadTree::isInside(const sf::Vector2f p, const float x, const float y, const float w, const float h) {


  float x1 = x - w;
  float x2 = x + w;
  float y1 = y - h;
  float y2 = y + h;

  return (x1 <= p.x && p.x < x2 && y1 <= p.y && p.y < y2);
}

void QuadTree::clear(){

  if (nullptr != topL_) {
    topL_->clear();
    topR_->clear();
    botL_->clear();
    botR_->clear();
  }


  free(topL_);
  topL_ = nullptr;
  free(topR_);
  topR_ = nullptr;
  free(botL_);
  botL_ = nullptr;
  free(botR_);
  botR_ = nullptr;


  currentCapacity_ = 0;



}

void QuadTree::query(const float x, const float y, const float w, const float h, int* output, int* size, Asteroid2* asteroidsData, 
                     const int currentAsteroidID) {


  if (!isInside(x, y, w, h)) {
    return;
  }

  sf::Vector2f p;
  for (int i = 0; i < currentCapacity_; ++i) {
    if (currentAsteroidID != asteroids_[i]) {
      p.x = (float)asteroidsData->x[asteroids_[i]];
      p.y = (float)asteroidsData->y[asteroids_[i]];
      if (isInside(p, x, y, w, h)) {
        if (*size < kMaxAsteroidPerAsteroid) {
          output[*size] = asteroids_[i];
          ++(*size);
        }
      }
    }
  }


  if (nullptr != topL_) {
    topL_->query(x, y, w, h, output, size, asteroidsData, currentAsteroidID);
    topR_->query(x, y, w, h, output, size, asteroidsData, currentAsteroidID);
    botL_->query(x, y, w, h, output, size, asteroidsData, currentAsteroidID);
    botR_->query(x, y, w, h, output, size, asteroidsData, currentAsteroidID);
  }
  return;

}
