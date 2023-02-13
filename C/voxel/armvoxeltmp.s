@ ---------------------------
@           VOXEL
@ ---------------------------

@typedef struct{
  
@  int znear;             +0
@  int zfar;              +4
@  int *uFX;              +8
@  int *vFX;              +12
@  int vxFX;              +16
@  int vzFX;              +20
@  int mipmapSizePow2;    +24
@  int maskReadTexture;   +28
@  int* texture;          +32
@  int cameraHeight;      +36
@  int* proyectionFX;     +40
@  int screenH;           +44
@  unsigned int* pixels;  +48
@  int* ymin;             +52
  

@}TInner

@ void InnerVoxel(TInner* innerData)
@ r0 -> TInner* innerData


.global InnerVoxel

InnerVoxel:

  @ prolog
  stmdb   sp!,{r4,r5,r6,r7,r8,r9,r10,r11,r12,r14}


  ldr r1,[r0]               @ int znear = innerData->znear;
  ldr r2,[r0,#4]            @ int zfar = innerData->zfar;

  ldr r3,[r0,#8]            @ innerData->uFX;
  ldr r3,[r3]               @ int uFX = *(innerData->uFX);
  ldr r4,[r0,#12]           @ innerData->vFX;
  ldr r4,[r4]               @ int vFX = *(innerData->vFX);
  ldr r12,[r0,#16]          @ int vxFX = innerData->vxFX;
  ldr r14,[r0,#20]          @ int vzFX = innerData->vzFX;


  ldr r5,[r0,#52]           @ int ymin = innerData->ymin;
  ldr r5,[r5]               @ int ymin = *(innerData->ymin);
  ldr r9, [r0,#40]          @ int* proyectionFX = innerData->proyectionFX;

  
  

start_for:


  ldr r6,[r0,#36]           @ int cameraHeight = innerData->cameraHeight;
  add r3,r3,r12             @ uFX = uFX + vxFX;
  add r4,r4,r14             @ vFX = vFX + vzFX;
  ldr r10,[r0,#24]          @ int mipmapSizePow2 = innerData->mipmapSizePow2; 
  mov r7,r3, asr#12         @ int ui = uFX >> radix;
  mov r8,r4, asr#12         @ int uv = vFX >> radix;
  ldr r11,[r0,#28]          @ int maskReadTexture = innerData->maskReadTexture

  add r7,r7,r8,lsl r10      @ int textureIndexI = ui + (vi << mipmapSizePow2 );
  ldr r10,[r0,#32]          @ int* texture = innerData->texture;
  
  and r7,r7,r11             @ textureIndexI &= maskReadTexture;
  ldr r11,[r9,r1,lsl#2]     @ proyectionFX[zi]         
  
  ldr r7,[r10,r7,lsl#2]     @ unsigned int mapInfo = texture [textureIndexI]; 
  add r8,r6,r7,lsr#24       @ int yi = (mapInfo>>24) + cameraHeight;
  

  ldr r10,[r0,#44]          @ int screenH = innerData->screenH;
  mov r11,r11,asr#12        @ proyectionFX[zi]>>radix;
  mul r6,r8,r11             @ int yp = ( yi * (proyectionFX[zi])>>radix);
  adds r6,r6,r10, asr#1     @ yp += screenH>>1;

  movlt r6,#0               @ yp = 0
  ldr r10,[r0,#48]          @ unsigned int* pixels = innerData->pixels;
  mov r8, #0xff0000;        @ r8 = 0xff0000
  orr r8,r8, #0xff00;       @ r8 = 0xffff00
  orr r8,r8, #0xff;         @ r8 = 0xffffff
  and r8,r7,r8              @ int color = mapInfo & 0x00ffffff;

  
  add r10,r10,r5,lsl#2      @ unsigned int* tmpPixels = pixels + ymin;


  cmp r5,r6                 @ compare ymin with yp

  ble end_draw_for

draw_for:
  
  str r8,[r10]              @ *tmpPixels = color;
  sub r5,r5,#1              @ --ymin
  sub r10,r10,#4            @ tmpPixels--;
 
  cmp r5,r6                 @ compare ymin with yp
  bgt draw_for


end_draw_for:

  add r1,r1,#1              @ zi++
  cmp r1,r2
  blt start_for


  ldr r10,[r0,#52]          @ innerData->ymin
  str r5,[r10]              @ *(innerData->ymin) = ymin;

  ldr r11,[r0,#8]           @ innerData->uFX
  str r3,[r11]              @ *(innerData->uFX) = uFX;

  ldr r12,[r0,#12]          @ innerData->vFX
  str r4,[r12]              @ *(innerData->vFX) = vFX;
     
  
  @ epilog
  ldmia   sp!,{r4,r5,r6,r7,r8,r9,r10,r11,r12,r14}
  bx lr

 




        
