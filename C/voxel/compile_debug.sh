gcc -c -g -Wall -mfpu=vfp -O0 -o obj/voxel.o voxel.c
gcc -c -g -Wall -mfpu=vfp -O0 -o obj/chrono.o chrono.c
gcc -c -g -o obj/armvoxel.o armvoxel.s

cd obj

gcc -o ../voxel.elf voxel.o  chrono.o armvoxel.o -lm -lSDL 
