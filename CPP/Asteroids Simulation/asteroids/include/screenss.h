#ifndef __SCRENSS_H__
#define __SCRENSS_H__ 1

#include "SFML/Graphics.hpp"


class ScreenSS: public sf::Drawable, public sf::Transformable
{
public:
	ScreenSS(unsigned int count):
		mAsteroids(sf::Points, count)
	{
	};
	~ScreenSS();
  void init(int limX, int limY, int size);
  void update(double* x, double* y, const int size,
    float centerSX, float centerSY, int ratioX,
    double invRatio,  const int mSize);

private:
	sf::VertexArray mAsteroids;

  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
  {
    // apply the transform
    states.transform *= getTransform();

    // our particles don't use a texture
    states.texture = NULL;

    // draw the vertex array
    target.draw(mAsteroids, states);
  }
};






#endif //__SCREENSS_H__