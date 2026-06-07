# TouchSlider::touchedInside
elf 0xa2448 body 76
Sig: undefined __thiscall touchedInside(TouchSlider * this, int param_1, int param_2)

## decompile
```c

/* TouchSlider::touchedInside(int, int) */

undefined4 __thiscall TouchSlider::touchedInside(TouchSlider *this,int param_1,int param_2)

{
  undefined4 uVar1;
  int iVar2;
  
  iVar2 = *(int *)(this + 0x38);
  if (((*(int *)(this + 8) - (*(int *)(this + 0x14) >> 1)) - iVar2 <= param_1) &&
     (param_1 < (*(int *)(this + 0x14) >> 1) + *(int *)(this + 8) + iVar2)) {
    uVar1 = 0;
    if (((*(int *)(this + 0xc) - iVar2) - (*(int *)(this + 0x18) >> 1) <= param_2) &&
       (param_2 <= (*(int *)(this + 0x18) >> 1) + *(int *)(this + 0xc) + iVar2)) {
      uVar1 = 1;
    }
    return uVar1;
  }
  return 0;
}

```

## target disasm
```
  000b2448: push {r4,r6,r7,lr}
  000b244a: add r7,sp,#0x8
  000b244c: ldr r3,[r0,#0x14]
  000b244e: ldr.w r12,[r0,#0x8]
  000b2452: ldr.w lr,[r0,#0x38]
  000b2456: sub.w r4,r12,r3, asr #0x1
  000b245a: sub.w r4,r4,lr
  000b245e: cmp r4,r1
  000b2460: bgt 0x000b2490
  000b2462: asrs r3,r3,#0x1
  000b2464: add r3,r12
  000b2466: add r3,lr
  000b2468: cmp r3,r1
  000b246a: ble 0x000b2490
  000b246c: ldr.w r12,[r0,#0xc]
  000b2470: ldr r1,[r0,#0x18]
  000b2472: sub.w r0,r12,lr
  000b2476: sub.w r3,r0,r1, asr #0x1
  000b247a: movs r0,#0x0
  000b247c: cmp r3,r2
  000b247e: bgt 0x000b248e
  000b2480: asrs r1,r1,#0x1
  000b2482: add.w r3,r12,lr
  000b2486: add r1,r3
  000b2488: cmp r1,r2
  000b248a: it ge
  000b248c: mov.ge r0,#0x1
  000b248e: pop {r4,r6,r7,pc}
  000b2490: movs r0,#0x0
  000b2492: pop {r4,r6,r7,pc}
```
