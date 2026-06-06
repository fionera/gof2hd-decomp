# Player::getGammaHP
elf 0xa2de2 body 14
Sig: undefined __stdcall getGammaHP(void)

## decompile
```c

/* Player::getGammaHP() */

int Player::getGammaHP(void)

{
  int in_r0;
  
  return (int)*(float *)(in_r0 + 0xb8);
}

```
## target disasm
```
  000b2de2: vldr.32 s0,[r0,#0xb8]
  000b2de6: vcvt.s32.f32 s0,s0
  000b2dea: vmov r0,s0
  000b2dee: bx lr
```
