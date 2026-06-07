# PlayerEgo::setSpeed
elf 0xa220e body 6
Sig: undefined __thiscall setSpeed(PlayerEgo * this, float param_1)

## decompile
```c

/* PlayerEgo::setSpeed(float) */

void __thiscall PlayerEgo::setSpeed(PlayerEgo *this,float param_1)

{
  int *in_r1;
  
  this[5].field_4 = in_r1;
  return;
}

```

## target disasm
```
  000b220e: str.w r1,[r0,#0xb8]
  000b2212: bx lr
```
