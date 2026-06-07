# PlayerFighter::setSpeed
elf 0xdcccc body 16
Sig: undefined __thiscall setSpeed(PlayerFighter * this, float param_1)

## decompile
```c

/* PlayerFighter::setSpeed(float) */

void __thiscall PlayerFighter::setSpeed(PlayerFighter *this,float param_1)

{
  undefined4 in_r1;
  
  *(undefined4 *)(this + 0x1ac) = in_r1;
  this[0x13d] = (PlayerFighter)0x0;
  *(undefined4 *)(this + 0x1e8) = in_r1;
  return;
}

```

## target disasm
```
  000ecccc: movs r2,#0x0
  000eccce: str.w r1,[r0,#0x1ac]
  000eccd2: strb.w r2,[r0,#0x13d]
  000eccd6: str.w r1,[r0,#0x1e8]
  000eccda: bx lr
```
