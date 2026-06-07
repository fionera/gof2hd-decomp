# PlayerStation::outerCollide
elf 0x122778 body 196
Sig: undefined __thiscall outerCollide(PlayerStation * this, float param_1, float param_2, float param_3)

## decompile
```c

/* PlayerStation::outerCollide(float, float, float) */

undefined4 __thiscall
PlayerStation::outerCollide(PlayerStation *this,float param_1,float param_2,float param_3)

{
  uint *puVar1;
  int iVar2;
  float in_r1;
  float in_r2;
  float in_r3;
  uint uVar3;
  uint in_fpscr;
  float fVar4;
  float extraout_s0;
  float extraout_s1;
  
  fVar4 = (float)VectorSignedToFloat(*(undefined4 *)(this + 0x154),(byte)(in_fpscr >> 0x16) & 3);
  if ((((in_r1 < *(float *)(this + 0x58) + fVar4) &&
       ((int)((uint)(*(float *)(this + 0x58) - fVar4 < in_r1) << 0x1f) < 0)) &&
      (in_r2 < *(float *)(this + 0x5c) + fVar4)) &&
     ((((int)((uint)(*(float *)(this + 0x5c) - fVar4 < in_r2) << 0x1f) < 0 &&
       (in_r3 < *(float *)(this + 0x60) + fVar4)) &&
      ((fVar4 = *(float *)(this + 0x60) - fVar4, (int)((uint)(fVar4 < in_r3) << 0x1f) < 0 &&
       (puVar1 = *(uint **)(this + 0x130), puVar1 != (uint *)0x0)))))) {
    for (uVar3 = 0; uVar3 < *puVar1; uVar3 = uVar3 + 1) {
      iVar2 = (**(code **)(**(int **)(puVar1[1] + uVar3 * 4) + 0xc))(fVar4,param_2);
      if (iVar2 != 0) {
        *(uint *)(this + 0x150) = uVar3;
        return 1;
      }
      puVar1 = *(uint **)(this + 0x130);
      fVar4 = extraout_s0;
      param_2 = extraout_s1;
    }
  }
  return 0;
}

```

## target disasm
```
  00132778: push {r4,r5,r6,r7,lr}
  0013277a: add r7,sp,#0xc
  0013277c: push {r8,r9,r10}
  00132780: vldr.32 s0,[r0,#0x154]
  00132784: vmov s4,r1
  00132788: vldr.32 s2,[r0,#0x58]
  0013278c: mov r6,r0
  0013278e: vcvt.f32.s32 s0,s0
  00132792: mov r8,r3
  00132794: mov r9,r2
  00132796: mov r10,r1
  00132798: movs r0,#0x0
  0013279a: vadd.f32 s6,s2,s0
  0013279e: vcmpe.f32 s6,s4
  001327a2: vmrs apsr,fpscr
  001327a6: ble 0x00132836
  001327a8: vsub.f32 s2,s2,s0
  001327ac: vcmpe.f32 s2,s4
  001327b0: vmrs apsr,fpscr
  001327b4: bpl 0x00132836
  001327b6: vldr.32 s4,[r6,#0x5c]
  001327ba: vmov s2,r9
  001327be: movs r0,#0x0
  001327c0: vadd.f32 s6,s4,s0
  001327c4: vcmpe.f32 s6,s2
  001327c8: vmrs apsr,fpscr
  001327cc: ble 0x00132836
  001327ce: vsub.f32 s4,s4,s0
  001327d2: vcmpe.f32 s4,s2
  001327d6: vmrs apsr,fpscr
  001327da: bpl 0x00132836
  001327dc: vldr.32 s4,[r6,#0x60]
  001327e0: vmov s2,r8
  001327e4: movs r0,#0x0
  001327e6: vadd.f32 s6,s4,s0
  001327ea: vcmpe.f32 s6,s2
  001327ee: vmrs apsr,fpscr
  001327f2: ble 0x00132836
  001327f4: vsub.f32 s0,s4,s0
  001327f8: vcmpe.f32 s0,s2
  001327fc: vmrs apsr,fpscr
  00132800: bpl 0x00132836
  00132802: ldr.w r0,[r6,#0x130]
  00132806: cbz r0,0x0013282c
  00132808: movs r5,#0x0
  0013280a: b 0x00132826
  0013280c: ldr r0,[r0,#0x4]
  0013280e: mov r2,r9
  00132810: mov r3,r8
  00132812: ldr.w r0,[r0,r5,lsl #0x2]
  00132816: ldr r1,[r0,#0x0]
  00132818: ldr r4,[r1,#0xc]
  0013281a: mov r1,r10
  0013281c: blx r4
  0013281e: cbnz r0,0x00132830
  00132820: ldr.w r0,[r6,#0x130]
  00132824: adds r5,#0x1
  00132826: ldr r1,[r0,#0x0]
  00132828: cmp r5,r1
  0013282a: bcc 0x0013280c
  0013282c: movs r0,#0x0
  0013282e: b 0x00132836
  00132830: movs r0,#0x1
  00132832: str.w r5,[r6,#0x150]
  00132836: pop.w {r8,r9,r10}
  0013283a: pop {r4,r5,r6,r7,pc}
```
