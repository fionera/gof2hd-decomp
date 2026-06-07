# PaintCanvas::SetMatForGlow
elf 0x7d24c body 380
Sig: undefined __thiscall SetMatForGlow(PaintCanvas * this, Material * param_1)

## decompile
```c

/* AbyssEngine::PaintCanvas::SetMatForGlow(AbyssEngine::Material*) */

void __thiscall AbyssEngine::PaintCanvas::SetMatForGlow(PaintCanvas *this,Material *param_1)

{
  code *pcVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  
  iVar3 = 0x38;
  pcVar1 = *(code **)(DAT_0008d3c8 + 0x8d280);
  for (uVar2 = 0; uVar2 < *(uint *)(param_1 + 0x44); uVar2 = uVar2 + 1) {
    ArrayAdd<AbyssEngine::Mesh*>
              (*(Mesh **)(*(int *)(param_1 + 0x48) + uVar2 * 4),(Array *)(this + 0x18c));
    iVar4 = *(int *)(param_1 + 0x30) + iVar3;
    (*pcVar1)(*(undefined4 *)(iVar4 + -0x38),*(undefined4 *)(iVar4 + -0x34),
              *(undefined4 *)(iVar4 + -0x30),*(undefined4 *)(iVar4 + -0x2c),
              *(undefined4 *)(iVar4 + -0x28),*(undefined4 *)(iVar4 + -0x24),
              *(undefined4 *)(iVar4 + -0x20),*(undefined4 *)(iVar4 + -0x1c),
              *(undefined4 *)(iVar4 + -0x18),*(undefined4 *)(iVar4 + -0x14),
              *(undefined4 *)(iVar4 + -0x10),*(undefined4 *)(iVar4 + -0xc),
              *(undefined4 *)(iVar4 + -8),*(undefined4 *)(iVar4 + -4),
              *(undefined4 *)(*(int *)(param_1 + 0x30) + iVar3),this + 0x198);
    iVar4 = *(int *)(param_1 + 0x3c) + iVar3;
    (*pcVar1)(*(undefined4 *)(iVar4 + -0x38),*(undefined4 *)(iVar4 + -0x34),
              *(undefined4 *)(iVar4 + -0x30),*(undefined4 *)(iVar4 + -0x2c),
              *(undefined4 *)(iVar4 + -0x28),*(undefined4 *)(iVar4 + -0x24),
              *(undefined4 *)(iVar4 + -0x20),*(undefined4 *)(iVar4 + -0x1c),
              *(undefined4 *)(iVar4 + -0x18),*(undefined4 *)(iVar4 + -0x14),
              *(undefined4 *)(iVar4 + -0x10),*(undefined4 *)(iVar4 + -0xc),
              *(undefined4 *)(iVar4 + -8),*(undefined4 *)(iVar4 + -4),
              *(undefined4 *)(*(int *)(param_1 + 0x3c) + iVar3),this + 0x1a4);
    ArrayAdd<unsigned_int>(*(uint *)(*(int *)(param_1 + 0x54) + uVar2 * 4),(Array *)(this + 0x1b0));
    iVar4 = *(int *)(param_1 + 0x60) + iVar3;
    (*pcVar1)(*(undefined4 *)(iVar4 + -0x38),*(undefined4 *)(iVar4 + -0x34),
              *(undefined4 *)(iVar4 + -0x30),*(undefined4 *)(iVar4 + -0x2c),
              *(undefined4 *)(iVar4 + -0x28),*(undefined4 *)(iVar4 + -0x24),
              *(undefined4 *)(iVar4 + -0x20),*(undefined4 *)(iVar4 + -0x1c),
              *(undefined4 *)(iVar4 + -0x18),*(undefined4 *)(iVar4 + -0x14),
              *(undefined4 *)(iVar4 + -0x10),*(undefined4 *)(iVar4 + -0xc),
              *(undefined4 *)(iVar4 + -8),*(undefined4 *)(iVar4 + -4),
              *(undefined4 *)(*(int *)(param_1 + 0x60) + iVar3),this + 0x1bc);
    iVar3 = iVar3 + 0x3c;
  }
  return;
}

```

## target disasm
```
  0008d24c: push {r4,r5,r6,r7,lr}
  0008d24e: add r7,sp,#0xc
  0008d250: push {r8,r9,r10,r11}
  0008d254: sub sp,#0x64
  0008d256: str r1,[sp,#0x58]
  0008d258: add.w r1,r0,#0x1bc
  0008d25c: str r1,[sp,#0x44]
  0008d25e: add.w r1,r0,#0x1b0
  0008d262: str r1,[sp,#0x40]
  0008d264: add.w r1,r0,#0x1a4
  0008d268: str r1,[sp,#0x3c]
  0008d26a: add.w r1,r0,#0x198
  0008d26e: str r1,[sp,#0x38]
  0008d270: add.w r0,r0,#0x18c
  0008d274: str r0,[sp,#0x34]
  0008d276: movs r1,#0x0
  0008d278: ldr r0,[0x0008d3c8]
  0008d27a: movs r4,#0x38
  0008d27c: add r0,pc
  0008d27e: ldr r0,[r0,#0x0]
  0008d280: str r0,[sp,#0x60]
  0008d282: b 0x0008d3b6
  0008d284: ldr.w r8,[sp,#0x58]
  0008d288: str r1,[sp,#0x5c]
  0008d28a: ldr.w r0,[r8,#0x48]
  0008d28e: ldr.w r0,[r0,r1,lsl #0x2]
  0008d292: ldr r1,[sp,#0x34]
  0008d294: blx 0x0006ecbc
  0008d298: ldr.w r6,[r8,#0x30]
  0008d29c: adds r5,r6,r4
  0008d29e: sub.w lr,r5,#0x28
  0008d2a2: ldr.w r0,[r5,#-0x38]
  0008d2a6: str r0,[sp,#0x54]
  0008d2a8: ldr.w r0,[r5,#-0x34]
  0008d2ac: str r0,[sp,#0x50]
  0008d2ae: ldr.w r0,[r5,#-0x30]
  0008d2b2: str r0,[sp,#0x4c]
  0008d2b4: ldr.w r0,[r5,#-0x2c]
  0008d2b8: str r0,[sp,#0x48]
  0008d2ba: ldm.w lr,{r2,r12,lr}
  0008d2be: ldrd r9,r11,[r5,#-0x1c]
  0008d2c2: ldrd r10,r3,[r5,#-0x14]
  0008d2c6: ldrd r1,r0,[r5,#-0xc]
  0008d2ca: ldr r6,[r6,r4]
  0008d2cc: ldr.w r5,[r5,#-0x4]
  0008d2d0: str r0,[sp,#0x20]
  0008d2d2: ldr r0,[sp,#0x38]
  0008d2d4: str r5,[sp,#0x24]
  0008d2d6: str r6,[sp,#0x28]
  0008d2d8: str r0,[sp,#0x2c]
  0008d2da: stm sp,{r2,r12,lr}
  0008d2de: strd r9,r11,[sp,#0xc]
  0008d2e2: strd r10,r3,[sp,#0x14]
  0008d2e6: str r1,[sp,#0x1c]
  0008d2e8: ldrd r1,r0,[sp,#0x50]
  0008d2ec: ldrd r3,r2,[sp,#0x48]
  0008d2f0: ldr r6,[sp,#0x60]
  0008d2f2: blx r6
  0008d2f4: ldr.w r6,[r8,#0x3c]
  0008d2f8: adds r5,r6,r4
  0008d2fa: sub.w lr,r5,#0x28
  0008d2fe: sub.w r11,r5,#0x1c
  0008d302: ldr.w r0,[r5,#-0x38]
  0008d306: str r0,[sp,#0x54]
  0008d308: ldr.w r0,[r5,#-0x34]
  0008d30c: str r0,[sp,#0x50]
  0008d30e: ldr.w r0,[r5,#-0x30]
  0008d312: str r0,[sp,#0x4c]
  0008d314: ldr.w r0,[r5,#-0x2c]
  0008d318: str r0,[sp,#0x48]
  0008d31a: ldm.w lr,{r2,r12,lr}
  0008d31e: ldm.w r11,{r3,r9,r10,r11}
  0008d322: ldrd r1,r0,[r5,#-0xc]
  0008d326: ldr.w r5,[r5,#-0x4]
  0008d32a: ldr r6,[r6,r4]
  0008d32c: str r0,[sp,#0x20]
  0008d32e: ldr r0,[sp,#0x3c]
  0008d330: str r0,[sp,#0x2c]
  0008d332: add r0,sp,#0xc
  0008d334: str r5,[sp,#0x24]
  0008d336: str r6,[sp,#0x28]
  0008d338: stm sp,{r2,r12,lr}
  0008d33c: stm r0,{r3,r9,r10,r11}
  0008d340: str r1,[sp,#0x1c]
  0008d342: ldrd r1,r0,[sp,#0x50]
  0008d346: ldrd r3,r2,[sp,#0x48]
  0008d34a: ldr r6,[sp,#0x60]
  0008d34c: blx r6
  0008d34e: ldr.w r0,[r8,#0x54]
  0008d352: ldr r1,[sp,#0x5c]
  0008d354: ldr.w r0,[r0,r1,lsl #0x2]
  0008d358: ldr r1,[sp,#0x40]
  0008d35a: blx 0x0006fed4
  0008d35e: ldr.w r6,[r8,#0x60]
  0008d362: adds r5,r6,r4
  0008d364: sub.w r11,r5,#0x1c
  0008d368: ldr.w r0,[r5,#-0x38]
  0008d36c: str r0,[sp,#0x54]
  0008d36e: ldr.w r0,[r5,#-0x34]
  0008d372: str r0,[sp,#0x50]
  0008d374: ldr.w r0,[r5,#-0x30]
  0008d378: str r0,[sp,#0x4c]
  0008d37a: ldrd r3,r2,[r5,#-0x2c]
  0008d37e: ldrd r12,lr,[r5,#-0x24]
  0008d382: ldm.w r11,{r8,r9,r10,r11}
  0008d386: ldrd r1,r0,[r5,#-0xc]
  0008d38a: ldr.w r5,[r5,#-0x4]
  0008d38e: ldr r6,[r6,r4]
  0008d390: str r0,[sp,#0x20]
  0008d392: ldr r0,[sp,#0x44]
  0008d394: str r0,[sp,#0x2c]
  0008d396: add r0,sp,#0xc
  0008d398: str r5,[sp,#0x24]
  0008d39a: str r6,[sp,#0x28]
  0008d39c: stm sp,{r2,r12,lr}
  0008d3a0: stm r0,{r8,r9,r10,r11}
  0008d3a4: str r1,[sp,#0x1c]
  0008d3a6: ldr r0,[sp,#0x54]
  0008d3a8: ldrd r2,r1,[sp,#0x4c]
  0008d3ac: ldr r6,[sp,#0x60]
  0008d3ae: blx r6
  0008d3b0: ldr r1,[sp,#0x5c]
  0008d3b2: adds r4,#0x3c
  0008d3b4: adds r1,#0x1
  0008d3b6: ldr r0,[sp,#0x58]
  0008d3b8: ldr r0,[r0,#0x44]
  0008d3ba: cmp r1,r0
  0008d3bc: bcc.w 0x0008d284
  0008d3c0: add sp,#0x64
  0008d3c2: pop.w {r8,r9,r10,r11}
  0008d3c6: pop {r4,r5,r6,r7,pc}
```
