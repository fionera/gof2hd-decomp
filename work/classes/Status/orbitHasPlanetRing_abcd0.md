# Status::orbitHasPlanetRing
elf 0xabcd0 body 34
Sig: undefined __thiscall orbitHasPlanetRing(Status * this, int param_1)

## decompile
```c

/* Status::orbitHasPlanetRing(int) */

uint __thiscall Status::orbitHasPlanetRing(Status *this,int param_1)

{
  if ((param_1 - 0x78U >> 1 | param_1 << 0x1f) < 7) {
    return 0x69U >> (param_1 - 0x78U >> 1 & 0x7f) & 1;
  }
  return 0;
}

```
## target disasm
```
  000bbcd0: sub.w r0,r1,#0x78
  000bbcd4: ror r1,r0, ror #0x1
  000bbcd8: cmp r1,#0x6
  000bbcda: itt hi
  000bbcdc: mov.hi r0,#0x0
  000bbcde: bx.hi lr
  000bbce0: lsrs r0,r0,#0x1
  000bbce2: movs r1,#0x69
  000bbce4: and r0,r0,#0x7f
  000bbce8: lsr.w r0,r1,r0
  000bbcec: and r0,r0,#0x1
  000bbcf0: bx lr
```
