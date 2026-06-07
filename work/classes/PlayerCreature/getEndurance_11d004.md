# PlayerCreature::getEndurance
elf 0x11d004 body 6
Sig: undefined __thiscall getEndurance(PlayerCreature * this)

## decompile
```c

/* PlayerCreature::getEndurance() */

undefined4 __thiscall PlayerCreature::getEndurance(PlayerCreature *this)

{
  return *(undefined4 *)(this + 0x138);
}

```

## target disasm
```
  0012d004: ldr.w r0,[r0,#0x138]
  0012d008: bx lr
```
