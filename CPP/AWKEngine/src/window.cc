#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "internals.h"
#include <gtx/transform.hpp >

#include <gtc/type_ptr.hpp>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "ecs.h"

namespace AWK {

  Window::Window() { _data = new Data(); }

  Window::~Window() { delete _data; }

  bool Window::initWindow(int32_t w, int32_t h, const char* w_name) {

    if (!glfwInit()) {
      printf("This message will be deleted in May... the force be with us");
      exit(EXIT_FAILURE);
      return false;
    }
    // Tell GLFW the OPENGL Version
    const char* glsl_version = "#version 460";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only



    int32_t count = 1;
    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    GLFWmonitor** monitors = glfwGetMonitors(&count);
 
    //glfwGetMonitors(&monitors);

    const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);

    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
    glfwWindowHint(GLFW_MAXIMIZED, true);
    //GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "My Title", monitor, NULL);


    //_data->_width = mode->width;
    //_data->_height = mode->height;

    _data->_width = w;
    _data->_height = h;

    _data->_mainWindow = glfwCreateWindow(_data->_width, _data->_height, w_name, NULL, NULL);
    //glfwSetWindowMonitor(_data->_mainWindow, primaryMonitor, 0, 0, mode->width, mode->height, mode->refreshRate);


    if (nullptr == _data->_mainWindow) {
      printf("This message will be deleted in May... the force be with us");
      glfwTerminate();
      exit(EXIT_FAILURE);
      return false;
    }

    glfwSetKeyCallback(_data->_mainWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
      if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
      }
    });

    glfwSetMouseButtonCallback(_data->_mainWindow,
      [](auto* window, int button, int action, int mods) {
      auto& io = ImGui::GetIO();
      const int idx = button == GLFW_MOUSE_BUTTON_LEFT ? 0 : button == GLFW_MOUSE_BUTTON_RIGHT ? 2 : 1;
      io.MouseDown[idx] = action == GLFW_PRESS;
    });

    glfwSetCursorPosCallback(_data->_mainWindow, 
      [](auto* window, double x, double y) {
        ImGui::GetIO().MousePos = ImVec2((float)x, (float)y);
      
      });

    glfwSetMouseButtonCallback(_data->_mainWindow,
      [](auto* window, int button, int action, int mods) {
        auto& io = ImGui::GetIO();
        const int idx = button == GLFW_MOUSE_BUTTON_LEFT ? 0 : button == GLFW_MOUSE_BUTTON_RIGHT ? 2 : 1;
        io.MouseDown[idx] = action == GLFW_PRESS;
      });
      
    glfwMakeContextCurrent(_data->_mainWindow);
    glfwSwapInterval(1);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

    io.IniFilename = "../../data/imgui/imgui.ini";

    //io.BackendFlags |= ImGuiBackendFlags_PlatformHasViewports;
    //io.BackendFlags |= ImGuiBackendFlags_RendererHasViewports;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(_data->_mainWindow, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    return true;
  }

  bool Window::isOpened(){
    //Update W and H of the window
    int width, height;
    glfwGetFramebufferSize(
      _data->_mainWindow, &width, &height);
    glViewport(0, 0, width, height);
    return !glfwWindowShouldClose(_data->_mainWindow);
  }

  void Window::swap(){
      
    glfwSwapBuffers(_data->_mainWindow);

  }

  void Window::closeWindow(){
    
    ImGui::DestroyContext();
    glfwDestroyWindow(_data->_mainWindow);
    glfwTerminate(); 
  }

  void Window::moveCamera(Engine* engine){

    int button = glfwGetMouseButton(_data->_mainWindow, GLFW_MOUSE_BUTTON_RIGHT);
    bool canMoveCamera = button == GLFW_PRESS;
    int state = glfwGetKey(_data->_mainWindow, GLFW_KEY_W);

    auto&& ECSTransaction = engine->_entityData->transaction();

    TRSComponent* trsComp = ECSTransaction.getTRSComponent(engine->_entityData->camera);
    FlyCameraMovementComponent* flyComp = ECSTransaction.getFlyCameraComponent(engine->_entityData->camera);
    float camSpeed = flyComp->lSpeed;
    if (state == GLFW_PRESS && canMoveCamera){
      trsComp->translation += flyComp->forward * camSpeed;
    }

    state = glfwGetKey(_data->_mainWindow, GLFW_KEY_S);
    if (state == GLFW_PRESS && canMoveCamera){
      trsComp->translation -= flyComp->forward * camSpeed;
    }

    state = glfwGetKey(_data->_mainWindow, GLFW_KEY_A);
    if (state == GLFW_PRESS && canMoveCamera) {
      trsComp->translation += flyComp->right * camSpeed;
    }

    state = glfwGetKey(_data->_mainWindow, GLFW_KEY_D);
    if (state == GLFW_PRESS && canMoveCamera) {
      trsComp->translation -= flyComp->right * camSpeed;
    }

    state = glfwGetKey(_data->_mainWindow, GLFW_KEY_E);
    if (state == GLFW_PRESS && canMoveCamera) {
      trsComp->translation += flyComp->up * camSpeed;
    }

    state = glfwGetKey(_data->_mainWindow, GLFW_KEY_Q);
    if (state == GLFW_PRESS && canMoveCamera) {
      trsComp->translation -= flyComp->up * camSpeed;
    }

    if(canMoveCamera){
      double currentXpos, currentYpos;
      glfwGetCursorPos(_data->_mainWindow, &currentXpos, &currentYpos);
      
      button = glfwGetMouseButton(_data->_mainWindow, GLFW_MOUSE_BUTTON_LEFT);
      bool canMoveCamera1 = button == GLFW_PRESS;

      if(currentXpos < mouseX){
        flyComp->yaw += flyComp->rSpeed;
      }
      else if (currentXpos > mouseX) {
        flyComp->yaw -= flyComp->rSpeed;
      }
      if (currentYpos < mouseY && flyComp->pitch < 1.54f) {
        flyComp->pitch += flyComp->rSpeed;
      }
      else if (currentYpos > mouseY && flyComp->pitch > -1.54f) {
        flyComp->pitch -= flyComp->rSpeed;
      }

      mouseX = currentXpos;
      mouseY = currentYpos;
    }
    

  }
  
  void Window::drawCameraImgui(Engine* engine){

    ImGui::Begin("Camera Data");
    auto&& ecsTransaction = engine->_entityData->transaction();
    TRSComponent* trs = ecsTransaction.getTRSComponent(engine->_entityData->camera);
    FlyCameraMovementComponent* flyComp = ecsTransaction.getFlyCameraComponent(engine->_entityData->camera);

    ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.00f), "Position (%f, %f, %f)",trs->translation.x, trs->translation.y, trs->translation.z );
    ImGui::TextColored(ImVec4(0.0f, 0.0f, 1.0f, 1.00f), "Forward (%f, %f, %f)", flyComp->forward.x, flyComp->forward.y, flyComp->forward.z );
    ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.00f), "Right (%f, %f, %f)", flyComp->right.x, flyComp->right.y, flyComp->right.z );
    ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.00f), "Up (%f, %f, %f)", flyComp->up.x, flyComp->up.y, flyComp->up.z );

    ImGui::InputFloat("Camera Lineal speed", &flyComp->lSpeed, 0.01f, 0.1f, "%.4f");
    ImGui::SliderFloat("Camera Lineal speed", &flyComp->lSpeed, 0.0f, 8.0f );


    ImGui::InputFloat("Camera Angular speed", &flyComp->rSpeed, 0.01f, 0.1f, "%.4f");
    ImGui::SliderFloat("Camera Angular speed", &flyComp->rSpeed, 0.0f, 1.0f);


    ImGui::End();

  }

  void Window::drawImgui(Engine *engine)
  {
    glfwPollEvents();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::DockSpaceOverViewport();
    ImGui::ShowDemoWindow();
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;

    bool p ;
    int shadowMode = (int)engine->_GPUState->frameData.shadows.data[2];
    
    drawCameraImgui(engine);

    // --- IMGUI BEGIN ---
    ImGui::Begin("Scene",&p,window_flags);
  
      if (ImGui::BeginMenuBar())
      {
        if (ImGui::BeginMenu("Options"))
        {
          ImGui::MenuItem("Simple layout", NULL, &showHierarchy);
          ImGui::ColorEdit4("ClearColor", engine->bgColor);
          ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
      }
      if (showHierarchy) {
        ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
        if (ImGui::Begin("Example: Simple layout", &showHierarchy, ImGuiWindowFlags_MenuBar))
        {
          if (ImGui::BeginMenuBar())
          {
            if (ImGui::BeginMenu("File"))
            {
              if (ImGui::MenuItem("Close")) showHierarchy = false;
              ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
          }

          // Left
          static int selected = 0;
          {
            ImGui::BeginChild("left pane", ImVec2(150, 0), true);
            for (int i = 0; i < engine->_entityData->entities.size(); i++)
            {
              // FIXME: Good candidate to use ImGuiSelectableFlags_SelectOnNav
              char label[128];
              sprintf(label, "MyObject %d", i);
              if (ImGui::Selectable(label, selected == i))
                selected = i;
            }
            ImGui::EndChild();
          }
          ImGui::SameLine();

          // Right
          {
            ImGui::BeginGroup();
            ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
            ImGui::Text("MyObject: %d", selected);
            ImGui::Separator();
            if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
            {
              auto&& transaction = engine->_entityData->transaction();
              TRSComponent* trs = transaction.getTRSComponent(engine->_entityData->entities[selected]->id);


              if (ImGui::BeginTabItem("Details"))
              {
                ImGui::Text("ID: %d", engine->_entityData->entities[selected]->id);
                ImGui::Text("Position: X: %f, Y: %f, Z: %f", trs->translation.x, trs->translation.y, trs->translation.z);
                ImGui::Text("Rotation: X: %f, Y: %f, Z: %f", trs->rotation.x, trs->rotation.y, trs->rotation.z);
                ImGui::Text("Scale: X: %f, Y: %f, Z: %f", trs->scale.x, trs->scale.y, trs->scale.z);
                ImGui::EndTabItem();
              }
              if (ImGui::BeginTabItem("Information"))
              {
                ImGui::SliderFloat("Position X", &trs->translation.x, -100.0f, 100.0f);
                ImGui::SliderFloat("Position Y", &trs->translation.y, -100.0f, 100.0f);
                ImGui::SliderFloat("Position Z", &trs->translation.z, -100.0f, 100.0f);
                ImGui::EndTabItem();
              }

           
              ImGui::EndTabBar();
            }
            ImGui::EndChild();
         
            ImGui::EndGroup();
          }
        }
        ImGui::End();

      }

      const int MAX_TYPE = 3;

      const char* elems_names[MAX_TYPE] = { "Hard Shadow", "PCF", "PCSS" };
      const char* elem_name = (shadowMode >= 0 && shadowMode < MAX_TYPE) ? elems_names[shadowMode] : "Unknown";
      ImGui::SliderInt("Shadow Mode", &shadowMode, 0, MAX_TYPE - 1, elem_name);
      engine->_GPUState->frameData.shadows.data[2] = (float)shadowMode;

      ImGui::SliderFloat("Min Bias", &engine->_GPUState->minBias, 0.0000001f, 0.01f, "%.8f");
      ImGui::SliderFloat("Max Bias", &engine->_GPUState->maxBias, 0.0000001f, 0.1f, "%.8f");

    ImGui::End();



    ImGui::Begin("Water Info");

    ImGui::SliderFloat("WindDirection X", &engine->_GPUState->water.waterKData.data2.x, -1.0f, 1.0f );
    ImGui::SliderFloat("WindDirection Y", &engine->_GPUState->water.waterKData.data2.y, -1.0f, 1.0f );

    ImGui::SliderFloat("WindSpeed X", &engine->_GPUState->water.waterKData.data2.z, 0.0f, 100.0f);
    ImGui::SliderFloat("L ", &engine->_GPUState->water.waterKData.data1.z, 0.0f, 100000.0f);

    ImGui::End();

    ImGui::Begin("Water Textures");
    

      uint32_t  wt = engine->_GPUState->_textureArray[engine->_GPUState->water.textureTilde_k0].width;
      uint32_t  ht = engine->_GPUState->_textureArray[engine->_GPUState->water.textureTilde_k0].height;
      GLuint idTexture = engine->_GPUState->_textureArray[engine->_GPUState->water.textureTilde_k0].openGLid;
      ImGui::Text("size = %d x %d", wt, ht);
      ImGui::Image((void*)(intptr_t)idTexture, ImVec2(400, 400), ImVec2(0, 1), ImVec2(1, 0));
      idTexture = engine->_GPUState->_textureArray[engine->_GPUState->water.textureTilde_kminus0].openGLid;
      ImGui::Image((void*)(intptr_t)idTexture, ImVec2(400, 400), ImVec2(0, 1), ImVec2(1, 0));

      idTexture = engine->_GPUState->_textureArray[engine->_GPUState->water.heigmapDisplacementTexture].openGLid;
      ImGui::Image((void*)(intptr_t)idTexture, ImVec2(400, 400), ImVec2(0, 1), ImVec2(1, 0));
      idTexture = engine->_GPUState->_textureArray[engine->_GPUState->water.choppyXTexture].openGLid;
      ImGui::Image((void*)(intptr_t)idTexture, ImVec2(400, 400), ImVec2(0, 1), ImVec2(1, 0));
      idTexture = engine->_GPUState->_textureArray[engine->_GPUState->water.choppyZTexture].openGLid;
      ImGui::Image((void*)(intptr_t)idTexture, ImVec2(400, 400), ImVec2(0, 1), ImVec2(1, 0));

      idTexture = engine->_GPUState->_textureArray[engine->_GPUState->water.twiddleIndiceTexture].openGLid;
      ImGui::Text("size = %d x %d", engine->_GPUState->_textureArray[engine->_GPUState->water.twiddleIndiceTexture].width,
        engine->_GPUState->_textureArray[engine->_GPUState->water.twiddleIndiceTexture].height);
      ImGui::Image((void*)(intptr_t)idTexture, ImVec2(400, 400), ImVec2(0, 1), ImVec2(1, 0));

      idTexture = engine->_GPUState->_textureArray[engine->_GPUState->water.pingpong1].openGLid;
      ImGui::Image((void*)(intptr_t)idTexture, ImVec2(400, 400), ImVec2(0, 1), ImVec2(1, 0));

      idTexture = engine->_GPUState->_textureArray[engine->_GPUState->water.DyTexture].openGLid;
      ImGui::Image((void*)(intptr_t)idTexture, ImVec2(400, 400), ImVec2(0, 1), ImVec2(1, 0));


    ImGui::End();

    // !--- IMGUI END ---

    ImGui::Begin("OpenGL Texture Text");
    auto&& ECSTransaction = engine->_entityData->transaction();
    CameraComponent* camComp = ECSTransaction.getCameraComponent(engine->_entityData->camera);
    ImGui::Text("size = %d x %d", camComp->width, camComp->height);
    ImGui::Image((void*)(intptr_t)engine->_GPUState->_textureArray[engine->blitTextureID].openGLid, ImVec2(camComp->width, camComp->height) , ImVec2(0, 1), ImVec2(1, 0));
    ImGui::End();

    ImGui::Begin("OpenGL Light Texture Text");
    for(uint32_t i = 0; i< engine->_GPUState->currentActivatedShadows; ++i){
      camComp = ECSTransaction.getCameraComponent(engine->_entityData->lights[i]);
      ImGui::Text("size = %d x %d", camComp->width, camComp->height);
      ImGui::Image((void*)(intptr_t)engine->_GPUState->_textureArray[engine->_GPUState->_lightShadowTexture[i]].openGLid, ImVec2(400, 400), ImVec2(0, 1), ImVec2(1, 0));
     
    }

    

    ImGui::End();

    

    ImGui::Begin("OpenGL Light Properties");

    ImGui::SliderInt("Number of Shadows:", &(int)engine->_GPUState->currentActivatedShadows, 0, 4);

    for(uint32_t i = 0; i < engine->_GPUState->currentActivatedShadows; ++i){
      char posx[128], posy[128], posz[128], rotx[128], roty[128], rotz[128], width[128];
      sprintf(posx, "%d Position  X", i);
      sprintf(posy, "%d Position  Y", i);
      sprintf(posz, "%d Position  Z", i);
      sprintf(rotx, "%d Rotation  X", i);
      sprintf(roty, "%d Rotation  Y", i);
      sprintf(rotz, "%d Rotation  Z", i); 
      sprintf(width, "%d Light Width", i); 
      TRSComponent* trsComp = ECSTransaction.getTRSComponent(engine->_entityData->lights[i]);
      LightComponent* lightComp = ECSTransaction.getLightComponent(engine->_entityData->lights[i]);

    
      ImGui::SliderFloat(posx, &trsComp->translation.x, -200, 200);
      ImGui::SliderFloat(posy, &trsComp->translation.y, -200, 200);
      ImGui::SliderFloat(posz, &trsComp->translation.z, -200, 200);
      ImGui::SliderFloat(rotx, &trsComp->rotation.x, -3.14f, 3.14f);
      ImGui::SliderFloat(roty, &trsComp->rotation.y, -3.14f, 3.14f);
      ImGui::SliderFloat(rotz, &trsComp->rotation.z, -3.14f, 3.14f);
      ImGui::SliderFloat(width, &lightComp->lightWidth, 0.1f, 1.5f);

      CameraComponent* camComp = ECSTransaction.getCameraComponent(engine->_entityData->lights[i]);

      ImGui::Text("Near: %f, Far: %f",camComp->zNear , camComp->zFar);
    }
    ImGui::End();

    ImGui::Begin("Best Atmosphere");

    ImGui::SliderFloat("Sun Brightness", &engine->_GPUState->atmosphereData.brightness, 0, 100);
    ImGui::SliderFloat("Density", &engine->_GPUState->atmosphereData.density, 0, 5);
    ImGui::SliderFloat("Clarity", &engine->_GPUState->atmosphereData.clarity, 0, 5);
    ImGui::SliderFloat("Pollution", &engine->_GPUState->atmosphereData.pollution, 0, 5);
    ImGui::SliderFloat("Planet Scale", &engine->_GPUState->atmosphereData.planet_scale, 0, 10);
    ImGui::SliderFloat("Atmosphere Scale", &engine->_GPUState->atmosphereData.atmosphere_scale, 0, 10);
    ImGui::SliderFloat("Sun Disk Radius", &engine->_GPUState->atmosphereData.sun_disk_radius, 0, 1);
    ImGui::SliderFloat("Sun Disk Intensity", &engine->_GPUState->atmosphereData.sun_disk_intensity, 0, 1);

    ImGui::SliderFloat("Azimuth", &engine->_GPUState->atmosphereData.yaw, 0.0f, 6.3f);
    ImGui::SliderFloat("Zenith", &engine->_GPUState->atmosphereData.pitch, 0.2f, 3.0f);
    
    engine->_GPUState->scatteringData.u_sunPosition.z = cos(engine->_GPUState->atmosphereData.yaw) * cos(engine->_GPUState->atmosphereData.pitch);
    engine->_GPUState->scatteringData.u_sunPosition.y = sin(engine->_GPUState->atmosphereData.pitch);
    engine->_GPUState->scatteringData.u_sunPosition.x = sin(engine->_GPUState->atmosphereData.yaw) * cos(engine->_GPUState->atmosphereData.pitch);

    ImGui::SliderFloat("Light [Red]", &engine->_GPUState->scatteringData.sunWave[0], 0, 1);
    ImGui::SliderFloat("Light [Green]", &engine->_GPUState->scatteringData.sunWave[1], 0, 1);
    ImGui::SliderFloat("Light [Blue]", &engine->_GPUState->scatteringData.sunWave[2], 0, 1);
     
    ImGui::End();

    ImGui::Render();
    
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {

      GLFWwindow* backup_current_context = glfwGetCurrentContext();
      ImGui::UpdatePlatformWindows();
      ImGui::RenderPlatformWindowsDefault();
      glfwMakeContextCurrent(backup_current_context);

    }
    
  } 
};