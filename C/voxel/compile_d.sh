
# Si compilais en linux x86/x64, teneis que quitar "-mfpu=vfp" que es la unidad de coma flotante de ARM
gcc -c -g -Wall -mfpu=vfp -O2 -o obj/voxel.o voxel.c
gcc -c -g -Wall -mfpu=vfp -O2 -o obj/innervoxel.o innervoxel.c
gcc -c -g -Wall -mfpu=vfp -O2 -o obj/chrono.o chrono.c

cd obj

gcc -o ../voxel.elf voxel.o  chrono.o  -lm -lSDL 
