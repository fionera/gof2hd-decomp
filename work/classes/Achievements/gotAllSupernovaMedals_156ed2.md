# Achievements::gotAllSupernovaMedals
elf 0x156ed2 body 6
Sig: undefined __thiscall gotAllSupernovaMedals(Achievements * this)

## decompile
```c

/* Achievements::gotAllSupernovaMedals() */

Achievements __thiscall Achievements::gotAllSupernovaMedals(Achievements *this)

{
  return this[0x22];
}

```

## target disasm
```
  00166ed2: ldrb.w r0,[r0,#0x22]
  00166ed6: bx lr
```
