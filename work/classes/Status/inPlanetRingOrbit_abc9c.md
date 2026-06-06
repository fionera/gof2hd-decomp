# Status::inPlanetRingOrbit
elf 0xabc9c body 52
Sig: undefined __stdcall inPlanetRingOrbit(void)

## decompile
```c

/* Status::inPlanetRingOrbit() */

uint Status::inPlanetRingOrbit(void)

{
  int in_r0;
  int iVar1;
  
  iVar1 = inAlienOrbit();
  if (iVar1 == 0) {
    iVar1 = Station::getIndex(*(Station **)(in_r0 + 0x19c));
    if ((iVar1 - 0x78U >> 1 | iVar1 << 0x1f) < 7) {
      return 0x69U >> (iVar1 - 0x78U >> 1 & 0x7f) & 1;
    }
  }
  return 0;
}

```
## target disasm
```
  000bbc9c: push {r4,r6,r7,lr}
  000bbc9e: add r7,sp,#0x8
  000bbca0: mov r4,r0
  000bbca2: blx 0x000723d0
  000bbca6: cbnz r0,0x000bbccc
  000bbca8: ldr.w r0,[r4,#0x19c]
  000bbcac: blx 0x00071824
  000bbcb0: subs r0,#0x78
  000bbcb2: ror r1,r0, ror #0x1
  000bbcb6: cmp r1,#0x6
  000bbcb8: bhi 0x000bbccc
  000bbcba: lsrs r0,r0,#0x1
  000bbcbc: movs r1,#0x69
  000bbcbe: and r0,r0,#0x7f
  000bbcc2: lsr.w r0,r1,r0
  000bbcc6: and r0,r0,#0x1
  000bbcca: pop {r4,r6,r7,pc}
  000bbccc: movs r0,#0x0
  000bbcce: pop {r4,r6,r7,pc}
```
