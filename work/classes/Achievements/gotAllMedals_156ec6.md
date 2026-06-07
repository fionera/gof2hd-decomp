# Achievements::gotAllMedals
elf 0x156ec6 body 6
Sig: undefined __thiscall gotAllMedals(Achievements * this)

## decompile
```c

/* Achievements::gotAllMedals() */

Achievements __thiscall Achievements::gotAllMedals(Achievements *this)

{
  return this[0x20];
}

```

## target disasm
```
  00166ec6: ldrb.w r0,[r0,#0x20]
  00166eca: bx lr
```
