workspace ("Awakened-Engine")
  
  location ("./build/") 
  language ("C++") 
  platforms { "Win64" }	
  defines { "_GLFW_WIN32" , "PX_SCHED_IMPLEMENTATION"}
  
  configurations ({ "Debug", "Release" })
  flags{"MultiProcessorCompile"}

  includedirs ({ "./deps/GLFW/include", "./deps/GLFW/src" , 
                 "./deps/loguru/include" ,
                 "./deps/glad/include", "./deps/glad/src" ,
                 "./deps/px","./deps/minitrace",
                 "./deps/FMOD/core/inc", "./deps/tools",
                 "./deps/glm", "./deps/tinyobj",
                 "./deps/img", "./deps/lua/src",
                 "./deps/imgui/include",
                 "./include/" })
                 
  libdirs { "./deps/FMOD/core/lib/x64" }
                 
  files{ "./deps/GLFW/src/**.c", "./deps/loguru/src/**.c*",
         "./deps/glad/src/**.c*", "./deps/FMOD/core/src/*.c*",  
         "./deps/minitrace/*.c*", "./deps/tools/*.c*",
         "./deps/lua/src/*.c*","./deps/imgui/src/*.c*",
         "./deps/tinyobj/tiny_obj_loader.cc"}
  
  removefiles{ "./deps/GLFW/src/cocoa_time.c",
               "./deps/GLFW/src/glx_context.c",
               "./deps/GLFW/src/linux**.c",
               "./deps/GLFW/src/platform.c",
               "./deps/GLFW/src/null**.c",
               "./deps/GLFW/src/posix**.c",
               "./deps/GLFW/src/wl**.c",
               "./deps/GLFW/src/x11**.c",
               "./deps/lua/src/lua.c",
               "./deps/lua/src/luac.c",}

  filter "configurations:Debug"
    defines { "DEBUG", "MTR_ENABLED" }
    symbols "On"

  filter "configurations:Release"
    defines { "NDEBUG" }
    optimize "On"
  
  --* Awakened Engine *--
  project "Awakened Engine"
    location "build/awakened"
    kind "StaticLib" 
    
    files { "./src/**.c*", "./src/internals.h", "./include/**.h*", "./data/lua/*.lua" }
    
    links { "OpenGL32" }
      
  --. Hello World Project .--    
  project "HelloWorld"
    filename("Example")
     
    location "build/test"
    kind "ConsoleApp"
    targetdir ("./bin/HelloWorld")
    files { "./example/helloexample/*.cc" }
   
    links { "Awakened Engine" }
        
    --. Hello World Project .--    
  project "Threads test"
    filename("Threads_test")
     
    location "build/Threads_test"
    kind "ConsoleApp"
    targetdir ("./bin/Thread")
    debugdir "./bin/Thread"
    includedirs("./deps/enkiTS")
    files { "./example/thread_test/*.cc",
            "./deps/enkiTS/TaskScheduler.cpp"}
   
    links { "Awakened Engine" }
    
    --. Hello Sound Project .--    
  project "Sound Test"
    filename("Sound_Test")
     
    location "build/Sound_Test"
    kind "ConsoleApp"
    targetdir ("./bin/HelloSound")
    debugdir "./bin/HelloSound"
    files { "./example/sound_test/*.cc" }
   
    links { "Awakened Engine", "fmod_vc.lib", "fmodL_vc.lib" }


     --. Hello Test Project .--    
  project "HelloTest"
    filename("TestExample")
     
    location "build/test"
    kind "ConsoleApp"
    targetdir ("./bin/HelloTest")
    files { "./example/helloTest/*.cc" }
   
    links { "Awakened Engine" }