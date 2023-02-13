

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

#include <SDL/SDL.h>

#include "chrono.h"

// ---------------------------------------------------------------------------

static int cpu_mhz = 0;
static int dump = 0;

static void ChronoShow ( char* name, int computations)
{
  float ms = ChronoWatchReset();
  float cycles = ms * (1000000.0f/1000.0f) * (float)cpu_mhz;
  float cyc_per_comp = cycles / (float)computations;
  if ((dump & 7) == 0)
    fprintf ( stdout, "%s: %f ms, %d cycles, %f cycles/loop\n", name, ms, (int)cycles, cyc_per_comp);
}


// // Limit framerate and return any remaining time to the OS 

static void FramerateLimit (int max_fps)
{   
  static unsigned int frame_time = 0;

  unsigned int t = GetMsTime();
  unsigned int elapsed = t - frame_time;
  // El tiempo sobrante se "regala" al sistema operativo con la funcion POSIX sleep
  const unsigned int limit = 1000 / max_fps;  // 1000 miliseconds / framerate minimo deseado 
  if ( elapsed < limit)
    usleep (( limit - elapsed) * 1000); // arg in microseconds
  frame_time = GetMsTime();
}

// ---------------------------------------------------------------------------


void CreateMesh(int w_map, int h_map, unsigned char* meshpoints, 
                 int w_smap, int h_smap, short* draw_points, 
                 float projection, int w, int* farpoints){
  //Changed according to w_smap  
  int far = 8192; 
  int x_origin = 2048;
  //int height = - 450;
  int scx = w >> 1;

  SDL_Surface *image = SDL_LoadBMP("resources/HM11.bmp"); 

  for(int i = 0; i < w_map * h_map; i++){

    *meshpoints = *(unsigned char*)(image->pixels+i);
    
    meshpoints++; 
  }
  for(int i = 0; i< w_smap; ++i){
       
    for(int j = 0; j < h_smap; ++j){
      /*int z = (far - i * 32);
      int x =it 32 ) - x_origin;

      int rotated_x = (int)((float)x * cos(0.7853f) - (float)z * sin(0.7853f));
      int rotated_z = (int)((float)x * sin(0.7853f) - (float)z * cos(0.7853f));

      float zp = projection/ (rotated_z);
      int zpi = (int)(zp * (float)(1<<8));
      *farpoints = zpi;
      farpoints++; 
      draw_points[0] = scx + (short)(rotated_x * zp);
      draw_points += 3;  */
      float zp = projection /((far - i * 32) );      
      int zpi = (int)(zp * (float)(1<<8));
      
      *farpoints = zpi;
      farpoints++; 
      draw_points[0] = scx + (short)(((j * 32 ) - x_origin) * zp);  
      //printf("draw: %d\n", draw_points[0]);
      draw_points += 3;  

    }
  }
}


#if 0
void HeighMapEffect(int map_size,
                    int dif_size_maps,                 
                    short* drawlist,
                    int scy,
                    unsigned char* meshp,
                    int *meshfarpoints ); 
#else

void HeighMapEffect(int map_size,
                    int dif_size_maps,                 
                    short* drawlist,
                    int scy,
                    unsigned char* meshp,
                    int *meshfarpoints ){ 
 
  for(int i = map_size; i > 0; i--){

    for(int j = map_size ; j > 0; j--){

      unsigned char y = *meshp;
      meshp++;
      int zpi = *meshfarpoints++;
      int yrelativepos  = ((y-1000) * zpi) >> 8; 
      int yp = scy - yrelativepos; 
      drawlist[1] = yp;
      drawlist[2] = y & 0xff;
      drawlist += 3; 
    } 
    meshp += dif_size_maps;
  }   
 
}

#endif



// ---------------------------------------------------------------------------

static void DisplayVertices (unsigned int* pixels, short* drawlist, int n_vertices, int stride, unsigned int* palette)
{
  int i;

  for(i = 0; i < n_vertices; i++) {

      int xp = drawlist[0];
      int yp = drawlist[1];
      pixels [xp + yp * stride] = palette[drawlist[2]];
      drawlist += 3;
  } 
}


// ---------------------------------------------------------------------------

static unsigned int palette [256];

int main ( int argc, char** argv)
{
  int end = 0;
  int mouse_x = 0, mouse_y = 0;
  SDL_Surface  *g_SDLSrf;
  int req_w = 1024;
  int req_h = 768; 

    
  if ( argc < 2) { fprintf ( stderr, "I need the cpu speed in Mhz!\n"); exit(0);}
  cpu_mhz = atoi( argv[1]);
  assert(cpu_mhz > 0);
  fprintf ( stdout, "Cycle times for a %d Mhz cpu\n", cpu_mhz);

  // Init SDL and screen
  if ( SDL_Init( SDL_INIT_VIDEO) < 0 ) {
      fprintf(stderr, "Can't Initialise SDL: %s\n", SDL_GetError());
      exit(1);
  }
  if (0 == SDL_SetVideoMode( req_w, req_h, 32,  SDL_HWSURFACE | SDL_DOUBLEBUF)) {
      printf("Couldn't set %dx%dx32 video mode: %s\n", req_w, req_h, SDL_GetError());
      return 0;
  }

  // Dot palette
  int i;
  for (i=0; i<256; i++)
    palette [i] = i | (i << 8) | (i << 16);



  // Small footprint buffer for vertices draw  
  // we are not using an structure to avoid padding to 8 bytes (instead of current 6)
  int map_size = 1024;
  int sub_map_size = 128;
  
  float x_mouse_factor =  (float)(map_size - sub_map_size)/req_w;
  float y_mouse_factor =  (float)(map_size - sub_map_size)/req_h;



  int n_vertices = map_size * map_size;
  short* drawlist = (short*) malloc (sub_map_size * sub_map_size * 3 * sizeof(short));
  //float* meshpoints = (float*)malloc(n_vertices * 3 * sizeof(float));  
  unsigned char* meshpoints1 = (unsigned char*)malloc(n_vertices * sizeof(short)); 
  int* meshfarpoints = (int*)malloc(sub_map_size * sub_map_size * sizeof(int)); 

 
  // Setup your effect initialization here
  // Horizontal field of view
  float hfov = 60.0f * ((3.1416f * 2.0f) / 360.0f);  // Degrees to radians
  float half_scr_w = (float)(req_w >> 1);
  float projection = (1.0f / tan ( hfov * 0.5f)) * half_scr_w;

  //Creation of the mesh
  CreateMesh(map_size, map_size, 
              meshpoints1,
              sub_map_size, sub_map_size, 
              drawlist, projection,
              req_w, meshfarpoints );

  // Main loop
  g_SDLSrf = SDL_GetVideoSurface();
  while ( !end) { 

    SDL_Event event;
    //  meshp += origin_y * w_map + origin_x;
    unsigned char* init_meshp = meshpoints1 + (int)(mouse_y * y_mouse_factor) * map_size + (int)(mouse_x * x_mouse_factor);
    int n_draw = sub_map_size * sub_map_size;
    //int n_draw = 1;
    // Your gfx effect goes here
    ChronoWatchReset();
    
    HeighMapEffect(sub_map_size, 
                  map_size - sub_map_size, 
                  drawlist,
                  g_SDLSrf->h >>1,
                  init_meshp,
                  meshfarpoints);

   


   ChronoShow("LandScape festival,", sub_map_size * sub_map_size);

    /*ChronoWatchReset();
    int n_draw = DoEffect (drawlist, n_vertices, g_SDLSrf->w, g_SDLSrf->h, dump, projection);
    assert(n_draw <= n_vertices);
    ChronoShow ( "Donut festival", n_vertices);*/

    // Draw vertices; don't modify this section
    // Lock screen to get access to the memory array
    SDL_LockSurface( g_SDLSrf);

    // Clean the screen
    SDL_FillRect(g_SDLSrf, NULL, SDL_MapRGB(g_SDLSrf->format, 0, 0, 0));
    ChronoShow ( "Clean", g_SDLSrf->w * g_SDLSrf->h);

    // Paint vertices
    DisplayVertices (g_SDLSrf->pixels, drawlist, n_draw, g_SDLSrf->pitch >> 2, palette);
    ChronoShow ( "Preview", n_draw);

    //Unlock the draw surface, dump to physical screen
    ChronoWatchReset();
    SDL_UnlockSurface( g_SDLSrf);
    SDL_Flip( g_SDLSrf);
    //ChronoShow ( "Screen dump", g_SDLSrf->w * g_SDLSrf->h);

    // Limit framerate and return any remaining time to the OS
    // Comment this line for benchmarking
    FramerateLimit (60);

    dump++;

    // Recoger eventos de la ventana
    while ( SDL_PollEvent(&event) ) 
    {
      switch (event.type) 
      {
        case SDL_MOUSEMOTION:
          mouse_x = event.motion.x;
          mouse_y = event.motion.y;
          break;
        case SDL_MOUSEBUTTONDOWN:
          //printf("Mouse button %d pressed at (%d,%d)\n",
          //       event.button.button, event.button.x, event.button.y);
          break;
        case SDL_QUIT:
          end = 1;
          break;
      }
    }
  }

  return 1;
}


