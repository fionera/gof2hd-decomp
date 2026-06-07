# PlayerFighter::render
elf 0xdfa50 body 290
Sig: undefined __thiscall render(PlayerFighter * this)

## decompile
```c

/* PlayerFighter::render() */

void __thiscall PlayerFighter::render(PlayerFighter *this)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  uint *puVar4;
  int *piVar5;
  undefined1 local_58 [60];
  int local_1c;
  
  piVar5 = *(int **)(DAT_000efb74 + 0xefa62);
  local_1c = *piVar5;
  if (*(int *)(this + 0x78) != 0) {
    AEGeometry::render();
  }
  iVar1 = Player::isActive();
  iVar3 = *(int *)(this + 0x88);
  if (iVar1 == 0) {
    if (iVar3 != 5) goto LAB_000efb42;
  }
  else if (iVar3 - 3U < 2) {
    if (*(int *)(this + 0x124) != 0) {
      Explosion::render();
      iVar3 = *(int *)(this + 0x88);
    }
    if (iVar3 == 4) {
      if (299 < *(int *)(this + 0x1f8)) goto LAB_000efb42;
      puVar4 = *(uint **)(DAT_000efb80 + 0xefb0e);
    }
    else {
      if (iVar3 != 3) goto LAB_000efb42;
      if (*(int *)(this + 0xc) == 0) {
        if (*piVar5 == local_1c) {
          (*(code *)(DAT_001abdd4 + 0x1abdd8))();
          return;
        }
                    /* WARNING: Subroutine does not return */
        __stack_chk_fail(*(undefined4 *)(this + 8));
      }
      puVar4 = *(uint **)(DAT_000efb7c + 0xefaa2);
    }
    uVar2 = AbyssEngine::PaintCanvas::TransformGetLocal(*puVar4);
    __aeabi_memcpy(local_58,uVar2,0x3c);
    AbyssEngine::PaintCanvas::TransformSetLocal(*puVar4,*(Matrix **)(*(int *)(this + 0xc) + 0xc));
    AEGeometry::render();
    AbyssEngine::PaintCanvas::TransformSetLocal(*puVar4,*(Matrix **)(*(int *)(this + 0xc) + 0xc));
    goto LAB_000efb42;
  }
  if (*(int *)(this + 0xc) == 0) {
    AEGeometry::render();
  }
  else {
    puVar4 = *(uint **)(DAT_000efb78 + 0xefab4);
    uVar2 = AbyssEngine::PaintCanvas::TransformGetLocal(*puVar4);
    __aeabi_memcpy(local_58,uVar2,0x3c);
    AbyssEngine::PaintCanvas::TransformSetLocal(*puVar4,*(Matrix **)(*(int *)(this + 0xc) + 0xc));
    AEGeometry::render();
    AbyssEngine::PaintCanvas::TransformSetLocal(*puVar4,*(Matrix **)(*(int *)(this + 0xc) + 0xc));
  }
  if (*(int *)(this + 0x154) != 0) {
    Trail::render();
  }
LAB_000efb42:
  if (*piVar5 == local_1c) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```

## target disasm
```
  000efa50: push {r4,r5,r6,r7,lr}
  000efa52: add r7,sp,#0xc
  000efa54: push.w r8
  000efa58: sub sp,#0x40
  000efa5a: mov r4,r0
  000efa5c: ldr r0,[0x000efb74]
  000efa5e: add r0,pc
  000efa60: ldr r6,[r0,#0x0]
  000efa62: ldr r0,[r6,#0x0]
  000efa64: str r0,[sp,#0x3c]
  000efa66: ldr r0,[r4,#0x78]
  000efa68: cbz r0,0x000efa6e
  000efa6a: blx 0x00072238
  000efa6e: ldr r0,[r4,#0x4]
  000efa70: blx 0x00071f14
  000efa74: ldr.w r1,[r4,#0x88]
  000efa78: cbz r0,0x000efaa6
  000efa7a: subs r0,r1,#0x3
  000efa7c: cmp r0,#0x2
  000efa7e: bcs 0x000efaaa
  000efa80: ldr.w r0,[r4,#0x124]
  000efa84: cbz r0,0x000efa8e
  000efa86: blx 0x00072d18
  000efa8a: ldr.w r1,[r4,#0x88]
  000efa8e: cmp r1,#0x4
  000efa90: beq 0x000efafc
  000efa92: cmp r1,#0x3
  000efa94: bne 0x000efb42
  000efa96: ldr r0,[r4,#0xc]
  000efa98: cmp r0,#0x0
  000efa9a: beq 0x000efb56
  000efa9c: ldr r1,[0x000efb7c]
  000efa9e: add r1,pc
  000efaa0: ldr r5,[r1,#0x0]
  000efaa2: ldr r1,[r0,#0xc]
  000efaa4: b 0x000efb10
  000efaa6: cmp r1,#0x5
  000efaa8: bne 0x000efb42
  000efaaa: ldr r0,[r4,#0xc]
  000efaac: cbz r0,0x000efaea
  000efaae: ldr r1,[0x000efb78]
  000efab0: add r1,pc
  000efab2: ldr r5,[r1,#0x0]
  000efab4: ldr r1,[r0,#0xc]
  000efab6: ldr r0,[r5,#0x0]
  000efab8: blx 0x000720c4
  000efabc: mov r8,sp
  000efabe: mov r1,r0
  000efac0: mov r0,r8
  000efac2: movs r2,#0x3c
  000efac4: blx 0x0006f1e4
  000efac8: ldr r1,[r4,#0xc]
  000efaca: ldr r2,[r4,#0x4]
  000efacc: ldr r0,[r5,#0x0]
  000eface: ldr r1,[r1,#0xc]
  000efad0: adds r2,#0x4
  000efad2: blx 0x000721c0
  000efad6: ldr r0,[r4,#0xc]
  000efad8: blx 0x00072238
  000efadc: ldr r1,[r4,#0xc]
  000efade: mov r2,r8
  000efae0: ldr r0,[r5,#0x0]
  000efae2: ldr r1,[r1,#0xc]
  000efae4: blx 0x000721c0
  000efae8: b 0x000efaf0
  000efaea: ldr r0,[r4,#0x8]
  000efaec: blx 0x00072238
  000efaf0: ldr.w r0,[r4,#0x154]
  000efaf4: cbz r0,0x000efb42
  000efaf6: blx 0x00075958
  000efafa: b 0x000efb42
  000efafc: ldr.w r0,[r4,#0x1f8]
  000efb00: cmp.w r0,#0x12c
  000efb04: bge 0x000efb42
  000efb06: ldr r0,[0x000efb80]
  000efb08: ldr r1,[r4,#0xc]
  000efb0a: add r0,pc
  000efb0c: ldr r5,[r0,#0x0]
  000efb0e: ldr r1,[r1,#0xc]
  000efb10: ldr r0,[r5,#0x0]
  000efb12: blx 0x000720c4
  000efb16: mov r8,sp
  000efb18: mov r1,r0
  000efb1a: mov r0,r8
  000efb1c: movs r2,#0x3c
  000efb1e: blx 0x0006f1e4
  000efb22: ldr r1,[r4,#0xc]
  000efb24: ldr r2,[r4,#0x4]
  000efb26: ldr r0,[r5,#0x0]
  000efb28: ldr r1,[r1,#0xc]
  000efb2a: adds r2,#0x4
  000efb2c: blx 0x000721c0
  000efb30: ldr r0,[r4,#0xc]
  000efb32: blx 0x00072238
  000efb36: ldr r1,[r4,#0xc]
  000efb38: mov r2,r8
  000efb3a: ldr r0,[r5,#0x0]
  000efb3c: ldr r1,[r1,#0xc]
  000efb3e: blx 0x000721c0
  000efb42: ldr r0,[sp,#0x3c]
  000efb44: ldr r1,[r6,#0x0]
  000efb46: subs r0,r1,r0
  000efb48: ittt eq
  000efb4a: add.eq sp,#0x40
  000efb4c: pop.eq.w r8
  000efb50: pop.eq {r4,r5,r6,r7,pc}
  000efb52: blx 0x0006e824
  000efb56: ldr r0,[r4,#0x8]
  000efb58: ldr r1,[sp,#0x3c]
  000efb5a: ldr r2,[r6,#0x0]
  000efb5c: subs r1,r2,r1
  000efb5e: itttt eq
  000efb60: add.eq sp,#0x40
  000efb62: pop.eq.w r8
  000efb66: pop.eq.w {r4,r5,r6,r7,lr}
  000efb6a: b.eq.w 0x001abdc8
  000efb6e: blx 0x0006e824
```
