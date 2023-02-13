/**
 * @brief Voronoi , Artificial intelligence demo , ESAT - 2022
 * @file voronoi.h
 * @author Ricardo Beltr�n Muriel <ricardobeltrandev@gmail.com
> 
 */


#ifndef __VORONOI_H__
#define __VORONOI_H__ 1

#include <vector>
#include <map>
#include <stdint.h>
#include "SFML/Graphics.hpp"
#include "SFML/System/Vector2.hpp"

const int kwidthStreet = 20;      ///< Width of the separations of the sites


//! Size struct.
     /*! Size of the box where the points are. */

struct Size{
  float width;        /*!< Width of the box. */ 
  float height;       /*!< Height of the box. */ 
};


//! Map struct.
     /*! Struct that represent the area in which voronoi is going to be calculated. */
struct Map{
  Size size;              /*!< Size of the area. */ 
  float widthStreets;     /*!< separation of each site. */ 
};


//! Line struct.
     /*! Struct that represent the area in which voronoi is going to be calculated. */
struct Line {
  sf::Vector2<float> mp;    /*!< Middle point of the line */ 
  sf::Vector2<float> p1;    /*!< First point of the line. */ 
  sf::Vector2<float> p2;    /*!< Second point of the line. */ 
};

//! Line struct v2.
     /*! Struct that represent the area in which voronoi is going to be calculated. */
struct LineP {
  sf::Vector2<float> p1;  /*!< First point of the line. */ 
  sf::Vector2<float> p2;  /*!< Second point of the line. */ 
  // p1-> Point, p2-> NextPoint,
};


//! Solution struct.
     /*!Represent a temporal solution for the voronoi algorithm */

struct Solution{    
  sf::Vector2<float> point;      /*!< solution point. */ 
  uint32_t n = 0;                /*!< First point of the line. */ 
  std::vector<int> parentSites;  /*!< Site where the solution is. */ 
};

//! Parabole struct.
     /*!Represent a parabole */
struct Parabole {
  sf::Vector3<float> parabole; /*!< solution point.  x = Ay^2 + By + C -> A = parabole.x, B = parabole.y, c = parabole.z */
  int parentPoint;             /*!< id of the focus point*/
};


//! Cells struct.
     /*! Represent cell/Site */
struct Cells{
  sf::Vector2<float> point;            /*!< point of the site*/    
  std::vector<LineP> perimetralLines;  /*!< lines of the polygon */
  sf::Color color;                     /*!< Color of each site  */
};


//! Cells struct.
     /*! Represent temporal site */

struct AuxCell {
  
  std::vector<sf::Vector2<float>> upperPoints;  /*!< Vector of point that are above the site point*/    
  std::vector<sf::Vector2<float>> bottonPoints; /*!< Vector of point that are down the site point*/    
};


//! Voronoi class.
     /*! Class dedicated to calculate and keep all the data for the Voronoi algorithm */
class Voronoi
{
public:
  Voronoi();
  ~Voronoi();

  /**
     *@brief Create the a random distribution and initialize the all need to make voronoi but always on the same way version 1
    */
  void customInit();

  /**
   *@brief Create the a random distribution and initialize the all need to make voronoi but always on the same way version 2
  */
  void customInit2();

  /**
  *@brief Create the a random distribution and initialize the all need to make voronoi.
  *@param int32_t nPoints number of sites that you want to have in voronoi diagram
 */
  void init(int32_t nPoints);

  /**
  *@brief Create the a random distribution and initialize the all need to make voronoi.
  *@param int32_t nPoints number of sites that you want to have in voronoi diagram
 */
  void draw(sf::RenderWindow* window);
  /**
  *@brief Clear all the information stored for the Voronoi diagram.
  */
  void clear();

  /**
 *@brief Calculate the visual paraboles.
 */
  void calculateParabolaDraw();


    /**
    *@brief Tell if a point is in the beach line or not
    *@return true if it is in the beach line, otherwise false;
    */
  bool isBeachLine(sf::Vector2<float> p);
  

  /**
 *@brief Calculate the voronoi diagram.
 */
  void calculateSites();

  /**
  *@brief Calculate the intersection of a parabole with and horizontal line.
  *@param Parabole p, the parabole
  *@param float y the horizontal line.
  */
  void intersectionParaboleHLine(Parabole p, float y);

  /**
  *@brief Calculate the intersection of a parabole with and vertical line.
  *@param Parabole p, the parabole
  *@param float x the vertical line.
  */
  void intersectionParaboleVLine(Parabole p, float x);

  /**
  *@brief Add a new solution to the system. that soultion may be obtained with the cut of two paraboles or a parabole and a line.
  *@param float x the X coord of the solution
  *@param float y the Y coord of the solution
  *@param  int32_t paraboleInde The id of the first parabole 
  *@param  int32_t parabole2Index = -1 The id of the second parabole . Default value -1
  */
  void addSolution(float x, float y,  int32_t paraboleIndex, int32_t parabole2Index = -1);

  /**
  *@brief Calculate the intersection between paraboles.
  *@param Parabole p, first parabole
  *@param Parabole q, second parabole
  */
  void intersectionParaboleParabole(Parabole p, Parabole q);



  float horizontal;              ///< y coordenate of the horizontal line
  float d;                       ///< X coordenate of the director line
  bool reducedPoly;              ///< If you want to calculate the normal voronoi = true y a little bit reduced = false;


  std::vector<Solution> solutions;              ///< Final solutions for the voronoi
  std::vector<Solution> solutionsVoronoi;       ///< Solutions for each loop of the parabole

  std::vector<sf::Vector3<float>> paraboleDraw; ///< Paraboles to draw
  std::vector<Solution> paraboleIPoints;        ///< Solutions for the intersection of the paraboles
  std::vector<Line> lines;                      ///< Square that surround the area of the voronoi area

  std::vector<Cells> sites;               ///< Square that surround the area of the voronoi area
  std::vector<AuxCell> auxsitesLittle;    ///< Temporal sites with reduced scale
  std::vector<AuxCell> auxsites;          ///< Temporal sites to order the points
  std::vector<Parabole> parabole;         ///< The paraboles for each step

  float stepParabole;     ///< Step for the director line
  float maxX;             ///<  Max distance of the director line   
  bool firstSol;          ///<  Max size of the board.
  
  Map map;                ///<  Area to calculate the voronoi.
  bool drawAllLine;       ///<  True if oy want to show the intersection lines.
  bool drawSectors;       ///<  True to draw the inside of the sector.
  bool drawIPoints;       ///<  True if you want to draw intersection point.  

  bool showSitesInfo;     ///<  True to show another window with more info about the sites.  
  float marginSamePoint;  ///<  Margin of for the same solutions.    
  float sameSolMargin;    ///<  Margin for the same solution.    
  bool pausedParabola;    ///< true  If the parabola to draw is paused, otherwise false

};
#endif //__VORONOI_H__



