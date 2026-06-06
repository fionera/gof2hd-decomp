# Player::stopShootSound
elf 0xa3a14 body 66
Sig: undefined __thiscall stopShootSound(Player * this, int param_1, int param_2)

## decompile
```c

/* Player::stopShootSound(int, int) */

void __thiscall Player::stopShootSound(Player *this,int param_1,int param_2)

{
  undefined4 uVar1;
  undefined4 uVar2;
  
  if (8 < (uint)param_2) {
    return;
  }
  if ((1 << (param_2 & 0xffU) & 0x10cU) != 0) {
    if ((*(int *)(this + 0xd0) == 0) || (*(int *)(*(int *)(this + 0xd0) + 0x28) != 9)) {
      uVar2 = *(undefined4 *)(DAT_000b3a5c + 0xb3a4a + param_1 * 4);
      uVar1 = **(undefined4 **)(DAT_000b3a58 + 0xb3a48);
    }
    else {
      uVar2 = 0x3e;
      uVar1 = **(undefined4 **)(DAT_000b3a60 + 0xb3a3a);
    }
    (*(code *)(DAT_001abd24 + 0x1abd28))(uVar1,uVar2);
    return;
  }
  return;
}

```
## target disasm
```
  000b3a14: cmp r2,#0x8
  000b3a16: bhi 0x000b3a54
  000b3a18: movs r3,#0x1
  000b3a1a: lsl.w r2,r3,r2
  000b3a1e: tst r2,#0x10c
  000b3a22: it eq
  000b3a24: bx.eq lr
  000b3a26: ldr.w r0,[r0,#0xd0]
  000b3a2a: cbz r0,0x000b3a40
  000b3a2c: ldr r0,[r0,#0x28]
  000b3a2e: cmp r0,#0x9
  000b3a30: bne 0x000b3a40
  000b3a32: ldr r0,[0x000b3a60]
  000b3a34: movs r1,#0x3e
  000b3a36: add r0,pc
  000b3a38: ldr r0,[r0,#0x0]
  000b3a3a: ldr r0,[r0,#0x0]
  000b3a3c: b.w 0x001abd18
  000b3a40: ldr r0,[0x000b3a58]
  000b3a42: ldr r2,[0x000b3a5c]
  000b3a44: add r0,pc
  000b3a46: add r2,pc
  000b3a48: ldr r0,[r0,#0x0]
  000b3a4a: ldr.w r1,[r2,r1,lsl #0x2]
  000b3a4e: ldr r0,[r0,#0x0]
  000b3a50: b.w 0x001abd18
  000b3a54: bx lr
```
