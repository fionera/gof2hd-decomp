# ImagePart::ImagePart
elf 0xa7a74 body 48
Sig: undefined __thiscall ImagePart(ImagePart * this, uint param_1, int param_2, int param_3)

## decompile
```c

/* ImagePart::ImagePart(unsigned int, int, int) */

ImagePart * __thiscall ImagePart::ImagePart(ImagePart *this,uint param_1,int param_2,int param_3)

{
  int iVar1;
  uint *puVar2;
  
  iVar1 = DAT_000b7aa4;
  this->id = param_1;
  this->field_04 = param_2;
  this->pos_y = param_3;
  puVar2 = *(uint **)(iVar1 + 0xb7a8a);
  iVar1 = AbyssEngine::PaintCanvas::GetImage2DWidth(*puVar2);
  this->scale_x = iVar1;
  iVar1 = AbyssEngine::PaintCanvas::GetImage2DHeight(*puVar2);
  this->scale_y = iVar1;
  return this;
}

```

## target disasm
```
  000b7a74: push {r4,r5,r6,r7,lr}
  000b7a76: add r7,sp,#0xc
  000b7a78: push.w r11
  000b7a7c: mov r5,r0
  000b7a7e: ldr r0,[0x000b7aa4]
  000b7a80: stm r5,{r1,r2,r3}
  000b7a84: mov r4,r1
  000b7a86: add r0,pc
  000b7a88: ldr r6,[r0,#0x0]
  000b7a8a: ldr r0,[r6,#0x0]
  000b7a8c: blx 0x00072d84
  000b7a90: str r0,[r5,#0xc]
  000b7a92: mov r1,r4
  000b7a94: ldr r0,[r6,#0x0]
  000b7a96: blx 0x00072d90
  000b7a9a: str r0,[r5,#0x10]
  000b7a9c: mov r0,r5
  000b7a9e: pop.w r11
  000b7aa2: pop {r4,r5,r6,r7,pc}
```
