# PlayerCreature::getMaxEndurance
elf 0x11cffe body 6
Sig: undefined __thiscall getMaxEndurance(PlayerCreature * this)

## decompile
```c

/* PlayerCreature::getMaxEndurance() */

undefined4 __thiscall PlayerCreature::getMaxEndurance(PlayerCreature *this)

{
  return *(undefined4 *)(this + 0x134);
}

```

## target disasm
```
  0012cffe: ldr.w r0,[r0,#0x134]
  0012d002: bx lr
```
