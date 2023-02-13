

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

#include <SDL/SDL.h>

#include "chrono.h"

// ---------------------------------------------------------------------------

#define PI  3.14159265359f



static int cpu_mhz = 0;
static int dump = 0;


typedef struct {
  float x,z;
}TVector;

typedef struct {
  int x,z;
}TVectorFX;

typedef struct{
  unsigned char* textureHM ;
  int* textureColor ;
}TImage;

typedef struct{
  int far;
  float cameraHeight;
  TVector initPos;
  float* proyectionZ;
  int* proyectionZFX;
  int* mipmap0;
  int* mipmap1;
  int* mipmap2;
  int numberMM;
}TData;

typedef struct{
  int screenW;
  int screenH;
  unsigned int* pixels;
  unsigned int* drawList;
  int stride;
  float hfov;

  int mm0Distance;
  int mm1Distance;
  int mm2Distance;

  

}TScreenData;

typedef struct{
  TVector forward;
  TVector right;
  TVector up; 
}TMovement;


typedef struct{
  
  int znear;
  int zfar;
  int *uFX;
  int *vFX; 
  int vxFX;
  int vzFX;
  int mipmapSizePow2; 
  int maskReadTexture;
  int* texture;
  int cameraHeight;
  int* proyectionFX;
  int screenH;
  unsigned int* pixels;

  int* ymin;

}TInner;

const int radix = 12;

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

static inline int fToFx(float f){
  
  int fx = (int) (f * (float)(1<<radix));
  return fx;
}

static inline float fxToF(int fx){
  
  float f = ((float)fx /(float)(1<<radix));
  return f;
}


static void InitVoxel(TData* data, float proyection, TScreenData* screenData){


  SDL_Surface *imageHM0 = SDL_LoadBMP("resources/D19.bmp"); 
  SDL_Surface *imageColor0 = SDL_LoadBMP("resources/C19.bmp"); 

  SDL_Surface *imageHM1 = SDL_LoadBMP("resources/D19_1.bmp"); 
  SDL_Surface *imageColor1 = SDL_LoadBMP("resources/C19_1.bmp"); 

  SDL_Surface *imageHM2 = SDL_LoadBMP("resources/D19_2.bmp"); 
  SDL_Surface *imageColor2 = SDL_LoadBMP("resources/C19_2.bmp"); 

  int texWidthHM = imageHM0->w;
  int texHeightHM = imageHM0->h;
  int sizeTexture0 = texWidthHM * texHeightHM;
  int sizeTexture1 = sizeTexture0>>2;
  int sizeTexture2 = sizeTexture1>>2;

  data->mipmap0 = (int*) malloc(sizeof(int)* sizeTexture0);
  data->mipmap1 = (int*) malloc(sizeof(int)* sizeTexture1);
  data->mipmap2 = (int*) malloc(sizeof(int)* sizeTexture2);



  for(int i = 0; i < sizeTexture0; i++){
    
    int j = i<<2;
    int color  = *(unsigned char*)(imageHM0->pixels+i) << 24;
    color = color | *(unsigned char*)(imageColor0->pixels+j+0)<<16;
    color = color | *(unsigned char*)(imageColor0->pixels+j+1)<<8;
    color = color | *(unsigned char*)(imageColor0->pixels+j+2);
    *(data->mipmap0+ i) = color;
     
  }

  for(int i = 0; i < sizeTexture1; i++){
    
    int j = i<<2;
    int color  = *(unsigned char*)(imageHM1->pixels+i) << 24;
    color = color | *(unsigned char*)(imageColor1->pixels+j+0) <<16;
    color = color | *(unsigned char*)(imageColor1->pixels+j+1) <<8;
    color = color | *(unsigned char*)(imageColor1->pixels+j+2);
    *(data->mipmap1 + i) = color;
     
  }

  for(int i = 0; i < sizeTexture2; i++){
    
    int j = i<<2;
    int color = *(unsigned char*)(imageHM2->pixels+i) << 24;
    color = color | *(unsigned char*)(imageColor2->pixels+j+0) <<16;
    color = color | *(unsigned char*)(imageColor2->pixels+j+1) <<8;
    color = color | *(unsigned char*)(imageColor2->pixels+j+2);
    *(data->mipmap2 + i) = color;
     
  }

 
  int i = 0;
   
  screenData->mm0Distance = 350;
  screenData->mm1Distance = 50 + screenData->mm0Distance;
  screenData->mm2Distance = 25 + screenData->mm1Distance;

  float farStep = 1.0f;
  float z = 1.0f;
  for(i = 0; i < screenData->mm2Distance; ++i){
    *(data->proyectionZ + i) =  proyection/z;
    *(data->proyectionZFX + i) =  fToFx(proyection/z); 
  

    if(i > screenData->mm0Distance){ farStep = 2.0f;}
    if(i > screenData->mm1Distance){ farStep = 4.0f;}

    z+=farStep;
  }

}

// ---------------------------------------------------------------------------
#if 1
void InnerVoxel(TInner* innerData); 

#else
static void InnerVoxel(TInner* innerData){


  int znear = innerData->znear;
  int zfar = innerData->zfar;
  int uFX = *(innerData->uFX);
  int vFX = *(innerData->vFX); 
  int vxFX = innerData->vxFX;
  int vzFX = innerData->vzFX;
  int mipmapSizePow2 = innerData->mipmapSizePow2; 
  int maskReadTexture = innerData->maskReadTexture;
  int* texture = innerData->texture;
  int cameraHeight = innerData->cameraHeight;
  int* proyectionFX = innerData->proyectionFX;
  int screenH = innerData->screenH;
  unsigned int* pixels = innerData->pixels;
  int ymin = *(innerData->ymin);

  int zi = znear; 
     
  
  for(zi = znear; zi < zfar; zi+=1){
      uFX = uFX + vxFX;
      vFX = vFX + vzFX;
     
      int ui = uFX >> radix;
      int vi = vFX >> radix;
      int textureIndexI = ui + (vi << mipmapSizePow2 );
      textureIndexI &= maskReadTexture;
      
      unsigned int mapInfo = texture [textureIndexI];
      int yi = (mapInfo>>24) + cameraHeight;
    
    
      int yp = ( yi * (proyectionFX[zi])>>radix);
      yp += screenH>>1;
     
      if(yp < 0){
        yp = 0;
      }
        
         
      int color = mapInfo & 0x00ffffff;

      //HORIZONTAL
      unsigned int* tmpPixels = pixels + ymin;
      for( ;ymin > yp; --ymin){

        *tmpPixels = color;
         tmpPixels--;
      }
        

   
      
    }

    *(innerData->ymin) = ymin;
    *(innerData->uFX) = uFX;
    *(innerData->vFX)= vFX; 
}
#endif

static void VoxelFX(TData data, TScreenData screenData){
  


  
  TVector right, left; 

  left.x = cos(screenData.hfov * 0.5f - PI*0.5f) ; //-1.0f;
  left.z = sin(screenData.hfov * 0.5f - PI*0.5f);

  right.x = - left.x;
  right.z = left.z;

  float invzright = 1.0f/right.z;
  right.x *= invzright;
  right.z *= invzright;

  float invzleft = 1.0f/left.z;
  left.x *= invzleft;
  left.z *= invzleft;


  TVectorFX rightFX, leftFX; 
  rightFX.x = fToFx(right.x);
  rightFX.z = fToFx(right.z);
  leftFX.x =  fToFx(left.x);
  leftFX.z =  fToFx(left.z);

  int screenW = screenData.screenW;
  int screenH = screenData.screenH;
  unsigned int* pixels = screenData.pixels;
  unsigned int* drawList = screenData.drawList;
  int stride = screenData.stride;
  int oneFX = fToFx(1.0f);

  int mipmap0SizePow2 = 10;
  int mipmap1SizePow2 = 9;
  int mipmap2SizePow2 = 8;
  int maskReadTexture0 =  ( (1<<(mipmap0SizePow2+mipmap0SizePow2)) -1);
  int maskReadTexture1 =  ( (1<<(mipmap1SizePow2+mipmap1SizePow2)) -1);
  int maskReadTexture2 =  ( (1<<(mipmap2SizePow2+mipmap2SizePow2)) -1);


  int* mipmap0Texture = data.mipmap0;
  int* mipmap1Texture = data.mipmap1;
  int* mipmap2Texture = data.mipmap2;

  int cameraHeightI = (int)data.cameraHeight;

  
 

  TVectorFX globalCoordsFX;
  globalCoordsFX.x = fToFx(data.initPos.x);
  globalCoordsFX.z = fToFx(data.initPos.z);

  int x;
  int edgemipmap0 = screenData.mm0Distance; 
  int edgemipmap1 = screenData.mm1Distance;
  int edgemipmap2 = screenData.mm2Distance;

  int screenXSize = screenW;


  
  TInner innerData;

    

  innerData.cameraHeight = cameraHeightI;
  innerData.proyectionFX = data.proyectionZFX;
  innerData.screenH = screenH;
  

  //Clean drawList
  ChronoShow( "ClearDrawList", screenH * screenW);
  for(int i=0; i < screenH; i++){
    for(int j=0; j < screenW; j++){
      drawList[j + (i * screenW)] = 0x6287DE;
    }
  }
  ChronoWatchReset();


  

  for(x = 0; x < screenXSize; x+=1){

    int alphaFX = ( ((x)<<radix)/(screenW) );
    //int alphaFX = x*screenWInvFX;
    int ymin = screenH - 1;
    int uFX = globalCoordsFX.x;
    int vFX = globalCoordsFX.z;
 
    int vxFX = rightFX.x * alphaFX + leftFX.x * (oneFX - alphaFX);
    vxFX = vxFX >>(radix);
    int vzFX = rightFX.z * alphaFX + leftFX.z * (oneFX - alphaFX);
    vzFX = vzFX >>(radix);
    
    

    //Commom

    innerData.uFX = &uFX;
    innerData.vFX = &vFX;
    innerData.vxFX = vxFX;
    innerData.vzFX = vzFX;
    innerData.ymin = &ymin;
    //innerData.pixels = drawList + x;
    innerData.pixels = drawList + (x * stride);


    //MipMap0
    if(data.numberMM >0){
      innerData.znear = 0;
      innerData.zfar = edgemipmap0;
      innerData.mipmapSizePow2 = mipmap0SizePow2;
      innerData.maskReadTexture = maskReadTexture0;
      innerData.texture = mipmap0Texture;
      
      InnerVoxel(&innerData);
    }


    //MipMap1
    if(data.numberMM > 1){
      *(innerData.uFX) = *(innerData.uFX) >> 1;
      *(innerData.vFX) = *(innerData.vFX) >> 1;
      innerData.znear = edgemipmap0;
      innerData.zfar = edgemipmap1;
      innerData.mipmapSizePow2 = mipmap1SizePow2;
      innerData.maskReadTexture = maskReadTexture1;
      innerData.texture = mipmap1Texture;
    
      InnerVoxel(&innerData);
    }

    //MipMap2

    if(data.numberMM > 2){
      *(innerData.uFX) = *(innerData.uFX) >> 1;
      *(innerData.vFX) = *(innerData.vFX) >> 1;
      innerData.znear = edgemipmap1;
      innerData.zfar = edgemipmap2;
      innerData.mipmapSizePow2 = mipmap2SizePow2;
      innerData.maskReadTexture = maskReadTexture2;
      innerData.texture = mipmap2Texture;

      InnerVoxel(&innerData);
    }

  }
  ChronoShow ( "Voxel", screenXSize * innerData.zfar);

  
  //DrawList to pixel
  /*for(int i=0; i < screenH; i++){
    for(int j=0; j < screenW; j++){
      pixels[j + (i * screenW)] = drawList[i + j*screenW];
    }
  }*/
  int tilingSize = 10;
  int screenXStep = tilingSize;
  int screenYStep = tilingSize;
  for(int i=0; i < screenH; i+=screenXStep){
    for(int j=0; j < screenW; j+=screenYStep){
      unsigned int* auxPixels = (pixels + j + (i*stride));
      unsigned int* auxDrawList = (drawList + i + (j*stride));
      for(int tx = 0; tx < tilingSize; tx++){
        for(int ty = 0; ty < tilingSize; ty++){
          auxPixels[ty + (tx * stride)] = auxDrawList[tx + ty*stride];
        } 
      }
      
    }
  }
  ChronoShow( "Tiling", screenH * screenW);
}




// ---------------------------------------------------------------------------

static unsigned int palette [256];

int main ( int argc, char** argv)
{
  int end = 0;
  TScreenData screenData;
  //int mouse_x = 0, mouse_y = 0;
  SDL_Surface  *g_SDLSrf;


  int req_w = 800;
  int req_h = 800;
  screenData.screenW = req_w;
  screenData.screenH = req_h;
  screenData.drawList = (unsigned int*)malloc(sizeof(unsigned int) *  req_w * req_h);


  TData data;
  data.far = 500.0f;
  data.numberMM = 1;
  data.cameraHeight = 20.0f;
  data.initPos.x = 240.0f;
  data.initPos.z = 26.0f;
  data.proyectionZ = (float*)malloc(sizeof(float) *  (int)data.far);
  data.proyectionZFX  = (int*)malloc(sizeof(int) *  (int)data.far);

  TMovement direction;
  direction.forward.x = 0;
  direction.forward.z = 0;

  direction.right.x = 0;
  direction.right.z = 0;

  direction.up.x = 0;
  direction.up.z = 0;
    
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
  int i, j = 255;
  for (i=0; i<256; i++, j--)
    palette [j] = i | (i << 8) | (i << 16);




  // Small footprint buffer for vertices draw  
  // we are not using an structure to avoid padding to 8 bytes (instead of current 6

  // Setup your effect initialization here
  // Horizontal field of view
  float hfov = 60.0f * ((3.1416f * 2.0f) / 360.0f);  // Degrees to radians
  screenData.hfov = hfov;
  float half_scr_w = (float)(req_w >> 1);
  float projection = (1.0f / tan ( hfov * 0.5f)) * half_scr_w;

  //InitVoxel(&textureHM, &textureColor, &textureColorI, &data, projection);
  InitVoxel(&data, projection, &screenData);

  // Main loop
  g_SDLSrf = SDL_GetVideoSurface();
  screenData.pixels = g_SDLSrf->pixels;
  screenData.stride = g_SDLSrf->pitch >> 2;
  
  while ( !end) { 

    SDL_Event event;

    //int n_draw = 1;
    // Your gfx effect goes here
    ChronoWatchReset();
    
    

 
  // ChronoShow("LandScape festival,", sub_map_size * sub_map_size);



    // Draw vertices; don't modify this section
    // Lock screen to get access to the memory array
    SDL_LockSurface( g_SDLSrf);

    // Clean the screen
    SDL_FillRect(g_SDLSrf, NULL, SDL_MapRGB(g_SDLSrf->format, 0, 0, 0));
    ChronoShow ( "Clean", g_SDLSrf->w * g_SDLSrf->h);

    //Move 
    float speed = 2.0f;
    data.initPos.z += direction.forward.z * speed; 
    data.initPos.x += direction.right.x * speed;
    data.cameraHeight += direction.up.x * speed;


   
    
    VoxelFX( data, screenData);

 

    
    
    //Unlock the draw surface, dump to physical screen
    ChronoWatchReset();
    SDL_UnlockSurface( g_SDLSrf);
    SDL_Flip( g_SDLSrf);
    ChronoShow ( "Screen dump", g_SDLSrf->w * g_SDLSrf->h);

    // Limit framerate and return any remaining time to the OS
    // Comment this line for benchmarking
    //FramerateLimit (60);

    dump++;

    unsigned char s = 0;
          
    
    // Recoger eventos de la ventana
    while ( SDL_PollEvent(&event) ) 
    {
      switch (event.type) 
      {
        /*case SDL_MOUSEMOTION:
          mouse_x = event.motion.x;
          mouse_y = event.motion.y;
          break;
        case SDL_MOUSEBUTTONDOWN:
          //printf("Mouse button %d pressed at (%d,%d)\n",
                // event.button.button, event.button.x, event.button.y);
          break;*/



      
        case SDL_KEYUP:
     
          switch (event.key.keysym.sym){

            case SDLK_w:
              
              direction.forward.z = 0.0f; 
              s = 1;
            break;
            case SDLK_s: 
              s = 1;
              direction.forward.z = 0.0f;
            break;

            case SDLK_a: 
              direction.right.x = 0.0f;
              s = 1;
            break;
            case SDLK_d: 
              direction.right.x = 0.0f;
              s = 1;
            break;

            case SDLK_e: 
              direction.up.x = 0.0f;
              s = 1;
            break;
            case SDLK_q: 
              direction.up.x = 0.0f;
              s = 1;
            break;


            default:
            break;
          }





        case SDL_KEYDOWN:
          if(s == 0)
          switch (event.key.keysym.sym){
        
            case SDLK_w:
             direction.forward.z = 1.0f;
            break;
            case SDLK_s: 
              direction.forward.z = -1.0f;
            break;

            case SDLK_a: 
              direction.right.x = -1.0f;
            break;
            case SDLK_d: 
              direction.right.x = 1.0f;
            break;

            case SDLK_e: 
              direction.up.x = -1.0f;

            break;
            case SDLK_q: 
              direction.up.x = 1.0f;

            break;

            case SDLK_1: 
              data.numberMM = 1;
            break;
            case SDLK_2: 
              data.numberMM = 2;
            break;
            case SDLK_3: 
              data.numberMM = 3;
            break;


            default:
            break;
          }
          
          


          break;
        case SDL_QUIT:
          end = 1;
          break;
      }
    }
  }

  return 1;
}


