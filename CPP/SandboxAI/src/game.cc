#include "game.h"
#include "imgui.h"
#include "imgui-SFML.h"
#include "SFML/System.hpp"
#include "b_loader.h"
#include "agent.h"
#include "voronoi.h"

Game::Game(){

  delta_time_ = 0.0f;
  
  intgmType = 0;

  fps.main_game = 60;
  fps.input_ = -1;
  fps.ai = 1;
  fps.world = 1;
  fps.draw_ = -1;
  imguifps = 0;

  fps.second_per_frame = 1/(float)fps.main_game;

  selectedAgentID = -1;
  targetRow = 0;
  targetCol = 0;
  possibleNextTarget = -1;
  startAddPattern = false;
  number_movement = 0; 
  voronoiInitialized = false;
  numberVPoint = 0;
  movement_type = Agent::PatternMovement::kPatternMovement_None;
}

Game::~Game() {

}

void Game::init(uint32_t w_width, uint32_t w_height) {

 // srand(time(NULL));
  w_width_ = w_width;
  w_height_ = w_height;
  voronoi.map.size.width = (float)w_width_;
  voronoi.map.size.height = (float)w_height_;
  voronoi.map.widthStreets = 20;

  w_.create(sf::VideoMode(w_width_, w_height_), "AI WINDOW");
  srand(time(NULL));


  ImGui::SFML::Init(w_);

  map_texture_.loadFromFile("../../data/gfx/maps/map_03_960x704_layout ABGS.png");
  map_sprite_ = sf::Sprite(map_texture_);
  //Board
  BoardFromImage(&board_, "../../data/gfx/maps/map_03_120x88_cost.png");

 //initEnemy
  board_.units_[0].currentPos = 0;
  






}

void Game::input() {
  
  while(w_.pollEvent(events_)){
    ImGui::SFML::ProcessEvent(events_);

    if(events_.type == sf::Event::Closed ||
       ((events_.type == sf::Event::KeyPressed) && (events_.key.code == sf::Keyboard::Escape))) {
      w_.close();
    }

    
    if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {

      selectedAgentID = board_.getAgent(w_);

    }
  }
}

void Game::update(float delta_time) {
  if(voronoi.pausedParabola){
    voronoi.d += 0.5f;

  }
}




void Game::draw() {
  if(gmType != VORONOI_TYPE){
    board_.drawBoard(&w_, possibleNextTarget, gmType);
  }else{

  voronoi.draw(&w_);
  voronoi.calculateParabolaDraw();

  }
  //possibleNextTarget = -1;
  //board_.drawLBoard(&w_);
}

void Game::end() {

  ImGui::SFML::Shutdown();
}

void Game::ImguiGame(){

  

  ImGui::Begin("FPS Controller");

  ImGui::TextColored(ImVec4(1, 0, 1, 1), "FPS:%d", imguifps);
  ImGui::SliderInt("Input", &fps.input_, -1, 60);
  if (fps.input_ == 0) fps.input_ = -1;
  ImGui::SliderInt("AI", &fps.ai, -1, 60);
  if (fps.ai == 0) fps.ai = -1;
  ImGui::SliderInt("World", &fps.world, -1, 60);
  if (fps.world == 0) fps.world = -1;
  ImGui::SliderInt("Draw", &fps.draw_, -1, 60);
  if (fps.draw_ == 0) fps.draw_ = -1;



  const char* elems_names[MAX_TYPE] = { "SandBox", "Voronoi", "PathFinding" };
  const char* elem_name = (intgmType >= 0 && intgmType < MAX_TYPE) ? elems_names[intgmType] : "Unknown";
  ImGui::SliderInt("Mode", &intgmType, 0, MAX_TYPE - 1, elem_name);
  gmType = (GameModeType)intgmType;

  

  switch (gmType)
  {
  case Game::NORMAL_TYPE: ImguiSandBox(); voronoiInitialized = false; break;
  case Game::VORONOI_TYPE: 
    ImGui::TextColored(ImVec4(0, 1, 0, 1), "INFO");
    ImGui::Text("Firts you decided the number of points\nyou want to do the voronoi and\nthen click in Create Points");
    ImGui::Text("After that, you click on \nCalculate Voronoi and you only have \nto wait");
    
    
    ImguiVoronoi(); break;
  case Game::PATHFINDING_TYPE: ImguiPathFinding(); voronoiInitialized = false; break;

  default: break;
  }

  ImGui::End();

}

void Game::ImguiSandBox(){



  ImGui::Begin("Agent Controller");

  ImGui::TextColored(ImVec4(0, 1, 0, 1), "CONTROLS");
  ImGui::Text("Right click on a agent to select it");

  if (selectedAgentID == -1) {
    ImGui::TextColored(ImVec4(1, 0, 1, 1), "Agent: None");  // Agent
  }
  else {
    ImGui::TextColored(ImVec4(1, 0, 1, 1), "Agent: %d", selectedAgentID);  // Agent

  ImGui::Text("Movement Type:");
    const char* elems_names[MAX_TYPE] = { "Random", "Patrol", "Track(Pac Man)" };
    const char* elem_name = (board_.units_[selectedAgentID].movementType >= 0 
      && board_.units_[selectedAgentID].movementType < Agent::Movement::kMovement_None) ? 
      elems_names[board_.units_[selectedAgentID].movementType] : "Unknown";
    //ImGui::SliderInt("Mode", &intgmType, 0, MAX_TYPE - 1, elem_name);

    ImGui::SliderInt("Movement", (int*)&board_.units_[selectedAgentID].movementType, Agent::Movement::kMovement_Random, Agent::Movement::kMovement_Track, elem_name);

    if (board_.units_[selectedAgentID].movementType == Agent::Movement::kMovement_Pattern) {
      if (ImGui::Button("Start Addition Pattern mode")) {
        startAddPattern = true;
      }

      if (startAddPattern) {
        if (ImGui::Button("Wait")) { movement_type = Agent::PatternMovement::kPatternMovement_Wait; }
        if (ImGui::Button("Forward")) { movement_type = Agent::PatternMovement::kPatternMovement_Forward; }
        if (ImGui::Button("Turn 180")) { movement_type = Agent::PatternMovement::kPatternMovement_Turn180; }
        if (ImGui::Button("Turn Left 90")) { movement_type = Agent::PatternMovement::kPatternMovement_TurnLeft90; }
        if (ImGui::Button("Turn Right 90")) { movement_type = Agent::PatternMovement::kPatternMovement_TurnRight90; }
        ImGui::InputInt("Number Movements", &number_movement);
        if (ImGui::Button("Add Pattern")) {
          board_.units_[selectedAgentID].addMovement2Patern(movement_type, number_movement);
          number_movement = 1;
          movement_type = Agent::PatternMovement::kPatternMovement_None;
        }
        if (ImGui::Button("End Addition Pattern mode")) {
          startAddPattern = false;
        }
        const char* name_movement = "NONE";
        switch (movement_type)
        {
        case Agent::PatternMovement::kPatternMovement_Wait: name_movement = "WAIT"; break;
        case Agent::PatternMovement::kPatternMovement_Forward: name_movement = "FORWARD"; break;
        case Agent::PatternMovement::kPatternMovement_Turn180: name_movement = "TURN180"; break;
        case Agent::PatternMovement::kPatternMovement_TurnLeft90: name_movement = "TURNLEFT90"; break;
        case Agent::PatternMovement::kPatternMovement_TurnRight90: name_movement = "TURNRIGHT90"; break;
        }
        ImGui::TextColored(ImVec4(1, 0, 0, 1), "PATTERN TO ADD:");
        ImGui::Text("Movement Type: %s || Steps: %d", name_movement, number_movement);


        //Show the current pattern
        ImGui::TextColored(ImVec4(0, 1, 0, 1), "CURRENT PATTERN");
        const char* name_mov = "NONE";

        for (int i = 0; i < board_.units_[selectedAgentID].index_movementArray; ++i) {
          switch (board_.units_[selectedAgentID].movementArray[i])
          {
          case Agent::PatternMovement::kPatternMovement_Wait: name_mov = "WAIT"; break;
          case Agent::PatternMovement::kPatternMovement_Forward: name_mov = "FORWARD"; break;
          case Agent::PatternMovement::kPatternMovement_Turn180: name_mov = "TURN180"; break;
          case Agent::PatternMovement::kPatternMovement_TurnLeft90: name_mov = "TURNLEFT90"; break;
          case Agent::PatternMovement::kPatternMovement_TurnRight90: name_mov = "TURNRIGHT90"; break;
          }
          //ImGui::Text("Movement Type: %s || Steps: %d", name_mov, board_.units_[selectedAgentID].movementCounterArray[i]);
          ImGui::TextColored(ImVec4(1, 1, 0, 1), "Movement Type: %s || Steps: %d", name_mov, board_.units_[selectedAgentID].movementCounterArray[i]);
        }

      }

    }

    if (board_.units_[selectedAgentID].movementType == Agent::Movement::kMovement_Track) {

      ImGui::SliderInt("Target Row:", &targetRow, 0, board_.height_ - 1);
      ImGui::SliderInt("Target Col:", &targetCol, 0, board_.width_ - 1);
      board_.rowcol2Index(targetRow, targetCol, &possibleNextTarget);

      if (ImGui::Button("Confirm Tile"))
        board_.rowcol2Index(targetRow, targetCol, &board_.units_[selectedAgentID].currentTarget);
    }
  }

  ImGui::End();



 
}

void Game::ImguiVoronoi() {
  ImGui::Begin("Voronoi");

  ImGui::InputInt("Number of points", (int*)&numberVPoint);
  if (ImGui::Button("Custom Init (TEST)")) {
    voronoi.clear();
    voronoi.customInit2();
    voronoiInitialized = true;
  }
  if (ImGui::Button("Create Points")) {
    voronoi.clear();
    voronoi.init(numberVPoint);
    voronoiInitialized = true;
  } 


  ImGui::Checkbox("Reduce Poly", &voronoi.reducedPoly);

  if (voronoiInitialized) {
    ImGui::Checkbox("Sites Info", &voronoi.showSitesInfo);
    ImGui::Checkbox("Sectors", &voronoi.drawSectors);
    ImGui::Checkbox("Intersection Points", &voronoi.drawIPoints);
    ImGui::Text("That values use to calculate \n Voronoi polygons if you change\nthem it does not may work properly");
    ImGui::InputFloat("Precision", &voronoi.marginSamePoint );
    ImGui::InputFloat("Same solutions", &voronoi.sameSolMargin);
    ImGui::InputFloat("Step Parabole", &voronoi.stepParabole, 0.00001f, 0.0001f, "%1.6f");
    ImGui::InputFloat("Max range X parabole", &voronoi.maxX);
    if (ImGui::Button("Calculate Voronoi")) {
      //voronoi.calculateParabola();
      voronoi.calculateSites();
    } 
    if (ImGui::Button("Reset")) {
      for (int i = 0; i < (int)voronoi.sites.size(); ++i) {
        voronoi.sites[i].perimetralLines.clear();
      };
      voronoi.solutionsVoronoi.clear();
    }
    if (ImGui::Button("Pause  Draw Parabola")) {
      voronoi.pausedParabola = !voronoi.pausedParabola;
    }
    for (int i = 0; i < (int)voronoi.paraboleDraw.size(); ++i) {
      ImGui::TextColored(ImVec4(1, 1, 1, 1), "Ecuacion: x = %fy^2 %fy %f", voronoi.paraboleDraw[i].x, voronoi.paraboleDraw[i].y, voronoi.paraboleDraw[i].z);
    }
    ImGui::SliderFloat("Direztriz line", &voronoi.d, 0, 1500);
    ImGui::SliderFloat("Horizontal line", &voronoi.horizontal, 0, voronoi.map.size.height);
    
    for (int n = 0; n < (int)voronoi.sites.size(); n++) {

      float red = ((float)n + 1.0f) / (float)voronoi.sites.size();
      ImGui::TextColored(ImVec4(1, red, red, 1), "Point: %d", n);
      char name[255];
      sprintf(name, "PositionX % d", n);
      ImGui::SliderFloat(name, &voronoi.sites[n].point.x, 0, voronoi.map.size.width);
      sprintf(name, "PositionY % d", n);
      ImGui::SliderFloat(name, &voronoi.sites[n].point.y, 0, voronoi.map.size.height);
    }

  }

  ImGui::End();

  if(voronoi.showSitesInfo){
    ImGui::Begin("Sites Info");   
    for (int i = 0; i < (int)voronoi.sites.size(); i++) {
      char name[255];
      sprintf(name, "Site %d", i);
      if (ImGui::TreeNode(name))
      {
        char name1[255], name2[255], name3[255], colorLabel[255];
        sprintf(name3, "Site %d-> Info", i);
        sprintf(name1, "Site %d-> UpperPoints", i);
        sprintf(name2, "Site %d-> BottomPoints", i);
        sprintf(colorLabel, "Site %d-> Color", i);
        float color[4];
        color[0] = voronoi.sites[i].color.r;
        color[1] = voronoi.sites[i].color.g;
        color[2] = voronoi.sites[i].color.b;
        color[3] = voronoi.sites[i].color.a;
        ImGui::ColorEdit4(colorLabel,color);
        voronoi.sites[i].color = sf::Color(color[0]*255, color[1] * 255, color[2] * 255, color[3] * 255);
        if (ImGui::TreeNode(name3)) {
          int b;
          for (b = 0; b < (int)voronoi.sites[i].perimetralLines.size(); b++) {
            ImGui::TextColored(ImVec4(1, 1, 1, 1), "Point %d: 1->(%f,%f)| 2->(%f,%f)", b, voronoi.sites[i].perimetralLines[b].p1.x,
              voronoi.sites[i].perimetralLines[b].p1.y,
              voronoi.sites[i].perimetralLines[b].p2.x,
              voronoi.sites[i].perimetralLines[b].p2.y);
          }
          ImGui::TreePop();
        }

        if(ImGui::TreeNode(name1)){
          int b;
          for(b = 0; b < (int)voronoi.auxsites[i].upperPoints.size(); b++){
            ImGui::TextColored(ImVec4(1, 1, 1, 1), "Point %d: (%f,%f)", b, voronoi.auxsites[i].upperPoints[b].x,
              voronoi.auxsites[i].upperPoints[b].y);
          }
          ImGui::TreePop();
        }

        if (ImGui::TreeNode(name2)) {
            int b;
            for (b = 0; b < (int)voronoi.auxsites[i].bottonPoints.size(); b++) {
              ImGui::TextColored(ImVec4(1, 1, 1, 1), "Point %d: (%f,%f)", b, voronoi.auxsites[i].bottonPoints[b].x,
                voronoi.auxsites[i].bottonPoints[b].y);
            }
           ImGui::TreePop();
        }

        ImGui::TreePop();
      }
    }
    ImGui::End();
  }
}

void Game::ImguiPathFinding(){

  ImGui::Begin("Path Finding");

  ImGui::Text("1-.");
  ImGui::Text("Choose the start cell ->"); ImGui::SameLine(); ImGui::TextColored(ImVec4(0, 0, 1, 1), " Blue S");
  ImGui::Text("Choose the end cell ->"); ImGui::SameLine(); ImGui::TextColored(ImVec4(1, 0, 0, 1), " Red M ");

  ImGui::Text("2-. Choose only one type of distance");
  ImGui::SliderInt("InitPosRow", &board_.targetRowI, 0, board_.height_ - 1);
  ImGui::SliderInt("InitPosCol", &board_.targetColI, 0, board_.width_ - 1);

  ImGui::SliderInt("DstPosRow", &board_.targetRowD, 0, board_.height_ - 1);
  ImGui::SliderInt("DstPosCol", &board_.targetColD, 0, board_.width_ - 1);
  ImGui::Checkbox("Manhattan Distance", &board_.aPath_.manhattanD);
  ImGui::Checkbox("Euclidean Distance", &board_.aPath_.euclideanD);
  ImGui::Checkbox("Chebyshov Distance", &board_.aPath_.chebyshovD);

  //ImGui::Checkbox("Allow Diagonals", &board_.aPath_.allowDiagonals);
  ImGui::Checkbox("Draw Board", &board_.drawLogical);

  ImGui::Text("3-. Choose only one type of distance");
  if (ImGui::Button("Check A Star")) {
    int origin = board_.targetColI + board_.targetRowI * board_.width_;
    int dst = board_.targetColD + board_.targetRowD * board_.width_;
    if (origin != dst) {
      board_.aPath_.calculatePath(&board_, origin, dst);
    }
  }

  char* types[3];
  types[0] = "Manhattan";
  types[1] = "Euclidean";
  types[2] = "Chebyshov";

  //ImGui::BeginChild("Paths");
  for (int i = 0; i < (int)board_.aPath_.currentPaths.size(); ++i) {
    ImGui::BeginChild("Path");
    char treeName[255];
    sprintf(treeName, "Path From %d -> %d Type: %s", board_.aPath_.currentPaths[i].origin,
      board_.aPath_.currentPaths[i].destination, types[board_.aPath_.currentPaths[i].type]);
    char name[255];
    sprintf(name, "Draw Path %d", i);
    ImGui::Checkbox(name, &board_.aPath_.currentPaths[i].draw);
    if (ImGui::TreeNode(treeName))
    {
      for (int p = 0; p < (int)board_.aPath_.currentPaths[i].path.size(); ++p) {
        ImGui::TextColored(ImVec4(1, 1, 1, 1), "%d : %d| G:%f + H:%f =  F:%f| ParentCell %d", p,
          board_.aPath_.currentPaths[i].path[p].cellID,
          board_.aPath_.currentPaths[i].path[p].g,
          board_.aPath_.currentPaths[i].path[p].score - board_.aPath_.currentPaths[i].path[p].g,
          board_.aPath_.currentPaths[i].path[p].score,
          board_.aPath_.currentPaths[i].path[p].parentCellID);
      }
      ImGui::TreePop();
    }
    ImGui::EndChild();
  }

  ImGui::End();


}


void Game::mainLoop(){

  init(1280, 704);

  sf::Clock sec_clock, main_clock, ia_clock, input_clock;
  sf::Clock world_clock, draw_clock, imgui_clock;

  int frames=0;
  

  while(w_.isOpen()){

    main_clock.restart();

    // Input Update
    if (input_clock.getElapsedTime().asSeconds() > 1.0f / fps.input_ || fps.input_ == -1) {
      input_clock.restart();
      input();
    }
   
    // IA Update
    if((ia_clock.getElapsedTime().asSeconds() > 1.0f/fps.ai || fps.ai == -1) 
      && (!startAddPattern)){
      ia_clock.restart();
      board_.unitMovement();
    }

    // World Update
    if (world_clock.getElapsedTime().asSeconds() > 1.0f / fps.world|| fps.world == -1) {
      world_clock.restart();
      update(delta_time_);      
    }
    //voronoi.calculateParabolaDraw();
    ImGui::SFML::Update(w_, imgui_clock.restart());


    ImguiGame();


    //Draw
    if (draw_clock.getElapsedTime().asSeconds() > 1.0f / (float)fps.draw_ || fps.draw_ == -1) {
      draw_clock.restart();
      w_.clear(sf::Color(0, 0, 0, 255));
      draw();
    }

    ImGui::SFML::Render(w_);
    w_.display();   
   
    if (sec_clock.getElapsedTime().asSeconds() > 1 ) {
      imguifps = frames;
      frames = 0;
      sec_clock.restart();      
    }

    frames++;

    do {
    } while (main_clock.getElapsedTime().asSeconds() < fps.second_per_frame);
   
  }
  
  end();
}