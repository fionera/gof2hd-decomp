# Player::addGun
elf 0xa3910 body 106
Sig: undefined __thiscall addGun(Player * this, Array * param_1, int param_2)

## decompile
```c

/* Player::addGun(Array<Gun*>*, int) */

void __thiscall Player::addGun(Player *this,Array *param_1,int param_2)

{
  Array<Gun*> *this_00;
  uint uVar1;
  
  if (*(int *)this != 0) {
    if ((uint)param_2 < 4) {
      this_00 = operator_new(0xc);
      Array<Gun*>::Array(this_00);
      *(Array<Gun*> **)(*(int *)(*(int *)this + 4) + param_2 * 4) = this_00;
      for (uVar1 = 0; uVar1 < *(uint *)param_1; uVar1 = uVar1 + 1) {
        ArrayAdd<Gun*>(*(Gun **)(*(int *)(param_1 + 4) + uVar1 * 4),
                       *(Array **)(*(int *)(*(int *)this + 4) + param_2 * 4));
      }
    }
    if (this[0x70] != (Player)0x0) {
      (*(code *)(DAT_001abd04 + 0x1abd08))(this,*(undefined4 *)(this + 0x10c));
      return;
    }
  }
  return;
}

```
## target disasm
```
  000b3910: push {r4,r5,r6,r7,lr}
  000b3912: add r7,sp,#0xc
  000b3914: push {r8,r9,r11}
  000b3918: mov r4,r0
  000b391a: ldr r0,[r0,#0x0]
  000b391c: mov r9,r2
  000b391e: mov r6,r1
  000b3920: cbz r0,0x000b3972
  000b3922: cmp.w r9,#0x3
  000b3926: bhi 0x000b395a
  000b3928: movs r0,#0xc
  000b392a: blx 0x0006eb24
  000b392e: mov r8,r0
  000b3930: blx 0x00072e14
  000b3934: ldr r0,[r4,#0x0]
  000b3936: movs r5,#0x0
  000b3938: ldr r0,[r0,#0x4]
  000b393a: str.w r8,[r0,r9,lsl #0x2]
  000b393e: b 0x000b3954
  000b3940: ldr r0,[r4,#0x0]
  000b3942: ldr r1,[r6,#0x4]
  000b3944: ldr r2,[r0,#0x4]
  000b3946: ldr.w r0,[r1,r5,lsl #0x2]
  000b394a: ldr.w r1,[r2,r9,lsl #0x2]
  000b394e: blx 0x00072fa0
  000b3952: adds r5,#0x1
  000b3954: ldr r0,[r6,#0x0]
  000b3956: cmp r5,r0
  000b3958: bcc 0x000b3940
  000b395a: ldrb.w r0,[r4,#0x70]
  000b395e: cbz r0,0x000b3972
  000b3960: ldr.w r1,[r4,#0x10c]
  000b3964: mov r0,r4
  000b3966: pop.w {r8,r9,r11}
  000b396a: pop.w {r4,r5,r6,r7,lr}
  000b396e: b.w 0x001abcf8
  000b3972: pop.w {r8,r9,r11}
  000b3976: pop {r4,r5,r6,r7,pc}
  001abcf8: bx pc
```
