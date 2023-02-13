@REM Compilación y Enlace con librería gráfica.
@cls
@echo ---------------------------------------------------
@echo  Proceso por lotes iniciado.
@echo ---------------------------------------------------
@echo off

gcc %1 -Wall -I ..\include\SDL2 -I ..\include -L ..\deps\SDL2\lib -lmingw32 -lSDL2main -lSDL2 -o %2
@REM opengl32.lib user32.lib gdi32.lib shell32.lib Ws2_32.lib
@echo ---------------------------------------------------
@echo  Proceso por lotes finalizado.
@echo ---------------------------------------------------