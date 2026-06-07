# PlayerEgo::turnVertical
elf 0xa1168 body 38
Sig: undefined __stdcall turnVertical(int param_1, float param_2)

## decompile
```c

/* PlayerEgo::turnVertical(int, float) */

void PlayerEgo::turnVertical(int param_1,float param_2)

{
  float in_r2;
  
  if ((int)((uint)(in_r2 < DAT_000b1190) << 0x1f) < 0) {
    (*(code *)(DAT_001abb74 + 0x1abb78))();
    return;
  }
  if (in_r2 <= 0.0) {
    return;
  }
  (*(code *)(DAT_001abb84 + 0x1abb88))(param_1);
  return;
}

```

## target disasm
```
  000b1168: vldr.32 s2,[pc,#0x24]
  000b116c: vmov s0,r2
  000b1170: vcmpe.f32 s0,s2
  000b1174: vmrs apsr,fpscr
  000b1178: it mi
  000b117a: b.mi.w 0x001abb68
  000b117e: vcmpe.f32 s0,#0
  000b1182: vmrs apsr,fpscr
  000b1186: it gt
  000b1188: b.gt.w 0x001abb78
  000b118c: bx lr
```
