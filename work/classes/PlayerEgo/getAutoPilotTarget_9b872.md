# PlayerEgo::getAutoPilotTarget
elf 0x9b872 body 6
Sig: undefined __thiscall getAutoPilotTarget(PlayerEgo * this)

## decompile
```c

/* PlayerEgo::getAutoPilotTarget() */

int * __thiscall PlayerEgo::getAutoPilotTarget(PlayerEgo *this)

{
  return this[9].field_18;
}

```

## target disasm
```
  000ab872: ldr.w r0,[r0,#0x15c]
  000ab876: bx lr
```
