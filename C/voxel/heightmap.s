@void HeighMapEffect(int map_size,
@                    int dif_size_maps,                 
@                    short* drawlist,
@                    int scy,
@                    unsigned char* meshp,
@                    int *meshfarpoints );





@.global HeighMapEffect

HeighMapEffect:

    ldr r4,[sp]         @extrac meshp from stack
    ldr r5,[sp,#4]      @extrac meshfarpoints from stack
    stmdb   sp!,{r4,r5,r6,r7,r8,r9,r10} @ prolog

    mov r6,r0          @ i = map_size
    
loop_for_i:

    mov r7,r0           @ j = map_size

loop_for_j:

    ldrb r8,[r4],#1     @ unsigned char y = *meshp++;
    ldr  r9,[r5],#4     @ int zpi = *meshfarpoints++;
  
    sub r10,r8,#1000    @ tmp = y-1000
    mul r10,r10,r9      @ tmp = tmp * zpi
    mov r10,r10,asr#8   @ int yrelativepos  = ((y-1000) * zpi) >> 8; 
    sub r10,r3,r10      @ int yp = scy - yrelativepos; 

    strh r10,[r2,#2]    @ drawlist[1] = yp;
    and r8,r8,#0xff     @ y & 0xff;
    strh r8,[r2,#4]     @ drawlist[2] = y & 0xff;
    add r2,r2,#6        @ drawlist += 3; 

    subs r7,r7,#1       @ j--; 
    bgt loop_for_j      

    add r4,r4,r1        @ meshp += dif_size_maps;

    subs r6,r6,#1       @ i--; 
    bgt loop_for_i      

    ldmia   sp!,{r4,r5,r6,r7,r8,r9,r10} @epilog
    bx      lr






