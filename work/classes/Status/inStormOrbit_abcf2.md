# Status::inStormOrbit
elf 0xabcf2 body 66
Sig: undefined __thiscall inStormOrbit(Status * this)

## decompile
```c

/* Status::inStormOrbit() */

bool __thiscall Status::inStormOrbit(Status *this)

{
  int iVar1;
  
  iVar1 = inAlienOrbit();
  if ((iVar1 != 0) || (*(int *)(this + 0x1e8) < 0x5a)) {
    return false;
  }
  iVar1 = inSupernovaSystem();
  if ((iVar1 == 0) && (iVar1 = SolarSystem::getTextureIndex(), iVar1 != 0x10)) {
    iVar1 = SolarSystem::getTextureIndex();
    return iVar1 == 0x12;
  }
  return true;
}

```
## target disasm
```
  000bbcf2: push {r4,r6,r7,lr}
  000bbcf4: add r7,sp,#0x8
  000bbcf6: mov r4,r0
  000bbcf8: blx 0x000723d0
  000bbcfc: cbnz r0,0x000bbd1e
  000bbcfe: ldr.w r0,[r4,#0x1e8]
  000bbd02: cmp r0,#0x5a
  000bbd04: blt 0x000bbd1e
  000bbd06: mov r0,r4
  000bbd08: blx 0x00071878
  000bbd0c: cbnz r0,0x000bbd1a
  000bbd0e: ldr.w r0,[r4,#0x1a4]
  000bbd12: blx 0x0007390c
  000bbd16: cmp r0,#0x10
  000bbd18: bne 0x000bbd22
  000bbd1a: movs r0,#0x1
  000bbd1c: pop {r4,r6,r7,pc}
  000bbd1e: movs r0,#0x0
  000bbd20: pop {r4,r6,r7,pc}
  000bbd22: ldr.w r0,[r4,#0x1a4]
  000bbd26: blx 0x0007390c
  000bbd2a: subs r0,#0x12
  000bbd2c: clz r0,r0
  000bbd30: lsrs r0,r0,#0x5
  000bbd32: pop {r4,r6,r7,pc}
```
