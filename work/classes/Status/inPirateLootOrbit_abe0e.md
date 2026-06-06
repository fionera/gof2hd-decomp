# Status::inPirateLootOrbit
elf 0xabe0e body 50
Sig: undefined __stdcall inPirateLootOrbit(void)

## decompile
```c

/* Status::inPirateLootOrbit() */

bool Status::inPirateLootOrbit(void)

{
  int iVar1;
  
  iVar1 = inAlienOrbit();
  if (iVar1 != 0) {
    return false;
  }
  iVar1 = SolarSystem::getIndex();
  if (iVar1 == 0x20) {
    return true;
  }
  iVar1 = SolarSystem::getIndex();
  return iVar1 == 0x21;
}

```
## target disasm
```
  000bbe0e: push {r4,r6,r7,lr}
  000bbe10: add r7,sp,#0x8
  000bbe12: mov r4,r0
  000bbe14: blx 0x000723d0
  000bbe18: cbz r0,0x000bbe1e
  000bbe1a: movs r0,#0x0
  000bbe1c: pop {r4,r6,r7,pc}
  000bbe1e: ldr.w r0,[r4,#0x1a4]
  000bbe22: blx 0x00071a7c
  000bbe26: cmp r0,#0x20
  000bbe28: bne 0x000bbe2e
  000bbe2a: movs r0,#0x1
  000bbe2c: pop {r4,r6,r7,pc}
  000bbe2e: ldr.w r0,[r4,#0x1a4]
  000bbe32: blx 0x00071a7c
  000bbe36: subs r0,#0x21
  000bbe38: clz r0,r0
  000bbe3c: lsrs r0,r0,#0x5
  000bbe3e: pop {r4,r6,r7,pc}
```
