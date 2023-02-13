
  require('premake5-cuda')

workspace ("Asteroids")
  
  location ("./build/") 
  language ("C++") 
  platforms { "Win32", "Win64" }	
  configurations ({ "DebugCUDA", "ReleaseCUDA", "DebugCL", "ReleaseCL" })
  defines {"SFML_STATIC" , "_CRT_SECURE_NO_WARNINGS"}
  buildcustomizations "BuildCustomizations/CUDA 11.8"
  
  includedirs { "./deps/x86/SFML/include", "./deps/imgui", "./deps/imgui-SFML"} 
  includedirs { "./include"}
  links {"opengl32", "winmm", "freetype", "OpenCL" }
  
  --* x86 *--
  filter { "platforms:Win32"}
  system "Windows"
  architecture "x86"
  libdirs{ "./deps/x86/SFML/lib" }
  defines {"s32"}

  filter "configurations:DebugCUDA"
  defines { "DEBUG", "CUDASIM" }
  symbols "On"
  links { "sfml-graphics-s-d", "sfml-window-s-d", "sfml-system-s-d" }
  
  filter "configurations:ReleaseCUDA"
  defines { "NDEBUG","CUDASIM"}
  optimize "On"
  links { "sfml-graphics-s", "sfml-window-s", "sfml-system-s" }


  filter "configurations:DebugCL"
  defines { "DEBUG", "CLSIM" }
  symbols "On"
  links { "sfml-graphics-s-d", "sfml-window-s-d", "sfml-system-s-d" }
  
  filter "configurations:ReleaseCL"
  defines { "NDEBUG","CLSIM"}
  optimize "On"

  links { "sfml-graphics-s", "sfml-window-s", "sfml-system-s" }
  
  --. x64 .--
  filter { "platforms:Win64"}  
    system "Windows"
    architecture "x64"    
    libdirs{ "./deps/x64/SFML/lib" }
    defines {"s64"}

    filter "configurations:DebugCUDA"
    defines { "DEBUG", "CUDASIM" }
      symbols "On"
      links { "sfml-graphics-s-d", "sfml-window-s-d", "sfml-system-s-d" }
      
      filter "configurations:ReleaseCUDA"
      defines { "NDEBUG", "CUDASIM" }
      optimize "On"
      links { "sfml-graphics-s", "sfml-window-s", "sfml-system-s" }


      filter "configurations:DebugCL"
      defines { "DEBUG", "CLSIM" }
      symbols "On"
      links { "sfml-graphics-s-d", "sfml-window-s-d", "sfml-system-s-d" }
      
      filter "configurations:ReleaseCL"
      defines { "NDEBUG","CLSIM"}
      optimize "On"
      links { "sfml-graphics-s", "sfml-window-s", "sfml-system-s" }

      
      
  --* Asteroids Workspace *--
  
  project "Asteroids Workspce"
  location ("./build/asteroids")
  --targetdir ("./bin/debug")
  debugdir "bin/debug"
  kind "ConsoleApp"


  files {"./src/**.c*", "./include/**.h*",
  "./src/**.cl",
  "./srcCU/**.cu", "./include/**.cuh",
  "./deps/x64/SFML/include/SFML/**.h*",
           "./deps/imgui/**.c*", "./deps/imgui-SFML/**.c*",
           "./deps/imgui/**.h*", "./deps/imgui-SFML/**.h*"}


           -- CUDA specific properties
cudaFiles {"./srcCU/**.cu"} -- files NVCC compiles
cudaMaxRegCount "32"

filter "configurations:ReleaseCL"
targetdir("./bin/releaseCL")  
debugdir "bin/releaseCL"
filter "configurations:ReleaseCUDA"
targetdir("./bin/releaseCUDA")  
debugdir "bin/releaseCUDA"
filter "configurations:DebugCL"
targetdir("./bin/debugCL")  
debugdir "bin/debugCL"
filter "configurations:DebugCUDA"
targetdir("./bin/debugCUDA")  
debugdir "bin/debugCUDA"
           