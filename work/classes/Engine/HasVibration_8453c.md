# Engine::HasVibration
elf 0x8453c body 22
Sig: undefined __thiscall HasVibration(Engine * this)

## decompile
```c

/* AbyssEngine::Engine::HasVibration() */

bool __thiscall AbyssEngine::Engine::HasVibration(Engine *this)

{
  if (this[0x480] != (Engine)0x0) {
    return this[0x2c] != (Engine)0x0;
  }
  return false;
}

```

## target disasm
```
  0009453c: ldrb.w r1,[r0,#0x480]
  00094540: cbz r1,0x0009454e
  00094542: ldrb.w r0,[r0,#0x2c]
  00094546: cmp r0,#0x0
  00094548: it ne
  0009454a: mov.ne r0,#0x1
  0009454c: bx lr
  0009454e: movs r0,#0x0
  00094550: bx lr
```
