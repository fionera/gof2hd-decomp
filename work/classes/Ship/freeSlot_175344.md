# Ship::freeSlot
elf 0x175344 body 68
Sig: undefined __thiscall freeSlot(Ship * this, Item * param_1)

## decompile
```c

/* Ship::freeSlot(Item*) */

void __thiscall Ship::freeSlot(Ship *this,Item *param_1)

{
  Item *this_00;
  int iVar1;
  uint uVar2;
  
  uVar2 = 0;
  do {
    if (**(uint **)(this + 0x6c) <= uVar2) {
LAB_0018537a:
      (*(code *)(DAT_001abf14 + 0x1abf18))(this);
      return;
    }
    this_00 = *(Item **)((*(uint **)(this + 0x6c))[1] + uVar2 * 4);
    if ((this_00 != (Item *)0x0) && (iVar1 = Item::equals(this_00,param_1), iVar1 != 0)) {
      *(undefined4 *)(*(int *)(*(int *)(this + 0x6c) + 4) + uVar2 * 4) = 0;
      goto LAB_0018537a;
    }
    uVar2 = uVar2 + 1;
  } while( true );
}

```
## target disasm
```
  00185344: push {r4,r5,r6,r7,lr}
  00185346: add r7,sp,#0xc
  00185348: push.w r11
  0018534c: mov r5,r1
  0018534e: mov r4,r0
  00185350: movs r6,#0x0
  00185352: b 0x00185366
  00185354: ldr r0,[r0,#0x4]
  00185356: ldr.w r0,[r0,r6,lsl #0x2]
  0018535a: cbz r0,0x00185364
  0018535c: mov r1,r5
  0018535e: blx 0x000734e0
  00185362: cbnz r0,0x00185370
  00185364: adds r6,#0x1
  00185366: ldr r0,[r4,#0x6c]
  00185368: ldr r1,[r0,#0x0]
  0018536a: cmp r6,r1
  0018536c: bcc 0x00185354
  0018536e: b 0x0018537a
  00185370: ldr r0,[r4,#0x6c]
  00185372: movs r1,#0x0
  00185374: ldr r0,[r0,#0x4]
  00185376: str.w r1,[r0,r6,lsl #0x2]
  0018537a: mov r0,r4
  0018537c: pop.w r11
  00185380: pop.w {r4,r5,r6,r7,lr}
  00185384: b.w 0x001abf08
```
