# SpaceLounge::OnRender3D
elf 0x173760 body 54
Sig: undefined __stdcall OnRender3D(void)

## decompile
```c

/* SpaceLounge::OnRender3D() */

void SpaceLounge::OnRender3D(void)

{
  int in_r0;
  int iVar1;
  
  if (*(char *)(in_r0 + 0x34) != '\0') {
    (*(code *)(DAT_001ac264 + 0x1ac268))(*(undefined4 *)(in_r0 + 4));
    return;
  }
  iVar1 = *(int *)(in_r0 + 0x44);
  if (iVar1 == 0) {
    return;
  }
  if (*(char *)(in_r0 + 0x1c) != '\0') {
    iVar1 = ListItemWindow::shows3DShip(*(ListItemWindow **)(in_r0 + 0xc));
    if (iVar1 != 0) {
      return;
    }
    iVar1 = *(int *)(in_r0 + 0x44);
  }
  (*(code *)(DAT_001ac6b4 + 0x1ac6b8))(iVar1);
  return;
}

```

## target disasm
```
  00183760: push {r4,r6,r7,lr}
  00183762: add r7,sp,#0x8
  00183764: mov r4,r0
  00183766: ldrb.w r0,[r0,#0x34]
  0018376a: cbz r0,0x00183776
  0018376c: ldr r0,[r4,#0x4]
  0018376e: pop.w {r4,r6,r7,lr}
  00183772: b.w 0x001ac258
  00183776: ldr r0,[r4,#0x44]
  00183778: cbz r0,0x00183786
  0018377a: ldrb r1,[r4,#0x1c]
  0018377c: cbz r1,0x0018378a
  0018377e: ldr r0,[r4,#0xc]
  00183780: blx 0x00077dc4
  00183784: cbz r0,0x00183788
  00183786: pop {r4,r6,r7,pc}
  00183788: ldr r0,[r4,#0x44]
  0018378a: pop.w {r4,r6,r7,lr}
  0018378e: b.w 0x001ac6a8
  001ac258: bx pc
  001ac6a8: bx pc
```
