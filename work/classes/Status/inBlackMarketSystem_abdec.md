# Status::inBlackMarketSystem
elf 0xabdec body 34
Sig: undefined __stdcall inBlackMarketSystem(void)

## decompile
```c

/* Status::inBlackMarketSystem() */

bool Status::inBlackMarketSystem(void)

{
  int iVar1;
  
  iVar1 = inAlienOrbit();
  if (iVar1 != 0) {
    return false;
  }
  iVar1 = SolarSystem::getIndex();
  return iVar1 == 0x19;
}

```
## target disasm
```
  000bbdec: push {r4,r6,r7,lr}
  000bbdee: add r7,sp,#0x8
  000bbdf0: mov r4,r0
  000bbdf2: blx 0x000723d0
  000bbdf6: cbz r0,0x000bbdfc
  000bbdf8: movs r0,#0x0
  000bbdfa: pop {r4,r6,r7,pc}
  000bbdfc: ldr.w r0,[r4,#0x1a4]
  000bbe00: blx 0x00071a7c
  000bbe04: subs r0,#0x19
  000bbe06: clz r0,r0
  000bbe0a: lsrs r0,r0,#0x5
  000bbe0c: pop {r4,r6,r7,pc}
```
