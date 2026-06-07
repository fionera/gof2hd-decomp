# PaintCanvas::BeginBG
elf 0x7e1e0 body 160
Sig: undefined __stdcall BeginBG(void)

## decompile
```c

/* AbyssEngine::PaintCanvas::BeginBG() */

void AbyssEngine::PaintCanvas::BeginBG(void)

{
  char cVar1;
  int in_r0;
  undefined1 *puVar2;
  char *pcVar3;
  undefined4 in_r3;
  code *pcVar4;
  float extraout_s0;
  float extraout_s1;
  float extraout_s2;
  float extraout_s3;
  undefined8 uVar5;
  
  *(undefined1 *)(*(int *)(in_r0 + 0x34) + 0xfd) = 0;
  glDisable(0xb71);
  glDepthMask(0);
  glDisable(0xbe2);
  Engine::SetColor(extraout_s0,extraout_s1,extraout_s2,extraout_s3);
  puVar2 = *(undefined1 **)(DAT_0008e280 + 0x8e224);
  pcVar3 = *(char **)(DAT_0008e284 + 0x8e226);
  *(undefined1 *)(in_r0 + 0x1f0) = *puVar2;
  cVar1 = *pcVar3;
  *puVar2 = 0;
  if (cVar1 == '\0') {
    pcVar4 = *(code **)(DAT_0008e288 + 0x8e248);
    (*pcVar4)(0x1702);
    glLoadIdentity();
    glScalef(0x3f800000,DAT_0008e27c,0x3f800000);
    (*pcVar4)(0x1701);
    glLoadMatrixf(in_r0 + 0x38);
    uVar5 = (*pcVar4)(0x1700);
    (*(code *)(DAT_001ab3e4 + 0x1ab3e8))
              ((int)uVar5,(int)((ulonglong)uVar5 >> 0x20),0x3f800000,in_r3);
    return;
  }
  (*(code *)(DAT_001ab3d4 + 0x1ab3d8))(*(undefined4 *)(in_r0 + 0x34),in_r0 + 0x38,0x3f800000,in_r3);
  return;
}

```

## target disasm
```
  0008e1e0: push {r2,r3,r4,r5,r7,lr}
  0008e1e2: add r7,sp,#0x10
  0008e1e4: mov r4,r0
  0008e1e6: ldr r0,[r0,#0x34]
  0008e1e8: movs r5,#0x0
  0008e1ea: strb.w r5,[r0,#0xfd]
  0008e1ee: movw r0,#0xb71
  0008e1f2: blx 0x0006edc4
  0008e1f6: movs r0,#0x0
  0008e1f8: blx 0x0006eec0
  0008e1fc: movw r0,#0xbe2
  0008e200: blx 0x0006edc4
  0008e204: ldr r0,[r4,#0x34]
  0008e206: mov.w r1,#0x3f800000
  0008e20a: str r1,[sp,#0x0]
  0008e20c: mov.w r1,#0x3f800000
  0008e210: mov.w r2,#0x3f800000
  0008e214: mov.w r3,#0x3f800000
  0008e218: blx 0x0006fad8
  0008e21c: ldr r0,[0x0008e280]
  0008e21e: ldr r1,[0x0008e284]
  0008e220: add r0,pc
  0008e222: add r1,pc
  0008e224: ldr r0,[r0,#0x0]
  0008e226: ldr r1,[r1,#0x0]
  0008e228: ldrb r2,[r0,#0x0]
  0008e22a: strb.w r2,[r4,#0x1f0]
  0008e22e: ldrb r1,[r1,#0x0]
  0008e230: strb r5,[r0,#0x0]
  0008e232: cbz r1,0x0008e242
  0008e234: ldr r0,[r4,#0x34]
  0008e236: add.w r1,r4,#0x38
  0008e23a: pop.w {r2,r3,r4,r5,r7,lr}
  0008e23e: b.w 0x001ab3c8
  0008e242: ldr r0,[0x0008e288]
  0008e244: add r0,pc
  0008e246: ldr r5,[r0,#0x0]
  0008e248: movw r0,#0x1702
  0008e24c: blx r5
  0008e24e: blx 0x0006fe8c
  0008e252: ldr r1,[0x0008e27c]
  0008e254: mov.w r0,#0x3f800000
  0008e258: mov.w r2,#0x3f800000
  0008e25c: blx 0x0006fe98
  0008e260: movw r0,#0x1701
  0008e264: blx r5
  0008e266: add.w r0,r4,#0x38
  0008e26a: blx 0x0006fea4
  0008e26e: mov.w r0,#0x1700
  0008e272: blx r5
  0008e274: pop.w {r2,r3,r4,r5,r7,lr}
  0008e278: b.w 0x001ab3d8
  001ab3c8: bx pc
  001ab3d8: bx pc
```
