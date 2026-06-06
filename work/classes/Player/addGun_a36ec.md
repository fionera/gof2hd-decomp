# Player::addGun
elf 0xa36ec body 86
Sig: undefined __thiscall addGun(Player * this, Gun * param_1, int param_2)

## decompile
```c

/* Player::addGun(Gun*, int) */

void __thiscall Player::addGun(Player *this,Gun *param_1,int param_2)

{
  Array<Gun*> *this_00;
  
  if (*(int *)this != 0) {
    if ((uint)param_2 < 4) {
      this_00 = operator_new(0xc);
      Array<Gun*>::Array(this_00);
      *(Array<Gun*> **)(*(int *)(*(int *)this + 4) + param_2 * 4) = this_00;
      ArrayAdd<Gun*>(param_1,*(Array **)(*(int *)(*(int *)this + 4) + param_2 * 4));
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
  000b36ec: push {r4,r5,r6,r7,lr}
  000b36ee: add r7,sp,#0xc
  000b36f0: push.w r8
  000b36f4: mov r4,r0
  000b36f6: ldr r0,[r0,#0x0]
  000b36f8: mov r6,r2
  000b36fa: mov r8,r1
  000b36fc: cbz r0,0x000b373c
  000b36fe: cmp r6,#0x3
  000b3700: bhi 0x000b3724
  000b3702: movs r0,#0xc
  000b3704: blx 0x0006eb24
  000b3708: mov r5,r0
  000b370a: blx 0x00072e14
  000b370e: ldr r0,[r4,#0x0]
  000b3710: ldr r0,[r0,#0x4]
  000b3712: str.w r5,[r0,r6,lsl #0x2]
  000b3716: ldr r0,[r4,#0x0]
  000b3718: ldr r0,[r0,#0x4]
  000b371a: ldr.w r1,[r0,r6,lsl #0x2]
  000b371e: mov r0,r8
  000b3720: blx 0x00072fa0
  000b3724: ldrb.w r0,[r4,#0x70]
  000b3728: cbz r0,0x000b373c
  000b372a: ldr.w r1,[r4,#0x10c]
  000b372e: mov r0,r4
  000b3730: pop.w r8
  000b3734: pop.w {r4,r5,r6,r7,lr}
  000b3738: b.w 0x001abcf8
  000b373c: pop.w r8
  000b3740: pop {r4,r5,r6,r7,pc}
```
