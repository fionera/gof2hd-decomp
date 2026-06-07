# Wanted::isActive
elf 0x122984 body 6
Sig: undefined __thiscall isActive(Wanted * this)

## decompile
```c

/* Wanted::isActive() */

Wanted __thiscall Wanted::isActive(Wanted *this)

{
  return this[0x51];
}

```

## target disasm
```
  00132984: ldrb.w r0,[r0,#0x51]
  00132988: bx lr
```
