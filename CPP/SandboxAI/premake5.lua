workspace ("Artificial Intellgence")
  
  location ("./build/") 
  language ("C++") 
  platforms { "Win32", "Win64" }	
  configurations ({ "Debug", "Release" })
  defines {"SFML_STATIC" , "_CRT_SECURE_NO_WARNINGS"}
  includedirs { "./deps/x86/SFML/include", "./deps/imgui", "./deps/imgui-SFML"} 
  includedirs { "./include"}
  links {"opengl32", "winmm", "freetype" }
  --defines { "_GLFW_WIN32" }
 
  --* x86 *--
  filter { "platforms:Win32"}
    system "Windows"
    architecture "x86"
    libdirs{ "./deps/x86/SFML/lib" }
    
    filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"
      links { "sfml-graphics-s-d", "sfml-window-s-d", "sfml-system-s-d" }
      
    filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"
      links { "sfml-graphics-s", "sfml-window-s", "sfml-system-s" }
      
  --. x64 .--
  filter { "platforms:Win64"}  
    system "Windows"
    architecture "x64"    
    libdirs{ "./deps/x64/SFML/lib" }
    
    filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"
      links { "sfml-graphics-s-d", "sfml-window-s-d", "sfml-system-s-d" }
      
    filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"
      links { "sfml-graphics-s", "sfml-window-s", "sfml-system-s" }
    
    
  --* AI Workspace *--
  
  project "AI Workspce"
    location ("./build/ai-workspace")
    targetdir ("./bin/debug")
    debugdir "bin/debug"
    kind "ConsoleApp"
    files {"./src/**.c*", "./include/**.h*",
           "./deps/x86/SFML/include/SFML/**.h*",
           "./deps/imgui/**.c*", "./deps/imgui-SFML/**.c*",
           "./deps/imgui/**.h*", "./deps/imgui-SFML/**.h*"}
           