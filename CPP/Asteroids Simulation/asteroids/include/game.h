#ifndef __GAME_H__
#define __GAME_H__ 1

#include <stdio.h>
#include <stdint.h>
#include "SFML/Graphics.hpp"
#include "screenss.h"



const time_t kSeed = 12398933;

class SolarSystem;

class Game
{
public:
  Game();
  ~Game();

  void mainLoop();
  void asteoridImGui();
  void init(uint32_t w_width, uint32_t w_height);
  void input();
  void update(float delta_time);
  void draw();
  void end();
  

  

  bool endSim;
  bool closeFinishSim;

  //Window
  sf::RenderWindow window_;   ///<  SFML window.
  int32_t w_width_;           ///<  With of the window.
  int32_t w_height_;          ///<  Height of the window.

  //Events
  sf::Event events_;          ///<  SFML events.

  //Simulation variables

  bool asteroidBelt;
  bool showQuadTree;
  bool showQuery;
  bool showMoreInfo;
  bool showQueryMoreInfo;

  //SolarSystem
  SolarSystem* solarSystem_;

  ScreenSS* solarSystemTexture_;

  typedef enum DemoState {
    Ready = 0,
    Running,
    MAX

  };
  struct DemoParameters {
    
    DemoState state;
    int numberOfRepetitons;
    int currentRepetitions;

  };
  DemoParameters demoParams;
  struct TimeMeasured {
    //Demo
    float perFrameTime;
    float totalTime;

    //Init
    float posMassTime;
    float velocityTime;
    float initTime;

    //QuadTree
    float queryQTTime;
    float updateQTTime;
    float quadTreeTime;

    //Upate Asteroid
    float forceKernelTime;
    float accelerationKernelTime;
    float velocityKernelTime;
    float positionKernelTime;
    float updateAsteroidTime;


  };
  TimeMeasured timeOutput;




private:


  void AsteroidsMoreInfo();
  void QueryMoreInfo();

  char* wNames[2];
  char* oNames[4];
  char* sNames[2];


};








#endif // __GAME_H__