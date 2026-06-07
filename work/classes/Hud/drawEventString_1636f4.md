# Hud::drawEventString
elf 0x1636f4 body 164
Sig: undefined __thiscall drawEventString(Hud * this, String param_1, bool param_2)

## decompile
```c

/* Hud::drawEventString(AbyssEngine::String, bool) */

void __thiscall Hud::drawEventString(Hud *this,int param_2,int param_3)

{
  char cVar1;
  int iVar2;
  String *pSVar3;
  uint uVar4;
  int iVar5;
  int iVar6;
  
  pSVar3 = (String *)**(undefined4 **)(DAT_00173798 + 0x173708);
  uVar4 = **(uint **)(DAT_0017379c + 0x17370e);
  if (this[0x1ec] == (Hud)0x0) {
    iVar2 = *(int *)(this + 0x4e8);
    iVar5 = *(int *)(this + 0x160);
    if (param_3 == 0) {
      iVar6 = AbyssEngine::PaintCanvas::GetTextWidth(uVar4,pSVar3);
      iVar2 = (iVar2 + 3) - iVar6;
    }
    else {
      iVar2 = -3 - iVar2;
    }
    cVar1 = (char)*(undefined4 *)(this + 0x164);
    iVar2 = iVar2 + iVar5;
  }
  else {
    if (param_3 == 0) {
      iVar5 = *(int *)(this + 0x4f0);
      iVar6 = **(int **)(DAT_001737a0 + 0x173746);
      iVar2 = AbyssEngine::PaintCanvas::GetTextWidth(uVar4,pSVar3);
      iVar2 = ((iVar6 + -1) - iVar5) - iVar2;
    }
    else {
      iVar2 = *(int *)(this + 0x4f0) + 1;
    }
    cVar1 = (char)*(undefined4 *)(this + 0x164);
  }
  AbyssEngine::PaintCanvas::DrawString(uVar4,pSVar3,param_2,iVar2,(bool)(cVar1 + -1));
  return;
}

```

## target disasm
```
  001736f4: push {r4,r5,r6,r7,lr}
  001736f6: add r7,sp,#0xc
  001736f8: push {r6,r7,r8,r9,r10}
  001736fc: mov r4,r0
  001736fe: ldr r0,[0x00173798]
  00173700: mov r8,r1
  00173702: ldr r1,[0x0017379c]
  00173704: add r0,pc
  00173706: ldrb.w r3,[r4,#0x1ec]
  0017370a: add r1,pc
  0017370c: ldr r0,[r0,#0x0]
  0017370e: cmp r3,#0x0
  00173710: ldr r1,[r1,#0x0]
  00173712: ldr r5,[r0,#0x0]
  00173714: ldr r6,[r1,#0x0]
  00173716: beq 0x00173722
  00173718: cbz r2,0x00173738
  0017371a: ldr.w r0,[r4,#0x4f0]
  0017371e: adds r3,r0,#0x1
  00173720: b 0x0017375a
  00173722: ldr.w r10,[r4,#0x4e8]
  00173726: cmp r2,#0x0
  00173728: ldr.w r9,[r4,#0x160]
  0017372c: beq 0x00173768
  0017372e: mvn r0,#0x2
  00173732: sub.w r0,r0,r10
  00173736: b 0x00173778
  00173738: ldr r0,[0x001737a0]
  0017373a: mov r1,r5
  0017373c: mov r2,r8
  0017373e: ldr.w r9,[r4,#0x4f0]
  00173742: add r0,pc
  00173744: ldr r0,[r0,#0x0]
  00173746: ldr.w r10,[r0,#0x0]
  0017374a: mov r0,r6
  0017374c: blx 0x0006faa8
  00173750: sub.w r1,r10,#0x1
  00173754: sub.w r1,r1,r9
  00173758: subs r3,r1,r0
  0017375a: ldr.w r0,[r4,#0x164]
  0017375e: movs r1,#0x0
  00173760: subs r0,#0x1
  00173762: strd r0,r1,[sp,#0x0]
  00173766: b 0x00173788
  00173768: mov r0,r6
  0017376a: mov r1,r5
  0017376c: mov r2,r8
  0017376e: blx 0x0006faa8
  00173772: add.w r1,r10,#0x3
  00173776: subs r0,r1,r0
  00173778: ldr.w r1,[r4,#0x164]
  0017377c: movs r2,#0x0
  0017377e: add.w r3,r0,r9
  00173782: subs r1,#0x1
  00173784: strd r1,r2,[sp,#0x0]
  00173788: mov r0,r6
  0017378a: mov r1,r5
  0017378c: mov r2,r8
  0017378e: blx 0x0006fe14
  00173792: pop.w {r2,r3,r8,r9,r10}
  00173796: pop {r4,r5,r6,r7,pc}
```
