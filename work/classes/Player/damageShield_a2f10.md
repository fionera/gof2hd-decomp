# Player::damageShield
elf 0xa2f10 body 82
Sig: undefined __thiscall damageShield(Player * this, int param_1)

## decompile
```c

/* Player::damageShield(int) */

void __thiscall Player::damageShield(Player *this,int param_1)

{
  uint uVar1;
  byte bVar2;
  uint in_fpscr;
  float fVar3;
  float fVar4;
  
  if ((this[0xc2] != (Player)0x0) && (this[0xc0] != (Player)0x0)) {
    fVar3 = *(float *)(this + 0x88);
    uVar1 = in_fpscr & 0xfffffff | (uint)(fVar3 == 0.0) << 0x1e | (uint)(0.0 <= fVar3) << 0x1d;
    bVar2 = (byte)(uVar1 >> 0x18);
    if (!(bool)(bVar2 >> 5 & 1) || (bool)(bVar2 >> 6)) {
      *(int *)(this + 0x78) =
           *(int *)(this + 0x78) - param_1 & ~(*(int *)(this + 0x78) - param_1 >> 0x1f);
    }
    else {
      fVar4 = (float)VectorSignedToFloat(param_1,(byte)(uVar1 >> 0x16) & 3);
      fVar3 = fVar3 - fVar4;
      *(float *)(this + 0x88) = fVar3;
    }
    if ((int)((uint)(fVar3 < 0.0) << 0x1f) < 0) {
      *(undefined4 *)(this + 0x88) = 0;
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
  000b2f10: ldrb.w r2,[r0,#0xc2]
  000b2f14: cbz r2,0x000b2f3c
  000b2f16: ldrb.w r2,[r0,#0xc0]
  000b2f1a: cbz r2,0x000b2f3c
  000b2f1c: vldr.32 s0,[r0,#0x88]
  000b2f20: vcmpe.f32 s0,#0
  000b2f24: vmrs apsr,fpscr
  000b2f28: bls 0x000b2f3e
  000b2f2a: vmov s2,r1
  000b2f2e: vcvt.f32.s32 s2,s2
  000b2f32: vsub.f32 s0,s0,s2
  000b2f36: vstr.32 s0,[r0,#0x88]
  000b2f3a: b 0x000b2f48
  000b2f3c: bx lr
  000b2f3e: ldr r2,[r0,#0x78]
  000b2f40: subs r1,r2,r1
  000b2f42: bic.w r1,r1,r1, asr #0x1f
  000b2f46: str r1,[r0,#0x78]
  000b2f48: vcmpe.f32 s0,#0
  000b2f4c: vmrs apsr,fpscr
  000b2f50: itt mi
  000b2f52: mov.mi r1,#0x0
  000b2f54: str.w.mi r1,[r0,#0x88]
  000b2f58: movs r1,#0x1
  000b2f5a: strb.w r1,[r0,#0xc1]
  000b2f5e: b.w 0x001abcd8
```
