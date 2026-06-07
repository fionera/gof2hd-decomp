# PlayerWormHole::setPosition
elf 0xa5336 body 62
Sig: undefined __stdcall setPosition(float param_1, float param_2, float param_3)

## decompile
```c

/* PlayerWormHole::setPosition(float, float, float) */

void PlayerWormHole::setPosition(float param_1,float param_2,float param_3)

{
  int in_r0;
  float in_r1;
  float in_r2;
  float in_r3;
  
  *(float *)(in_r0 + 0x58) = in_r1;
  *(float *)(in_r0 + 0x5c) = in_r2;
  *(float *)(in_r0 + 0x60) = in_r3;
  *(int *)(in_r0 + 0x124) = (int)in_r1;
  *(int *)(in_r0 + 0x128) = (int)in_r2;
  *(int *)(in_r0 + 300) = (int)in_r3;
  (*(code *)(DAT_001abd74 + 0x1abd78))(*(undefined4 *)(in_r0 + 8));
  return;
}

```

## target disasm
```
  000b5336: vmov s2,r2
  000b533a: vmov s0,r1
  000b533e: vmov s4,r3
  000b5342: vcvt.s32.f32 s2,s2
  000b5346: vcvt.s32.f32 s0,s0
  000b534a: vcvt.s32.f32 s4,s4
  000b534e: ldr.w r12,[r0,#0x8]
  000b5352: str r1,[r0,#0x58]
  000b5354: str r2,[r0,#0x5c]
  000b5356: str r3,[r0,#0x60]
  000b5358: vstr.32 s0,[r0,#0x124]
  000b535c: vstr.32 s2,[r0,#0x128]
  000b5360: vstr.32 s4,[r0,#0x12c]
  000b5364: mov r0,r12
  000b5366: b.w 0x001abd68
  001abd68: bx pc
  001abd6c: ldr r12,[0x1abd74]
  001abd70: add pc,r12,pc
```
