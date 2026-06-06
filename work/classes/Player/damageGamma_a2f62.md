# Player::damageGamma
elf 0xa2f62 body 52
Sig: undefined __thiscall damageGamma(Player * this, float param_1)

## decompile
```c

/* Player::damageGamma(float) */

float __thiscall Player::damageGamma(Player *this,float param_1)

{
  float in_r1;
  
  if ((this[0xc2] != (Player)0x0) && (this[0xc0] != (Player)0x0)) {
    param_1 = *(float *)(this + 0xb8) - in_r1;
    this[0x67] = (Player)0x1;
    *(float *)(this + 0xb8) = param_1;
    if (0.0 > param_1 || param_1 == 0.0) {
      *(uint *)(this + 0xb8) = (uint)(0.0 <= param_1 && param_1 != 0.0);
    }
  }
  return param_1;
}

```
## target disasm
```
  000b2f62: ldrb.w r2,[r0,#0xc2]
  000b2f66: cbz r2,0x000b2f94
  000b2f68: ldrb.w r2,[r0,#0xc0]
  000b2f6c: cbz r2,0x000b2f94
  000b2f6e: vmov s0,r1
  000b2f72: vldr.32 s2,[r0,#0xb8]
  000b2f76: movs r1,#0x1
  000b2f78: vsub.f32 s0,s2,s0
  000b2f7c: strb.w r1,[r0,#0x67]
  000b2f80: vcmpe.f32 s0,#0
  000b2f84: vstr.32 s0,[r0,#0xb8]
  000b2f88: vmrs apsr,fpscr
  000b2f8c: itt ls
  000b2f8e: mov.ls r1,#0x0
  000b2f90: str.w.ls r1,[r0,#0xb8]
  000b2f94: bx lr
```
