# Hud::updateQueue
elf 0x1624dc body 116
Sig: undefined __thiscall updateQueue(Hud * this, int param_1)

## decompile
```c

/* Hud::updateQueue(int) */

void __thiscall Hud::updateQueue(Hud *this,int param_1)

{
  int iVar1;
  void *pvVar2;
  uint uVar3;
  undefined4 uVar4;
  undefined4 *puVar5;
  uint uVar6;
  
  iVar1 = *(int *)(this + 0x268) + param_1;
  *(int *)(this + 0x268) = iVar1;
  if (iVar1 < 0xfa1) {
    if (iVar1 < 0x7d1) {
      return;
    }
    if (*(int *)(this + 0x270) != 0) {
      return;
    }
    uVar4 = 1;
  }
  else {
    *(undefined4 *)(this + 0x268) = 0;
    puVar5 = *(undefined4 **)(*(int *)(this + 0x264) + 4);
    if ((ListItem *)*puVar5 != (ListItem *)0x0) {
      pvVar2 = (void *)::ListItem::~ListItem((ListItem *)*puVar5);
      operator_delete(pvVar2);
      puVar5 = *(undefined4 **)(*(int *)(this + 0x264) + 4);
    }
    *puVar5 = 0;
    uVar6 = 0;
    while( true ) {
      uVar3 = (*(uint **)(this + 0x264))[1];
      if (**(uint **)(this + 0x264) <= uVar6 + 1) break;
      *(undefined4 *)(uVar3 + uVar6 * 4) = *(undefined4 *)(uVar3 + uVar6 * 4 + 4);
      uVar6 = uVar6 + 1;
    }
    if (*(int *)(uVar3 + 4) == 0) {
      this[0x26c] = (Hud)0x0;
    }
    uVar4 = 0;
  }
  *(undefined4 *)(this + 0x270) = uVar4;
  return;
}

```

## target disasm
```
  001724dc: push {r4,r5,r7,lr}
  001724de: add r7,sp,#0x8
  001724e0: mov r4,r0
  001724e2: ldr.w r0,[r0,#0x268]
  001724e6: add r0,r1
  001724e8: str.w r0,[r4,#0x268]
  001724ec: cmp.w r0,#0xfa0
  001724f0: ble 0x0017253c
  001724f2: ldr.w r0,[r4,#0x264]
  001724f6: movs r5,#0x0
  001724f8: str.w r5,[r4,#0x268]
  001724fc: ldr r1,[r0,#0x4]
  001724fe: ldr r0,[r1,#0x0]
  00172500: cbz r0,0x00172510
  00172502: blx 0x00076138
  00172506: blx 0x0006eb48
  0017250a: ldr.w r0,[r4,#0x264]
  0017250e: ldr r1,[r0,#0x4]
  00172510: str r5,[r1,#0x0]
  00172512: b 0x00172520
  00172514: add.w r2,r0,r5, lsl #0x2
  00172518: ldr r2,[r2,#0x4]
  0017251a: str.w r2,[r0,r5,lsl #0x2]
  0017251e: mov r5,r1
  00172520: ldr.w r0,[r4,#0x264]
  00172524: adds r1,r5,#0x1
  00172526: ldrd r2,r0,[r0,#0x0]
  0017252a: cmp r1,r2
  0017252c: bcc 0x00172514
  0017252e: ldr r0,[r0,#0x4]
  00172530: cbnz r0,0x00172538
  00172532: movs r0,#0x0
  00172534: strb.w r0,[r4,#0x26c]
  00172538: movs r0,#0x0
  0017253a: b 0x0017254a
  0017253c: cmp.w r0,#0x7d0
  00172540: ble 0x0017254e
  00172542: ldr.w r0,[r4,#0x270]
  00172546: cbnz r0,0x0017254e
  00172548: movs r0,#0x1
  0017254a: str.w r0,[r4,#0x270]
  0017254e: pop {r4,r5,r7,pc}
```
