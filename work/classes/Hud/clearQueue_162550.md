# Hud::clearQueue
elf 0x162550 body 78
Sig: undefined __thiscall clearQueue(Hud * this)

## decompile
```c

/* Hud::clearQueue() */

void __thiscall Hud::clearQueue(Hud *this)

{
  ListItem *this_00;
  void *pvVar1;
  undefined4 *puVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  
  iVar5 = 4;
  for (uVar4 = 1; uVar4 < **(uint **)(this + 0x264); uVar4 = uVar4 + 1) {
    uVar3 = (*(uint **)(this + 0x264))[1];
    this_00 = *(ListItem **)(uVar3 + iVar5);
    if (this_00 == (ListItem *)0x0) {
      puVar2 = (undefined4 *)(uVar3 + uVar4 * 4);
    }
    else {
      pvVar1 = (void *)::ListItem::~ListItem(this_00);
      operator_delete(pvVar1);
      puVar2 = (undefined4 *)(*(int *)(*(int *)(this + 0x264) + 4) + iVar5);
    }
    *puVar2 = 0;
    iVar5 = iVar5 + 4;
  }
  *(undefined4 *)(this + 0x270) = 0;
  return;
}

```

## target disasm
```
  00172550: push {r4,r5,r6,r7,lr}
  00172552: add r7,sp,#0xc
  00172554: push.w r8
  00172558: mov r4,r0
  0017255a: movs r5,#0x1
  0017255c: movs r6,#0x4
  0017255e: mov.w r8,#0x0
  00172562: b 0x00172588
  00172564: ldr r1,[r0,#0x4]
  00172566: ldr r0,[r1,r6]
  00172568: cbz r0,0x0017257c
  0017256a: blx 0x00076138
  0017256e: blx 0x0006eb48
  00172572: ldr.w r0,[r4,#0x264]
  00172576: ldr r0,[r0,#0x4]
  00172578: add r0,r6
  0017257a: b 0x00172580
  0017257c: add.w r0,r1,r5, lsl #0x2
  00172580: str.w r8,[r0,#0x0]
  00172584: adds r6,#0x4
  00172586: adds r5,#0x1
  00172588: ldr.w r0,[r4,#0x264]
  0017258c: ldr r1,[r0,#0x0]
  0017258e: cmp r5,r1
  00172590: bcc 0x00172564
  00172592: movs r0,#0x0
  00172594: str.w r0,[r4,#0x270]
  00172598: pop.w r8
  0017259c: pop {r4,r5,r6,r7,pc}
```
