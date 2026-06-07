# Radar::isPlasmaInRange
elf 0x131056 body 6
Sig: undefined __thiscall isPlasmaInRange(Radar * this)

## decompile
```c

/* Radar::isPlasmaInRange() */

Radar __thiscall Radar::isPlasmaInRange(Radar *this)

{
  return this[0x130];
}

```

## target disasm
```
  00141056: ldrb.w r0,[r0,#0x130]
  0014105a: bx lr
```
