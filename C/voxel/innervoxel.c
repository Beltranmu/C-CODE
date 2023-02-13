
#include <stdio.h>

const int radix = 12;
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





void InnerVoxel(TInner* innerData){


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
