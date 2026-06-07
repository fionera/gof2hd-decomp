# PlayerCreature::getItemIndex
elf 0x11cff8 body 6
Sig: undefined __thiscall getItemIndex(PlayerCreature * this)

## decompile
```c

/* PlayerCreature::getItemIndex() */

undefined4 __thiscall PlayerCreature::getItemIndex(PlayerCreature *this)

{
  return *(undefined4 *)(this + 0x140);
}

```

## target disasm
```
  0012cff8: ldr.w r0,[r0,#0x140]
  0012cffc: bx lr
```
