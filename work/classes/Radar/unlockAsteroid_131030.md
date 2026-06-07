# Radar::unlockAsteroid
elf 0x131030 body 6
Sig: undefined __thiscall unlockAsteroid(Radar * this)

## decompile
```c

/* Radar::unlockAsteroid() */

void __thiscall Radar::unlockAsteroid(Radar *this)

{
  *(undefined4 *)(this + 0xc) = 0;
  return;
}

```

## target disasm
```
  00141030: movs r1,#0x0
  00141032: str r1,[r0,#0xc]
  00141034: bx lr
```
