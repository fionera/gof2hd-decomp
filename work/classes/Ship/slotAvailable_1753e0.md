# Ship::slotAvailable
elf 0x1753e0 body 62
Sig: undefined __thiscall slotAvailable(Ship * this, int param_1)

## decompile
```c

/* Ship::slotAvailable(int) */

undefined4 __thiscall Ship::slotAvailable(Ship *this,int param_1)

{
  Item *this_00;
  int iVar1;
  uint uVar2;
  
  if ((param_1 != 0) && (param_1 != 0xc)) {
    for (uVar2 = 0; uVar2 < **(uint **)(this + 0x6c); uVar2 = uVar2 + 1) {
      this_00 = *(Item **)((*(uint **)(this + 0x6c))[1] + uVar2 * 4);
      if ((this_00 != (Item *)0x0) && (iVar1 = Item::getSort(this_00), iVar1 == param_1)) {
        return 0;
      }
    }
  }
  return 1;
}

```
## target disasm
```
  001853e0: push {r4,r5,r6,r7,lr}
  001853e2: add r7,sp,#0xc
  001853e4: push.w r11
  001853e8: mov r5,r0
  001853ea: mov r4,r1
  001853ec: movs r0,#0x1
  001853ee: cbz r1,0x00185418
  001853f0: cmp r4,#0xc
  001853f2: beq 0x00185418
  001853f4: movs r6,#0x0
  001853f6: b 0x0018540a
  001853f8: ldr r0,[r0,#0x4]
  001853fa: ldr.w r0,[r0,r6,lsl #0x2]
  001853fe: cbz r0,0x00185408
  00185400: blx 0x00071908
  00185404: cmp r0,r4
  00185406: beq 0x00185416
  00185408: adds r6,#0x1
  0018540a: ldr r0,[r5,#0x6c]
  0018540c: ldr r1,[r0,#0x0]
  0018540e: cmp r6,r1
  00185410: bcc 0x001853f8
  00185412: movs r0,#0x1
  00185414: b 0x00185418
  00185416: movs r0,#0x0
  00185418: pop.w r11
  0018541c: pop {r4,r5,r6,r7,pc}
```
