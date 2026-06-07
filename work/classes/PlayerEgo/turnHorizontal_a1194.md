# PlayerEgo::turnHorizontal
elf 0xa1194 body 30
Sig: undefined __stdcall turnHorizontal(int param_1, float param_2)

## decompile
```c

/* PlayerEgo::turnHorizontal(int, float) */

void PlayerEgo::turnHorizontal(int param_1,float param_2)

{
  float in_r2;
  
  if (in_r2 < 0.0) {
    (*(code *)(DAT_001abb94 + 0x1abb98))();
    return;
  }
  if (in_r2 == 0.0) {
    return;
  }
  (*(code *)(DAT_001abba4 + 0x1abba8))(param_1);
  return;
}

```

## target disasm
```
  000b1194: vmov s0,r2
  000b1198: vcmpe.f32 s0,#0
  000b119c: vmrs apsr,fpscr
  000b11a0: it mi
  000b11a2: b.mi.w 0x001abb88
  000b11a6: it gt
  000b11a8: b.gt.w 0x001abb98
  000b11ac: bx lr
  001abb88: bx pc
  001abb98: bx pc
```
