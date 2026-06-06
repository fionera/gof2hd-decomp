# Player::damageHull
elf 0xa2ec6 body 62
Sig: undefined __thiscall damageHull(Player * this, int param_1)

## decompile
```c

/* Player::damageHull(int) */

void __thiscall Player::damageHull(Player *this,int param_1)

{
  uint uVar1;
  
  if ((this[0xc2] != (Player)0x0) && (this[0xc0] != (Player)0x0)) {
    uVar1 = *(uint *)(this + 0x8c);
    if ((int)uVar1 < 1) {
      *(int *)(this + 0x78) =
           *(int *)(this + 0x78) - param_1 & ~(*(int *)(this + 0x78) - param_1 >> 0x1f);
    }
    else {
      uVar1 = uVar1 - param_1;
      *(uint *)(this + 0x8c) = uVar1;
    }
    if (0x7fffffff < uVar1) {
      *(undefined4 *)(this + 0x8c) = 0;
    }
    this[0xc1] = (Player)0x1;
    (*(code *)(DAT_001abce4 + 0x1abce8))();
    return;
  }
  return;
}

```
## target disasm
```
  000b2ec6: ldrb.w r2,[r0,#0xc2]
  000b2eca: cbz r2,0x000b2ee2
  000b2ecc: ldrb.w r2,[r0,#0xc0]
  000b2ed0: cbz r2,0x000b2ee2
  000b2ed2: ldr.w r2,[r0,#0x8c]
  000b2ed6: cmp r2,#0x0
  000b2ed8: ble 0x000b2ee4
  000b2eda: subs r2,r2,r1
  000b2edc: str.w r2,[r0,#0x8c]
  000b2ee0: b 0x000b2eee
  000b2ee2: bx lr
  000b2ee4: ldr r3,[r0,#0x78]
  000b2ee6: subs r1,r3,r1
  000b2ee8: bic.w r1,r1,r1, asr #0x1f
  000b2eec: str r1,[r0,#0x78]
  000b2eee: cmp.w r2,#0xffffffff
  000b2ef2: itt le
  000b2ef4: mov.le r1,#0x0
  000b2ef6: str.w.le r1,[r0,#0x8c]
  000b2efa: movs r1,#0x1
  000b2efc: strb.w r1,[r0,#0xc1]
  000b2f00: b.w 0x001abcd8
```
