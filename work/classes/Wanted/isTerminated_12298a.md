# Wanted::isTerminated
elf 0x12298a body 6
Sig: undefined __thiscall isTerminated(Wanted * this)

## decompile
```c

/* Wanted::isTerminated() */

Wanted __thiscall Wanted::isTerminated(Wanted *this)

{
  return this[0x50];
}

```

## target disasm
```
  0013298a: ldrb.w r0,[r0,#0x50]
  0013298e: bx lr
```
