# PlayerEgo::getVolatileForce
elf 0x9a9c4 body 46
Sig: undefined __thiscall getVolatileForce(PlayerEgo * this)

## decompile
```c

/* PlayerEgo::getVolatileForce() */

float __thiscall PlayerEgo::getVolatileForce(PlayerEgo *this)

{
  float fVar1;
  float fVar2;
  
  fVar1 = *(float *)(this->m_pPlayer + 0x60);
  fVar2 = DAT_000aa9f4;
  if ((-1 < (int)((uint)(fVar1 < 0.0) << 0x1f)) && (fVar2 = 1.0, fVar1 <= 1.0)) {
    fVar2 = fVar1;
  }
  return fVar2;
}

```

## target disasm
```
  000aa9c4: ldr r0,[r0,#0x0]
  000aa9c6: vldr.32 s0,[r0,#0x60]
  000aa9ca: vcmpe.f32 s0,#0
  000aa9ce: vmrs apsr,fpscr
  000aa9d2: bpl 0x000aa9da
  000aa9d4: vldr.32 s2,[pc,#0x1c]
  000aa9d8: b 0x000aa9ec
  000aa9da: vmov.f32 s2,0x3f800000
  000aa9de: vcmpe.f32 s0,s2
  000aa9e2: vmrs apsr,fpscr
  000aa9e6: it le
  000aa9e8: vmov.le.f32 s2,s0
  000aa9ec: vmov r0,s2
  000aa9f0: bx lr
```
