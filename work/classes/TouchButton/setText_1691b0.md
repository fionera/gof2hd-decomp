# TouchButton::setText
elf 0x1691b0 body 244
Sig: undefined __stdcall setText(String * param_1)

## decompile
```c

/* TouchButton::setText(AbyssEngine::String const&) */

void TouchButton::setText(String *param_1)

{
  int iVar1;
  String *in_r1;
  int iVar2;
  int iVar3;
  uint *puVar4;
  
  AbyssEngine::String::operator=((String *)(param_1 + 0xc),in_r1);
  puVar4 = *(uint **)(DAT_001792a4 + 0x1791cc);
  iVar1 = AbyssEngine::PaintCanvas::GetTextWidth(*puVar4,*(String **)(param_1 + 8));
  if (*(int *)(param_1 + 0x24) != -1) {
    iVar1 = AbyssEngine::PaintCanvas::GetImage2DWidth(*puVar4);
  }
  iVar2 = *(int *)(param_1 + 0x94);
  iVar3 = iVar1;
  if (0 < *(int *)(param_1 + 0x6c)) {
    iVar3 = (*(int *)(param_1 + 0x6c) - iVar2) - *(int *)(param_1 + 0x9c);
  }
  *(int *)(param_1 + 0xa0) = iVar3;
  iVar3 = *(int *)(param_1 + 0x9c) + iVar3 + iVar2;
  *(int *)(param_1 + 0x90) = iVar3;
  if ((param_1[0x75] & 2) == 0) {
    if ((param_1[0x75] & 1) != 0) {
      *(int *)(param_1 + 0xa4) = iVar2;
      goto LAB_00179236;
    }
    iVar3 = (iVar3 - iVar1) / 2;
    *(int *)(param_1 + 0xa4) = iVar3;
    if (*(int *)(param_1 + 0x70) == 6) {
      iVar3 = iVar3 + -5;
    }
    else {
      if (*(int *)(param_1 + 0x70) != 5) goto LAB_00179236;
      iVar3 = iVar3 + 5;
    }
  }
  else {
    iVar3 = iVar3 - (iVar1 + iVar2);
  }
  *(int *)(param_1 + 0xa4) = iVar3;
LAB_00179236:
  iVar3 = *(int *)(param_1 + 0x88);
  iVar1 = AbyssEngine::PaintCanvas::GetTextHeight(*puVar4);
  iVar1 = (iVar3 - iVar1) / 2;
  *(int *)(param_1 + 0xa8) = iVar1;
  if (*(int *)(param_1 + 0x70) == 3) {
    *(int *)(param_1 + 0xa8) = iVar1 + 2;
  }
  if (*(int *)(param_1 + 0x24) != -1) {
    iVar3 = *(int *)(param_1 + 0x88);
    iVar1 = AbyssEngine::PaintCanvas::GetImage2DHeight(*puVar4);
    *(int *)(param_1 + 0xa8) = (iVar3 - iVar1) / 2;
    if (*(int *)(param_1 + 0x70) == 1) {
      *(int *)(param_1 + 0xa4) = *(int *)(param_1 + 0xa4) + 3;
    }
  }
  (*(code *)(DAT_001ac464 + 0x1ac468))
            (param_1,*(undefined4 *)(param_1 + 0x80),*(undefined4 *)(param_1 + 0x84),param_1[0x74]);
  return;
}

```

## target disasm
```
  001791b0: push {r4,r5,r6,r7,lr}
  001791b2: add r7,sp,#0xc
  001791b4: push.w r11
  001791b8: mov r4,r0
  001791ba: adds r0,#0xc
  001791bc: mov r5,r1
  001791be: blx 0x0006f2b0
  001791c2: ldr r0,[0x001792a4]
  001791c4: mov r2,r5
  001791c6: ldr r1,[r4,#0x8]
  001791c8: add r0,pc
  001791ca: ldr r6,[r0,#0x0]
  001791cc: ldr r0,[r6,#0x0]
  001791ce: blx 0x0006faa8
  001791d2: ldr r1,[r4,#0x24]
  001791d4: adds r2,r1,#0x1
  001791d6: itt ne
  001791d8: ldr.ne r0,[r6,#0x0]
  001791da: blx.ne 0x00072d84
  001791de: ldr.w r2,[r4,#0x9c]
  001791e2: ldr.w r1,[r4,#0x94]
  001791e6: ldr r3,[r4,#0x6c]
  001791e8: cmp r3,#0x1
  001791ea: itee lt
  001791ec: mov.lt r3,r0
  001791ee: sub.ge r3,r3,r1
  001791f0: sub.ge r3,r3,r2
  001791f2: str.w r3,[r4,#0xa0]
  001791f6: adds r5,r3,r1
  001791f8: ldrb.w r3,[r4,#0x75]
  001791fc: add r2,r5
  001791fe: str.w r2,[r4,#0x90]
  00179202: tst r3,#0x2
  00179206: bne 0x00179228
  00179208: lsls r3,r3,#0x1f
  0017920a: bne 0x00179232
  0017920c: subs r0,r2,r0
  0017920e: ldr r1,[r4,#0x70]
  00179210: add.w r0,r0,r0, lsr #0x1f
  00179214: cmp r1,#0x6
  00179216: asr.w r0,r0, asr #0x1
  0017921a: str.w r0,[r4,#0xa4]
  0017921e: beq 0x001792a0
  00179220: cmp r1,#0x5
  00179222: bne 0x00179236
  00179224: adds r0,#0x5
  00179226: b 0x0017922c
  00179228: add r0,r1
  0017922a: subs r0,r2,r0
  0017922c: str.w r0,[r4,#0xa4]
  00179230: b 0x00179236
  00179232: str.w r1,[r4,#0xa4]
  00179236: ldr r1,[r4,#0x8]
  00179238: ldr r0,[r6,#0x0]
  0017923a: ldr.w r5,[r4,#0x88]
  0017923e: blx 0x0006fe20
  00179242: subs r0,r5,r0
  00179244: ldr r1,[r4,#0x70]
  00179246: add.w r0,r0,r0, lsr #0x1f
  0017924a: cmp r1,#0x3
  0017924c: asr.w r0,r0, asr #0x1
  00179250: str.w r0,[r4,#0xa8]
  00179254: itt eq
  00179256: add.eq r0,#0x2
  00179258: str.w.eq r0,[r4,#0xa8]
  0017925c: ldr r1,[r4,#0x24]
  0017925e: adds r0,r1,#0x1
  00179260: beq 0x0017928a
  00179262: ldr r0,[r6,#0x0]
  00179264: ldr.w r5,[r4,#0x88]
  00179268: blx 0x00072d90
  0017926c: subs r0,r5,r0
  0017926e: ldr r1,[r4,#0x70]
  00179270: add.w r0,r0,r0, lsr #0x1f
  00179274: cmp r1,#0x1
  00179276: asr.w r0,r0, asr #0x1
  0017927a: str.w r0,[r4,#0xa8]
  0017927e: ittt eq
  00179280: ldr.eq.w r0,[r4,#0xa4]
  00179284: add.eq r0,#0x3
  00179286: str.w.eq r0,[r4,#0xa4]
  0017928a: ldrb.w r3,[r4,#0x74]
  0017928e: mov r0,r4
  00179290: ldrd r1,r2,[r4,#0x80]
  00179294: pop.w r11
  00179298: pop.w {r4,r5,r6,r7,lr}
  0017929c: b.w 0x001ac458
  001792a0: subs r0,#0x5
  001792a2: b 0x0017922c
```
