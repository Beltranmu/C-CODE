@REM Compilación y Enlace con librería gráfica.
@cls
@echo ---------------------------------------------------
@echo  Proceso por lotes iniciado.
@echo ---------------------------------------------------
@echo off

cl /nologo /Zi /EHs /MDd /W1 -I ..\include -I ..\deps\SDL2\include /c ..\src\*.cc
cl /nologo /Zi /EHs /MDd /W4 /Fe:..\bin\main.exe ..\build\*.obj /link /SUBSYSTEM:CONSOLE ..\deps\SDL2\lib\x86\SDL2.lib ..\deps\SDL2\lib\x86\SDL2main.lib ..\deps\SDL2\lib\x86\SDL2_image.lib ..\deps\SDL2\lib\x86\SDL2_ttf.lib opengl32.lib user32.lib gdi32.lib shell32.lib Ws2_32.lib

@REM opengl32.lib user32.lib gdi32.lib shell32.lib Ws2_32.lib
@echo ---------------------------------------------------
@echo  Proceso por lotes finalizado.
@echo ---------------------------------------------------