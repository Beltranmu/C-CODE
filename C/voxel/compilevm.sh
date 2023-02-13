
# Si compilais en linux x86/x64, teneis que quitar "-mfpu=vfp" que es la unidad de coma flotante de ARM
gcc -c -g -Wall -O2 -o obj/dot_canvas.o dot_canvas.c
gcc -c -g -Wall -O2 -o obj/chrono.o chrono.c

cd obj

gcc -o ../dot_canvas.elf dot_canvas.o   chrono.o  -lm -lSDL 
