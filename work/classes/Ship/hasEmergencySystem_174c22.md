# Ship::hasEmergencySystem
elf 0x174c22 body 6
Sig: undefined __thiscall hasEmergencySystem(Ship * this)

## decompile
```c

/* Ship::hasEmergencySystem() */

Ship __thiscall Ship::hasEmergencySystem(Ship *this)

{
  return this[0x5c];
}

```
## target disasm
```
  00184c22: ldrb.w r0,[r0,#0x5c]
  00184c26: bx lr
```
