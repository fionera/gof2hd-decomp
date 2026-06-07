# Hud::drawPauseButton
elf 0x1637a4 body 58
Sig: undefined __stdcall drawPauseButton(void)

## decompile
```c

/* Hud::drawPauseButton() */

void Hud::drawPauseButton(void)

{
  int in_r0;
  undefined4 uVar1;
  uint *puVar2;
  
  puVar2 = *(uint **)(DAT_001737dc + 0x1737b4);
  AbyssEngine::PaintCanvas::SetColor(*puVar2);
  if ((*(byte *)(in_r0 + 0x284) & 1) == 0) {
    uVar1 = *(undefined4 *)(in_r0 + 0x2f8);
  }
  else {
    uVar1 = *(undefined4 *)(in_r0 + 0x2f4);
  }
  (*(code *)(DAT_001ac074 + 0x1ac078))
            (*puVar2,uVar1,*(undefined2 *)(in_r0 + 0x40a),*(undefined2 *)(in_r0 + 0x40c));
  return;
}

```

## target disasm
```
  001737a4: push {r4,r5,r7,lr}
  001737a6: add r7,sp,#0x8
  001737a8: mov r4,r0
  001737aa: ldr r0,[0x001737dc]
  001737ac: mov.w r1,#0xffffffff
  001737b0: add r0,pc
  001737b2: ldr r5,[r0,#0x0]
  001737b4: ldr r0,[r5,#0x0]
  001737b6: blx 0x0006fac0
  001737ba: ldrb.w r1,[r4,#0x284]
  001737be: ldr r0,[r5,#0x0]
  001737c0: ldrh.w r3,[r4,#0x40c]
  001737c4: lsls r1,r1,#0x1f
  001737c6: ldrh.w r2,[r4,#0x40a]
  001737ca: ite eq
  001737cc: ldr.eq.w r1,[r4,#0x2f8]
  001737d0: ldr.ne.w r1,[r4,#0x2f4]
  001737d4: pop.w {r4,r5,r7,lr}
  001737d8: b.w 0x001ac068
  001ac068: bx pc
```
