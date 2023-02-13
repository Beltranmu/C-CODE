@ECHO OFF

del *.obj *.pdb *.ilk *.exe

clang-cl /nologo /Zi /EHsc /MDd /W4 -m32 -I..\include -I ..\deps\sdl2\include -I ..\deps\imgui\ -I ..\deps\imgui_sdl /c ..\deps\imgui\*.cpp
clang-cl /nologo /Zi /EHsc /MDd /W4 -m32 -I..\include -I ..\deps\sdl2\include -I ..\deps\imgui\ -I ..\deps\imgui_sdl /c ..\deps\imgui_sdl\imgui_sdl.cpp
clang-cl /nologo /Zi /EHsc /MDd /W4 -m32 -I..\include -I..\deps\sdl2\include -I..\deps\imgui\ -I..\deps\imgui_sdl /c ..\src\*.cc
clang-cl /nologo /Zi /EHsc /MDd /W4 -m32 -I..\include -I..\deps\sdl2\include -I..\deps\imgui -I..\deps\imgui_sdl /c ..\src\*.cc
clang-cl /nologo /Zi /MDd /W4 -m32 /Fetest.exe *.obj /link /SUBSYSTEM:CONSOLE ..\deps\sdl2\lib\x86\SDL2.lib ..\deps\sdl2\lib\x86\SDL2main.lib ..\deps\sdl2\lib\x86\SDL2_image.lib ..\deps\SDL2\lib\x86\SDL2_ttf.lib opengl32.lib shell32.lib user32.lib gdi32.lib

