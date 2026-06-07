# PaintCanvas::Begin3d
elf 0x7ce44 body 142
Sig: undefined __stdcall Begin3d(void)

## decompile
```c

/* AbyssEngine::PaintCanvas::Begin3d() */

void AbyssEngine::PaintCanvas::Begin3d(void)

{
  int in_r0;
  undefined4 in_r3;
  code *pcVar1;
  float extraout_s0;
  float extraout_s1;
  float extraout_s2;
  float extraout_s3;
  undefined8 uVar2;
  
  *(undefined1 *)(*(int *)(in_r0 + 0x34) + 0xfd) = 0;
  glEnable(0xb71);
  glDepthMask(1);
  glDisable(0xbe2);
  Engine::SetColor(extraout_s0,extraout_s1,extraout_s2,extraout_s3);
  if (**(char **)(DAT_0008ced8 + 0x8ce86) == '\0') {
    pcVar1 = *(code **)(DAT_0008cedc + 0x8ce9e);
    (*pcVar1)(0x1702);
    glLoadIdentity();
    glScalef(0x3f800000,DAT_0008ced4,0x3f800000);
    (*pcVar1)(0x1701);
    glLoadMatrixf(in_r0 + 0x38);
    uVar2 = (*pcVar1)(0x1700);
    (*(code *)(DAT_001ab3e4 + 0x1ab3e8))
              ((int)uVar2,(int)((ulonglong)uVar2 >> 0x20),0x3f800000,in_r3);
    return;
  }
  (*(code *)(DAT_001ab3d4 + 0x1ab3d8))(*(undefined4 *)(in_r0 + 0x34),in_r0 + 0x38,0x3f800000,in_r3);
  return;
}

```

## target disasm
```
  0008ce44: push {r2,r3,r4,r5,r7,lr}
  0008ce46: add r7,sp,#0x10
  0008ce48: mov r4,r0
  0008ce4a: ldr r0,[r0,#0x34]
  0008ce4c: movs r1,#0x0
  0008ce4e: strb.w r1,[r0,#0xfd]
  0008ce52: movw r0,#0xb71
  0008ce56: blx 0x0006edb8
  0008ce5a: movs r0,#0x1
  0008ce5c: blx 0x0006eec0
  0008ce60: movw r0,#0xbe2
  0008ce64: blx 0x0006edc4
  0008ce68: ldr r0,[r4,#0x34]
  0008ce6a: mov.w r1,#0x3f800000
  0008ce6e: str r1,[sp,#0x0]
  0008ce70: mov.w r1,#0x3f800000
  0008ce74: mov.w r2,#0x3f800000
  0008ce78: mov.w r3,#0x3f800000
  0008ce7c: blx 0x0006fad8
  0008ce80: ldr r0,[0x0008ced8]
  0008ce82: add r0,pc
  0008ce84: ldr r0,[r0,#0x0]
  0008ce86: ldrb r0,[r0,#0x0]
  0008ce88: cbz r0,0x0008ce98
  0008ce8a: ldr r0,[r4,#0x34]
  0008ce8c: add.w r1,r4,#0x38
  0008ce90: pop.w {r2,r3,r4,r5,r7,lr}
  0008ce94: b.w 0x001ab3c8
  0008ce98: ldr r0,[0x0008cedc]
  0008ce9a: add r0,pc
  0008ce9c: ldr r5,[r0,#0x0]
  0008ce9e: movw r0,#0x1702
  0008cea2: blx r5
  0008cea4: blx 0x0006fe8c
  0008cea8: ldr r1,[0x0008ced4]
  0008ceaa: mov.w r0,#0x3f800000
  0008ceae: mov.w r2,#0x3f800000
  0008ceb2: blx 0x0006fe98
  0008ceb6: movw r0,#0x1701
  0008ceba: blx r5
  0008cebc: add.w r0,r4,#0x38
  0008cec0: blx 0x0006fea4
  0008cec4: mov.w r0,#0x1700
  0008cec8: blx r5
  0008ceca: pop.w {r2,r3,r4,r5,r7,lr}
  0008cece: b.w 0x001ab3d8
```
