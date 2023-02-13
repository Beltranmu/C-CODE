#include "game.h"
#include "solarsystem.h"
#include "SFML/OpenGL.hpp"
#include "imgui.h"
#include "imgui-SFML.h"

#include <cstdio>





Game::Game()
{
  showMoreInfo = false;
  asteroidBelt = false;
  showQuadTree = false;
  showQuery = false;
  showQueryMoreInfo = false;
 


  

  endSim = false;
  closeFinishSim = false;
  solarSystem_ = new SolarSystem();

  demoParams.state = Ready;
  if (Running == demoParams.state) {
    solarSystem_->chronoSS.start = std::chrono::high_resolution_clock::now();
  }
  demoParams.numberOfRepetitons = 100;
  demoParams.currentRepetitions = 0;

  wNames[0] = "ASTEROIDS CUDA";
  wNames[1] = "ASTEROIDS OPENCL";

  oNames[0] = "CUDA DEBUG";
  oNames[1] = "OpenCL DEBUG";
  oNames[2] = "CUDA RELEASE";
  oNames[3] = "OpenCL RELEASE";

  sNames[0] = "x86";
  sNames[1] = "x64";
}

Game::~Game()
{
  if (nullptr != solarSystem_) {
    delete(solarSystem_);
  }
}



void Game::init(uint32_t w_width, uint32_t w_height) {

  srand(kSeed);

  w_width_ = w_width;
  w_height_ = w_height;
  solarSystemTexture_ = new ScreenSS(kMaxAsteroids);
  solarSystemTexture_->init(w_width * 2, w_height * 2, kMaxAsteroids);

  int id = 1;


#if CUDASIM
  id = 0;

#endif

  window_.create(sf::VideoMode(w_width, w_height), wNames[id], sf::Style::Default);
  window_.setActive();

  ImGui::SFML::Init(window_);
  ImGuiIO io = ImGui::GetIO();
  io.IniFilename = NULL;
  ImGui::LoadIniSettingsFromDisk("../../imgui.ini");
  
  solarSystem_->seed = kSeed;
  solarSystem_->initSolarSystem(w_width_/2, w_height_/2);

}



void Game::input() {

  while (window_.pollEvent(events_)) {
    ImGui::SFML::ProcessEvent(events_);
    if (events_.type == sf::Event::Closed ||
      ((events_.type == sf::Event::KeyPressed) && (events_.key.code == sf::Keyboard::Escape))) {
      window_.close();
    }
  }
}

void Game::update(float delta_time) {
  solarSystem_->updateAsteroidBelt(demoParams.currentRepetitions);
  solarSystemTexture_->update(solarSystem_->asteroidData.x, solarSystem_->asteroidData.y,
  solarSystem_->currentAsteroidsNumber, w_width_ /2, w_height_ / 2, 450.0f, 1.0 / kAU, kMaxAsteroids );

  ++demoParams.currentRepetitions;
  if (demoParams.currentRepetitions >= demoParams.numberOfRepetitons) {
    solarSystem_->chronoSS.endFrame[demoParams.currentRepetitions - 1] = std::chrono::high_resolution_clock::now();
    solarSystem_->chronoSS.end = std::chrono::high_resolution_clock::now();
    demoParams.state = Ready;

    demoParams.currentRepetitions = 0;

    //Calculate the results
    {
      timeOutput.forceKernelTime = 0.0f;
      timeOutput.accelerationKernelTime = 0.0f;
      timeOutput.velocityKernelTime = 0.0f;
      timeOutput.positionKernelTime = 0.0f;
      timeOutput.perFrameTime = 0.0f;
      timeOutput.totalTime = 0.0f;
      timeOutput.updateAsteroidTime = 0.0f;



      for (int i = 0; i < demoParams.numberOfRepetitons; ++i) {
        solarSystem_->chronoSS.durationF = std::chrono::duration_cast<std::chrono::microseconds>(
                                           solarSystem_->chronoSS.startA[i] -solarSystem_->chronoSS.startF[i]);
        timeOutput.forceKernelTime += solarSystem_->chronoSS.durationF.count();

        solarSystem_->chronoSS.durationA = std::chrono::duration_cast<std::chrono::microseconds>(
          solarSystem_->chronoSS.startV[i] - solarSystem_->chronoSS.startA[i]);
        timeOutput.accelerationKernelTime += solarSystem_->chronoSS.durationA.count();
        
        solarSystem_->chronoSS.durationV = std::chrono::duration_cast<std::chrono::microseconds>(
          solarSystem_->chronoSS.startP[i] - solarSystem_->chronoSS.startV[i]);
        timeOutput.velocityKernelTime += solarSystem_->chronoSS.durationV.count();

        solarSystem_->chronoSS.durationP = std::chrono::duration_cast<std::chrono::microseconds>(
          solarSystem_->chronoSS.endP[i] - solarSystem_->chronoSS.startP[i]);
        timeOutput.positionKernelTime += solarSystem_->chronoSS.durationP.count();

        solarSystem_->chronoSS.durationUdpate = std::chrono::duration_cast<std::chrono::microseconds>(
          solarSystem_->chronoSS.endUpdate[i] - solarSystem_->chronoSS.startUpdate[i]);
        timeOutput.updateAsteroidTime += solarSystem_->chronoSS.durationUdpate.count();

        solarSystem_->chronoSS.durationFrame = std::chrono::duration_cast<std::chrono::microseconds>(
          solarSystem_->chronoSS.endFrame[i] - solarSystem_->chronoSS.startFrame[i]);
        timeOutput.perFrameTime += solarSystem_->chronoSS.durationFrame.count();
      }

      solarSystem_->chronoSS.durationTotal = std::chrono::duration_cast<std::chrono::microseconds>(
        solarSystem_->chronoSS.end - solarSystem_->chronoSS.start);
      timeOutput.totalTime = solarSystem_->chronoSS.durationTotal.count()/1000000.0f;
    }

    //OpenFile

    char outPutBuffer[2048] ;
    FILE* fp = fopen("../../log/log.txt", "a+");
    if (NULL == fp) {
      fp = fopen("../../log/log.txt", "w");
      if (NULL == fp) {
        fp = fopen("log.txt", "w");
      }
    }

    int n = 0;
    int s = 0;

#ifdef s64
    s = 1;
#endif
#ifdef NDEBUG
    n = 2;
#endif
#ifdef CLSIM
    n++;
#endif // CLSIM

    float invTimes = 1.0f / (float)(demoParams.numberOfRepetitons);
    tm* date;
    time_t timeT = time(NULL);
    date = localtime(&timeT);
    n = sprintf(outPutBuffer,
      "______________RESULTS_____________\n\n"
      "- Date: %d/%d/%d || %02d:%02d:%02d\n"
      "- Api Type: %s  || %s\n"
      "- Number of Asteroids : %d\n"
      "- Number Repetitions (Frames) : %d\n"
      "- Total time : %f seconds\n"
      "- Average time per frame : %f, microseconds\n"
      "- Average time Update Asteroids : %f, microseconds\n"
      "- Average time ForceKernel : %f, microseconds\n"
      "- Average time AccelerationKernel : %f, microseconds\n"
      "- Average time VelocityKernel : %f, microseconds\n"
      "- Average time PositionKernel : %f, microseconds\n\n"

      "- Time spent per frame : %f, microseconds\n"
      "- Time spent Update Asteroids : %f, microseconds\n"
      "- Time spent ForceKernel : %f, microseconds\n"
      "- Time spent AccelerationKernel : %f, microseconds\n"
      "- Time spent VelocityKernel : %f, microseconds\n"
      "- Time spent PositionKernel : %f, microseconds\n"

      , date->tm_mday, date->tm_mon + 1, date->tm_year + 1900, date->tm_hour, date->tm_min, date->tm_sec,
      oNames[n], sNames[s],
      solarSystem_->currentAsteroidsNumber,
      demoParams.numberOfRepetitons,
      timeOutput.totalTime,
      timeOutput.perFrameTime / demoParams.numberOfRepetitons,
      timeOutput.updateAsteroidTime / demoParams.numberOfRepetitons,
      timeOutput.forceKernelTime / demoParams.numberOfRepetitons,
      timeOutput.velocityKernelTime / demoParams.numberOfRepetitons,
      timeOutput.accelerationKernelTime / demoParams.numberOfRepetitons,
      timeOutput.positionKernelTime / demoParams.numberOfRepetitons,

      timeOutput.perFrameTime ,
      timeOutput.updateAsteroidTime ,
      timeOutput.forceKernelTime ,
      timeOutput.velocityKernelTime ,
      timeOutput.accelerationKernelTime,
      timeOutput.positionKernelTime 
      );

    fprintf(fp, "%s\n", outPutBuffer);
    fclose(fp);

    if (closeFinishSim) { endSim = true; }

  }
}




void Game::draw() {
  solarSystem_->drawSolarSystem(window_,asteroidBelt, showQuadTree, showQuery);
  window_.draw(*solarSystemTexture_);
}



void Game::AsteroidsMoreInfo() {

  float xp = 0.0f, yp = 0.0f, zp = 0.0f,  mp = 0.0f, rp = 0.0f, vxp = 0.0f, vyp= 0.0f, vzp = 0.0f;
  float sx = 0.0f;
  float sy = 0.0f;

  ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
  if (ImGui::Begin("Asteroids Info", &showMoreInfo, ImGuiWindowFlags_MenuBar))
  {
    if (ImGui::BeginMenuBar())
    {
      if (ImGui::BeginMenu("File"))
      {
        if (ImGui::MenuItem("Close")) showMoreInfo = false;
        ImGui::EndMenu();
      }
      ImGui::EndMenuBar();
    }

    // Left
    static int selected = 0;
    {
      ImGui::BeginChild("left pane", ImVec2(150, 0), true);
      for (int i = 0; i < solarSystem_->currentAsteroidsNumber; i++)
      {
        // FIXME: Good candidate to use ImGuiSelectableFlags_SelectOnNav
        char label[128];
        sprintf(label, "ASTEROID %d", i);
        if (ImGui::Selectable(label, selected == i)) {
          selected = i;


        }
          
      }
      ImGui::EndChild();
    }
    ImGui::SameLine();


    xp = solarSystem_->asteroidData.x[selected];
    yp = solarSystem_->asteroidData.y[selected];
    zp = solarSystem_->asteroidData.z[selected];

    mp = solarSystem_->asteroidData.m[selected];
    rp = solarSystem_->asteroidData.r[selected];

    vxp = solarSystem_->asteroidData.vx[selected];
    vyp = solarSystem_->asteroidData.vy[selected];
    vzp = solarSystem_->asteroidData.vz[selected];


    // Right
    {
      ImGui::BeginGroup();
      ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
      ImGui::Text("ASTEROID: %d", selected);
  
      if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
      {
        ImGui::Text("Position PTR (%f, %f, %f) M ", xp,yp,zp);
        ImGui::Text("Screen Position (%f, %f, 0.0)",sx,sy);
        ImGui::Text("Velocity PTR (%f, %f, %f) M/s^2", vxp,vyp,vzp);
        ImGui::Text("Mass PTR: %f KG", mp);
        ImGui::Text("Radii PTR: %f M", rp);

        
        ImGui::EndTabBar();
      }
      ImGui::EndChild();
 
      ImGui::EndGroup();
    }
  }
  ImGui::End();

}


void Game::QueryMoreInfo() {

  float xp = 0.0f, yp = 0.0f, zp = 0.0f, mp = 0.0f, rp = 0.0f, vxp = 0.0f, vyp = 0.0f, vzp = 0.0f;
  float sx = 0.0f;
  float sy = 0.0f;

  ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
  if (ImGui::Begin("Query Info", &showQueryMoreInfo, ImGuiWindowFlags_MenuBar))
  {
    if (ImGui::BeginMenuBar())
    {
      if (ImGui::BeginMenu("File"))
      {
        if (ImGui::MenuItem("Close")) showQueryMoreInfo = false;
        ImGui::EndMenu();
      }
      ImGui::EndMenuBar();
    }

    // Left
    {
      ImGui::BeginChild("left pane", ImVec2(150, 0), true);
      for (int i = 0; i < solarSystem_->currentAsteroidsNumber; i++)
      {
        // FIXME: Good candidate to use ImGuiSelectableFlags_SelectOnNav
        char label[128];
        sprintf(label, "ASTEROID %d", i);
        if (ImGui::Selectable(label, solarSystem_->indexQueryToShow == i)) {
          solarSystem_->indexQueryToShow = i;


        }

      }
      ImGui::EndChild();
    }
    ImGui::SameLine();


   

    // Right
    {
      ImGui::BeginGroup();
      ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
      ImGui::Text("ASTEROID: %d", solarSystem_->indexQueryToShow);
 
      if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
      {

     
        for (int i = 0; i < solarSystem_->indexOutputAsteroids[solarSystem_->indexQueryToShow]; ++i)
        {

          int id = solarSystem_->outputAsteroids[solarSystem_->indexQueryToShow * kMaxAsteroidPerAsteroid +i];
          xp = solarSystem_->asteroidData.x[id];
          yp = solarSystem_->asteroidData.y[id];
          zp = solarSystem_->asteroidData.z[id];

          mp = solarSystem_->asteroidData.m[id];
          rp = solarSystem_->asteroidData.r[id];

          vxp = solarSystem_->asteroidData.vx[id];
          vyp = solarSystem_->asteroidData.vy[id];
          vzp = solarSystem_->asteroidData.vz[id];




          ImGui::Text("Asteroid %d", id);
          ImGui::Text("Position (%f, %f, %f) M ", xp, yp, zp);
          ImGui::Text("Screen Position (%f, %f, 0.0)", sx, sy);
          ImGui::Text("Velocity PTR (%f, %f, %f) M/s^2", vxp, vyp, vzp);
          ImGui::Text("Mass PTR: %f KG", mp);
          ImGui::Text("Radii PTR: %f M", rp);

        }


        ImGui::EndTabBar();
      }
      ImGui::EndChild();

      ImGui::EndGroup();
    }
  }
  ImGui::End();

}

void Game::asteoridImGui() {

 
  if (showMoreInfo) { AsteroidsMoreInfo(); }
  if (showQueryMoreInfo) { QueryMoreInfo(); }

  if (Ready == demoParams.state) {
    ImGui::Begin("AsteroidsController");



    ImGui::Checkbox("Show Ateroids belt", &asteroidBelt);
    ImGui::Checkbox("Show Quad Tree", &showQuadTree);
    ImGui::SliderFloat("TimeStep", &solarSystem_->timeStep_, 0.0001f, 10.0f);


    ImGui::InputInt("Number of repetitons demo", &demoParams.numberOfRepetitons, 10, 10);
    if (demoParams.numberOfRepetitons < 0) { demoParams.numberOfRepetitons = 1; }
    if (demoParams.numberOfRepetitons > kMaxRepetition) { demoParams.numberOfRepetitons = kMaxRepetition; }

    ImGui::InputInt("Number of Asteroids", &solarSystem_->currentAsteroidsNumber, kMinAsteroids, kMaxAsteroids);
    if (solarSystem_->currentAsteroidsNumber < kMinAsteroids) { solarSystem_->currentAsteroidsNumber = kMinAsteroids; }
    if (solarSystem_->currentAsteroidsNumber > kMaxAsteroids) { solarSystem_->currentAsteroidsNumber = kMaxAsteroids; }



    /*ImGui::PushID(2);
    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor(0.5f, 0.5f, 0.00f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor(0.7f, 0.7f, 0.4f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor(0.5f, 0.5f, 0.2f));
    if (ImGui::Button("More Info")) {
      printf("MORE INFO\n");
      showMoreInfo = !showMoreInfo;
    }
    ImGui::SameLine();
    if (ImGui::Button("Query Info")) {
      printf("MORE INFO\n");
      showQueryMoreInfo = !showQueryMoreInfo;
    }
    ImGui::PopStyleColor(3);
    ImGui::PopID();*/

    ImGui::PushID(1);
    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor(0.0f, 0.6f, 0.00f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor(0.8f, 0.0f, 0.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor(0.8f, 0.6f, 0.0f));
    if (ImGui::Button("Run Demo")) {
      demoParams.state = Running;
      solarSystem_->chronoSS.start = std::chrono::high_resolution_clock::now();
    }
    ImGui::SameLine();
    if (ImGui::Button("Reset")) {
      solarSystem_->initAsteroidBelt();
#ifdef CLSIM
      solarSystem_->resetCL();
#endif // CLSIM

    }

    ImGui::PopStyleColor(3);
    ImGui::PopID();



    ImGui::End();

  }
  
  
}

void Game::end() {

  ImGui::SaveIniSettingsToDisk("../../imgui.ini");
  ImGui::SFML::Shutdown();
}


void Game::mainLoop()
{
  sf::Clock secClock, mainClock, iaClock, inputClock;
  sf::Clock worldClock, drawClock, imguiClock;

  init(1920,1080);
 

  // Start the game loop
  while (window_.isOpen() && !endSim) {

    solarSystem_->chronoSS.startFrame[demoParams.currentRepetitions] = std::chrono::high_resolution_clock::now();
    mainClock.restart();

    // Input Update
   
      input();
    

    // IA Update
    if (Running == demoParams.state) {


      // World Update
      
        worldClock.restart();
        update(0.0f);
      
    }

    ImGui::SFML::Update(window_, imguiClock.restart());
    asteoridImGui();

    // Finally, display the rendered frame on screen
    //Draw
    window_.clear(sf::Color(10, 10, 10, 255));
    draw();

    ImGui::SFML::Render(window_);
    window_.display();

   



    solarSystem_->chronoSS.endFrame[demoParams.currentRepetitions-1] = std::chrono::high_resolution_clock::now();




  }

  end();
}
