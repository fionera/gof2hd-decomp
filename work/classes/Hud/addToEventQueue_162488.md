# Hud::addToEventQueue
elf 0x162488 body 48
Sig: undefined __thiscall addToEventQueue(Hud * this, ListItem * param_1)

## decompile
```c

/* Hud::addToEventQueue(ListItem*) */

void __thiscall Hud::addToEventQueue(Hud *this,ListItem *param_1)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  
  uVar2 = 0;
  do {
    uVar3 = uVar2 + 1;
    if (**(uint **)(this + 0x264) <= uVar3) {
      return;
    }
    uVar4 = (*(uint **)(this + 0x264))[1];
    iVar1 = uVar2 * 4;
    uVar2 = uVar3;
  } while (*(int *)(uVar4 + iVar1 + 4) != 0);
  *(ListItem **)(uVar4 + uVar3 * 4) = param_1;
  this[0x26c] = (Hud)0x1;
  return;
}

```

## target disasm
```
  00172488: push {r4,r6,r7,lr}
  0017248a: add r7,sp,#0x8
  0017248c: ldr.w lr,[r0,#0x264]
  00172490: movs r2,#0x0
  00172492: ldr.w r12,[lr,#0x0]
  00172496: adds r3,r2,#0x1
  00172498: cmp r3,r12
  0017249a: bcs 0x001724b6
  0017249c: ldr.w r4,[lr,#0x4]
  001724a0: add.w r2,r4,r2, lsl #0x2
  001724a4: ldr r2,[r2,#0x4]
  001724a6: cmp r2,#0x0
  001724a8: mov r2,r3
  001724aa: bne 0x00172496
  001724ac: str.w r1,[r4,r3,lsl #0x2]
  001724b0: movs r1,#0x1
  001724b2: strb.w r1,[r0,#0x26c]
  001724b6: pop {r4,r6,r7,pc}
```
