@ECHO OFF

del *.obj *.pdb *.ilk *.exe
cls

cl /nologo /Zi /GR- /EHs /MDd /c ../src/ADT/adt_memory_node.c -I ../include -I ../include/ADT -I ../deps/abgs_memory_manager/include
cl /nologo /Zi /GR- /EHs /MDd /c ../src/ADT/adt_vector.c -I ../include -I ../include/ADT -I ../deps/abgs_memory_manager/include


cl /nologo /Zi /EHs /MDd /W4 -I ..\include -I ..\deps\sdl2\include -I ..\deps\imgui\ -I ..\deps\imgui_sdl /c ..\deps\imgui\*.cpp
cl /nologo /Zi /EHs /MDd /W4 -I ..\include -I ..\deps\sdl2\include -I ..\deps\imgui\ -I ..\deps\imgui_sdl /c ..\deps\imgui_sdl\imgui_sdl.cpp
cl /nologo /Zi /EHs /MDd /W4 -I ..\include -I ..\deps\sdl2\include -I ..\deps\imgui\ -I ..\deps\imgui_sdl -I ..\include\ADT -I ..\deps\abgs_memory_manager\include -I..\deps\abgs_memory_manager\include\ABGS_MemoryManager /c ..\src\*.cc
cl /nologo /Zi /EHs /MDd /W4 /Fe: ..\bin\main.exe *.obj /link /SUBSYSTEM:CONSOLE ..\deps\sdl2\lib\x86\SDL2.lib ..\deps\sdl2\lib\x86\SDL2main.lib ..\deps\sdl2\lib\x86\SDL2_image.lib ..\deps\sdl2\lib\x86\SDL2_ttf.lib ..\deps\sql\lib\sqlite.lib ..\deps\abgs_memory_manager\lib\ABGS_MemoryManager\ABGS_MemoryManager_w32d.lib opengl32.lib shell32.lib user32.lib gdi32.lib