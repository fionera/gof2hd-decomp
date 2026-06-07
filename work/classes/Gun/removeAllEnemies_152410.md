# Gun::removeAllEnemies
elf 0x152410 body 8
Sig: undefined __thiscall removeAllEnemies(Gun * this)

## decompile
```c

/* Gun::removeAllEnemies() */

void __thiscall Gun::removeAllEnemies(Gun *this)

{
  *(undefined4 *)(this + 0xb4) = 0;
  return;
}

```

## target disasm
```
  00162410: movs r1,#0x0
  00162412: str.w r1,[r0,#0xb4]
  00162416: bx lr
```
