# Gun::getEnemies
elf 0x1523bc body 6
Sig: undefined __thiscall getEnemies(Gun * this)

## decompile
```c

/* Gun::getEnemies() */

undefined4 __thiscall Gun::getEnemies(Gun *this)

{
  return *(undefined4 *)(this + 0xb4);
}

```

## target disasm
```
  001623bc: ldr.w r0,[r0,#0xb4]
  001623c0: bx lr
```
