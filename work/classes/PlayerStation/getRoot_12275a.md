# PlayerStation::getRoot
elf 0x12275a body 6
Sig: undefined __thiscall getRoot(PlayerStation * this)

## decompile
```c

/* PlayerStation::getRoot() */

undefined4 __thiscall PlayerStation::getRoot(PlayerStation *this)

{
  return *(undefined4 *)(this + 0x140);
}

```

## target disasm
```
  0013275a: ldr.w r0,[r0,#0x140]
  0013275e: bx lr
```
