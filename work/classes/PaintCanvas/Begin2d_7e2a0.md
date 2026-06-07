# PaintCanvas::Begin2d
elf 0x7e2a0 body 210
Sig: undefined __stdcall Begin2d(void)

## decompile
```c

/* AbyssEngine::PaintCanvas::Begin2d() */

void AbyssEngine::PaintCanvas::Begin2d(void)

{
  int in_r0;
  float extraout_s0;
  float extraout_s1;
  float extraout_s2;
  float extraout_s3;
  
  *(undefined1 *)(*(int *)(in_r0 + 0x34) + 0xfd) = 1;
  glDisable(0xb71);
  glDepthMask(0);
  glEnable(0xbe2);
  glBlendFunc(0x302,0x303);
  Engine::SetColor(extraout_s0,extraout_s1,extraout_s2,extraout_s3);
  Engine::GlEnable(*(uint *)(in_r0 + 0x34),true);
  if (**(char **)(DAT_0008e374 + 0x8e2fa) == '\0') {
    glTexEnvi(0x2300,0x2200,0x2100);
    glMatrixMode(0x1702);
    glLoadIdentity();
    glScalef(0x3f800000,0x3f800000,0x3f800000);
    glMatrixMode(0x1701);
    glLoadMatrixf(in_r0 + 0x78);
    if (*(int *)(in_r0 + 0x30) != 2) {
      glMultMatrixf(in_r0 + 0xb8);
    }
    glMatrixMode(0x1700);
    glLoadIdentity();
  }
  else {
    Engine::SetOrthoMatrix
              (*(Engine **)(in_r0 + 0x34),(float *)(in_r0 + 0x78),(float *)(in_r0 + 0xb8),
               *(int *)(in_r0 + 0x30) != 2);
  }
  *(undefined4 *)(in_r0 + 0xc) = 0;
  return;
}

```

## target disasm
```
  0008e2a0: push {r2,r3,r4,r6,r7,lr}
  0008e2a2: add r7,sp,#0x10
  0008e2a4: mov r4,r0
  0008e2a6: ldr r0,[r0,#0x34]
  0008e2a8: movs r1,#0x1
  0008e2aa: strb.w r1,[r0,#0xfd]
  0008e2ae: movw r0,#0xb71
  0008e2b2: blx 0x0006edc4
  0008e2b6: movs r0,#0x0
  0008e2b8: blx 0x0006eec0
  0008e2bc: movw r0,#0xbe2
  0008e2c0: blx 0x0006edb8
  0008e2c4: movw r0,#0x302
  0008e2c8: movw r1,#0x303
  0008e2cc: blx 0x0006fe68
  0008e2d0: ldr r0,[r4,#0x34]
  0008e2d2: mov.w r1,#0x3f800000
  0008e2d6: str r1,[sp,#0x0]
  0008e2d8: mov.w r1,#0x3f800000
  0008e2dc: mov.w r2,#0x3f800000
  0008e2e0: mov.w r3,#0x3f800000
  0008e2e4: blx 0x0006fad8
  0008e2e8: ldr r0,[r4,#0x34]
  0008e2ea: movw r1,#0xde1
  0008e2ee: movs r2,#0x1
  0008e2f0: blx 0x0006eddc
  0008e2f4: ldr r0,[0x0008e374]
  0008e2f6: add r0,pc
  0008e2f8: ldr r0,[r0,#0x0]
  0008e2fa: ldrb r0,[r0,#0x0]
  0008e2fc: cbz r0,0x0008e316
  0008e2fe: ldrd r1,r0,[r4,#0x30]
  0008e302: add.w r2,r4,#0xb8
  0008e306: subs r3,r1,#0x2
  0008e308: add.w r1,r4,#0x78
  0008e30c: it ne
  0008e30e: mov.ne r3,#0x1
  0008e310: blx 0x0006ff58
  0008e314: b 0x0008e36c
  0008e316: mov.w r0,#0x2300
  0008e31a: mov.w r1,#0x2200
  0008e31e: mov.w r2,#0x2100
  0008e322: blx 0x0006f268
  0008e326: movw r0,#0x1702
  0008e32a: blx 0x0006ff64
  0008e32e: blx 0x0006fe8c
  0008e332: mov.w r0,#0x3f800000
  0008e336: mov.w r1,#0x3f800000
  0008e33a: mov.w r2,#0x3f800000
  0008e33e: blx 0x0006fe98
  0008e342: movw r0,#0x1701
  0008e346: blx 0x0006ff64
  0008e34a: add.w r0,r4,#0x78
  0008e34e: blx 0x0006fea4
  0008e352: ldr r0,[r4,#0x30]
  0008e354: cmp r0,#0x2
  0008e356: itt ne
  0008e358: add.ne.w r0,r4,#0xb8
  0008e35c: blx.ne 0x0006ff70
  0008e360: mov.w r0,#0x1700
  0008e364: blx 0x0006ff64
  0008e368: blx 0x0006fe8c
  0008e36c: movs r0,#0x0
  0008e36e: str r0,[r4,#0xc]
  0008e370: pop {r2,r3,r4,r6,r7,pc}
```
