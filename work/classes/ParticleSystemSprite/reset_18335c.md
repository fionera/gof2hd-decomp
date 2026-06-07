# ParticleSystemSprite::reset
elf 0x18335c body 92
Sig: undefined __stdcall reset(void)

## decompile
```c

/* ParticleSystemSprite::reset() */

void ParticleSystemSprite::reset(void)

{
  int in_r0;
  int iVar1;
  float in_s0;
  float extraout_s0;
  float in_s1;
  float extraout_s1;
  float in_s2;
  float extraout_s2;
  
  for (iVar1 = 0; iVar1 < *(int *)(in_r0 + 0x48); iVar1 = iVar1 + 1) {
    AbyssEngine::PaintCanvas::SpriteSystemSetPosition
              (*(uint *)(in_r0 + 8),(ushort)*(undefined4 *)(in_r0 + 0x54),in_s0,in_s1,in_s2);
    AbyssEngine::PaintCanvas::SpriteSystemSetSize
              (*(uint *)(in_r0 + 8),(ushort)*(undefined4 *)(in_r0 + 0x54),
               (short)*(undefined4 *)(in_r0 + 0x58) + (short)iVar1);
    *(undefined4 *)(*(int *)(in_r0 + 0x68) + iVar1 * 4) = 0xffffffff;
    in_s0 = extraout_s0;
    in_s1 = extraout_s1;
    in_s2 = extraout_s2;
  }
  *(undefined1 *)(in_r0 + 4) = 1;
  *(undefined4 *)(in_r0 + 0x60) = 0;
  return;
}

```

## target disasm
```
  0019335c: push {r4,r5,r6,r7,lr}
  0019335e: add r7,sp,#0xc
  00193360: push.w r8
  00193364: sub sp,#0x8
  00193366: mov r4,r0
  00193368: movs r5,#0x0
  0019336a: mov.w r6,#0x4f800000
  0019336e: mov.w r8,#0xffffffff
  00193372: b 0x001933a2
  00193374: ldr r0,[r4,#0x8]
  00193376: mov.w r3,#0x4f800000
  0019337a: ldrd r1,r2,[r4,#0x54]
  0019337e: add r2,r5
  00193380: strd r6,r6,[sp,#0x0]
  00193384: uxth r2,r2
  00193386: blx 0x000778d8
  0019338a: ldr r0,[r4,#0x8]
  0019338c: movs r3,#0x0
  0019338e: ldrd r1,r2,[r4,#0x54]
  00193392: add r2,r5
  00193394: uxth r2,r2
  00193396: blx 0x000778cc
  0019339a: ldr r0,[r4,#0x68]
  0019339c: str.w r8,[r0,r5,lsl #0x2]
  001933a0: adds r5,#0x1
  001933a2: ldr r0,[r4,#0x48]
  001933a4: cmp r5,r0
  001933a6: blt 0x00193374
  001933a8: movs r0,#0x1
  001933aa: strb r0,[r4,#0x4]
  001933ac: movs r0,#0x0
  001933ae: str r0,[r4,#0x60]
  001933b0: add sp,#0x8
  001933b2: pop.w r8
  001933b6: pop {r4,r5,r6,r7,pc}
```
