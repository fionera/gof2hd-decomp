# Status::inFogSkyboxOrbit
elf 0xabd66 body 50
Sig: undefined __stdcall inFogSkyboxOrbit(void)

## decompile
```c

/* Status::inFogSkyboxOrbit() */

bool Status::inFogSkyboxOrbit(void)

{
  int iVar1;
  
  iVar1 = inAlienOrbit();
  if (iVar1 != 0) {
    return false;
  }
  iVar1 = SolarSystem::getTextureIndex();
  if (iVar1 == 0x11) {
    return true;
  }
  iVar1 = SolarSystem::getTextureIndex();
  return iVar1 == 0x12;
}

```
## target disasm
```
  000bbd66: push {r4,r6,r7,lr}
  000bbd68: add r7,sp,#0x8
  000bbd6a: mov r4,r0
  000bbd6c: blx 0x000723d0
  000bbd70: cbz r0,0x000bbd76
  000bbd72: movs r0,#0x0
  000bbd74: pop {r4,r6,r7,pc}
  000bbd76: ldr.w r0,[r4,#0x1a4]
  000bbd7a: blx 0x0007390c
  000bbd7e: cmp r0,#0x11
  000bbd80: bne 0x000bbd86
  000bbd82: movs r0,#0x1
  000bbd84: pop {r4,r6,r7,pc}
  000bbd86: ldr.w r0,[r4,#0x1a4]
  000bbd8a: blx 0x0007390c
  000bbd8e: subs r0,#0x12
  000bbd90: clz r0,r0
  000bbd94: lsrs r0,r0,#0x5
  000bbd96: pop {r4,r6,r7,pc}
```
