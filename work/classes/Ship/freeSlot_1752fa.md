# Ship::freeSlot
elf 0x1752fa body 74
Sig: undefined __thiscall freeSlot(Ship * this, Item * param_1, int param_2)

## decompile
```c

/* Ship::freeSlot(Item*, int) */

void __thiscall Ship::freeSlot(Ship *this,Item *param_1,int param_2)

{
  Item *this_00;
  int iVar1;
  uint uVar2;
  
  uVar2 = 0;
  do {
    if (**(uint **)(this + 0x6c) <= uVar2) {
LAB_00185336:
      (*(code *)(DAT_001abf14 + 0x1abf18))(this);
      return;
    }
    this_00 = *(Item **)((*(uint **)(this + 0x6c))[1] + uVar2 * 4);
    if (((this_00 != (Item *)0x0) && (iVar1 = Item::equals(this_00,param_1), param_2 == uVar2)) &&
       (iVar1 != 0)) {
      *(undefined4 *)(*(int *)(*(int *)(this + 0x6c) + 4) + param_2 * 4) = 0;
      goto LAB_00185336;
    }
    uVar2 = uVar2 + 1;
  } while( true );
}

```
## target disasm
```
  001852fa: push {r4,r5,r6,r7,lr}
  001852fc: add r7,sp,#0xc
  001852fe: push.w r8
  00185302: mov r8,r2
  00185304: mov r6,r1
  00185306: mov r4,r0
  00185308: movs r5,#0x0
  0018530a: b 0x00185322
  0018530c: ldr r0,[r0,#0x4]
  0018530e: ldr.w r0,[r0,r5,lsl #0x2]
  00185312: cbz r0,0x00185320
  00185314: mov r1,r6
  00185316: blx 0x000734e0
  0018531a: cmp r8,r5
  0018531c: bne 0x00185320
  0018531e: cbnz r0,0x0018532c
  00185320: adds r5,#0x1
  00185322: ldr r0,[r4,#0x6c]
  00185324: ldr r1,[r0,#0x0]
  00185326: cmp r5,r1
  00185328: bcc 0x0018530c
  0018532a: b 0x00185336
  0018532c: ldr r0,[r4,#0x6c]
  0018532e: movs r1,#0x0
  00185330: ldr r0,[r0,#0x4]
  00185332: str.w r1,[r0,r8,lsl #0x2]
  00185336: mov r0,r4
  00185338: pop.w r8
  0018533c: pop.w {r4,r5,r6,r7,lr}
  00185340: b.w 0x001abf08
```
