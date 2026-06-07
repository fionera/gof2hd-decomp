# Sparks::Sparks
elf 0x15e38c body 200
Sig: undefined __thiscall Sparks(Sparks * this, int param_1)

## decompile
```c

/* Sparks::Sparks(int) */

Sparks * __thiscall Sparks::Sparks(Sparks *this,int param_1)

{
  undefined4 uVar1;
  uint uVar2;
  undefined4 *puVar3;
  int iVar4;
  undefined4 uVar5;
  uint uVar6;
  uint *puVar7;
  int *piVar8;
  float extraout_s0;
  float extraout_s1;
  float extraout_s2;
  float extraout_s3;
  
  uVar5 = 5;
  if (param_1 == 0) {
    uVar5 = 1;
  }
  puVar7 = *(uint **)(DAT_0016e460 + 0x16e3a0);
  uVar1 = 0x514;
  if (param_1 == 0) {
    uVar1 = 500;
  }
  *(int *)(this + 0x14) = param_1;
  *(undefined4 *)(this + 0x18) = uVar5;
  *(undefined4 *)(this + 0x1c) = uVar1;
  AbyssEngine::PaintCanvas::SpriteSystemCreate
            ((PaintCanvas *)*puVar7,(ushort)uVar5,false,(uint *)(this + 4));
  AbyssEngine::PaintCanvas::SpriteSystemSetAllUv
            ((PaintCanvas *)*puVar7,*(uint *)(this + 4),extraout_s0,extraout_s1,extraout_s2,
             extraout_s3);
  uVar6 = *(uint *)(this + 0x18);
  this[0x10] = (Sparks)0x0;
  uVar2 = (uint)((ulonglong)uVar6 * 4);
  if ((int)((ulonglong)uVar6 * 4 >> 0x20) != 0) {
    uVar2 = 0xffffffff;
  }
  puVar3 = operator_new__(uVar2);
  *(undefined4 **)this = puVar3;
  *(undefined4 *)(this + 0x20) = 0;
  if (*(int *)(this + 0x14) == 1) {
    piVar8 = *(int **)(DAT_0016e464 + 0x16e410);
    for (uVar2 = 0; uVar2 < uVar6; uVar2 = uVar2 + 1) {
      AbyssEngine::PaintCanvas::SpriteSystemSetSize
                (*puVar7,(ushort)*(undefined4 *)(this + 4),(short)uVar2);
      iVar4 = AbyssEngine::AERandom::nextInt(*piVar8);
      *(int *)(*(int *)this + uVar2 * 4) = iVar4;
      uVar6 = *(uint *)(this + 0x18);
      *(int *)(this + 0x20) = iVar4 + *(int *)(this + 0x20);
    }
  }
  else {
    *puVar3 = 0;
    *(undefined4 *)(this + 0x20) = 0;
  }
  *(undefined4 *)(this + 0xc) = 0;
  return this;
}

```

## target disasm
```
  0016e38c: push {r4,r5,r6,r7,lr}
  0016e38e: add r7,sp,#0xc
  0016e390: push {r6,r7,r8,r9,r11}
  0016e394: mov r4,r0
  0016e396: ldr r0,[0x0016e460]
  0016e398: movs r2,#0x5
  0016e39a: cmp r1,#0x0
  0016e39c: add r0,pc
  0016e39e: it eq
  0016e3a0: mov.eq r2,#0x1
  0016e3a2: cmp r1,#0x0
  0016e3a4: add.w r3,r4,#0x4
  0016e3a8: ldr.w r8,[r0,#0x0]
  0016e3ac: movw r0,#0x514
  0016e3b0: it eq
  0016e3b2: mov.eq.w r0,#0x1f4
  0016e3b6: strd r1,r2,[r4,#0x14]
  0016e3ba: str r0,[r4,#0x1c]
  0016e3bc: mov r1,r2
  0016e3be: ldr.w r0,[r8,#0x0]
  0016e3c2: movs r2,#0x0
  0016e3c4: movs r6,#0x0
  0016e3c6: blx 0x000778b4
  0016e3ca: ldr r5,[0x0016e45c]
  0016e3cc: ldr r1,[r4,#0x4]
  0016e3ce: ldr r2,[0x0016e458]
  0016e3d0: ldr.w r0,[r8,#0x0]
  0016e3d4: ldr r3,[0x0016e454]
  0016e3d6: strd r3,r5,[sp,#0x0]
  0016e3da: mov.w r3,#0x3b000000
  0016e3de: blx 0x000778c0
  0016e3e2: ldr r5,[r4,#0x18]
  0016e3e4: movs r0,#0x4
  0016e3e6: strb r6,[r4,#0x10]
  0016e3e8: umull r0,r1,r5,r0
  0016e3ec: cmp r1,#0x0
  0016e3ee: it ne
  0016e3f0: mov.ne r1,#0x1
  0016e3f2: cmp r1,#0x0
  0016e3f4: it ne
  0016e3f6: mov.ne.w r0,#0xffffffff
  0016e3fa: blx 0x0006ec08
  0016e3fe: ldr r1,[r4,#0x14]
  0016e400: str r0,[r4,#0x0]
  0016e402: cmp r1,#0x1
  0016e404: str r6,[r4,#0x20]
  0016e406: bne 0x0016e444
  0016e408: ldr r0,[0x0016e464]
  0016e40a: movs r6,#0x0
  0016e40c: add r0,pc
  0016e40e: ldr.w r9,[r0,#0x0]
  0016e412: b 0x0016e43e
  0016e414: ldr r1,[r4,#0x4]
  0016e416: uxth r2,r6
  0016e418: ldr.w r0,[r8,#0x0]
  0016e41c: movs r3,#0x1
  0016e41e: blx 0x000778cc
  0016e422: ldr.w r0,[r9,#0x0]
  0016e426: mov.w r1,#0x1f4
  0016e42a: blx 0x00071848
  0016e42e: ldr r1,[r4,#0x0]
  0016e430: str.w r0,[r1,r6,lsl #0x2]
  0016e434: adds r6,#0x1
  0016e436: ldr r1,[r4,#0x20]
  0016e438: ldr r5,[r4,#0x18]
  0016e43a: add r0,r1
  0016e43c: str r0,[r4,#0x20]
  0016e43e: cmp r6,r5
  0016e440: bcc 0x0016e414
  0016e442: b 0x0016e448
  0016e444: str r6,[r0,#0x0]
  0016e446: str r6,[r4,#0x20]
  0016e448: movs r0,#0x0
  0016e44a: str r0,[r4,#0xc]
  0016e44c: mov r0,r4
  0016e44e: pop.w {r2,r3,r8,r9,r11}
  0016e452: pop {r4,r5,r6,r7,pc}
```
