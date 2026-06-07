# SolarSystem::currentOrbitHasWarpGate
elf 0x155818 body 32
Sig: undefined __stdcall currentOrbitHasWarpGate(void)

## decompile
```c

/* SolarSystem::currentOrbitHasWarpGate() */

bool SolarSystem::currentOrbitHasWarpGate(void)

{
  int in_r0;
  Station *this;
  int iVar1;
  int iVar2;
  
  iVar2 = *(int *)(in_r0 + 0x30);
  this = (Station *)Status::getStation();
  iVar1 = Station::getIndex(this);
  return iVar2 == iVar1;
}

```

## target disasm
```
  00165818: push {r4,r6,r7,lr}
  0016581a: add r7,sp,#0x8
  0016581c: ldr r1,[0x00165838]
  0016581e: ldr r4,[r0,#0x30]
  00165820: add r1,pc
  00165822: ldr r1,[r1,#0x0]
  00165824: ldr r0,[r1,#0x0]
  00165826: blx 0x00071c14
  0016582a: blx 0x00071824
  0016582e: subs r0,r4,r0
  00165830: clz r0,r0
  00165834: lsrs r0,r0,#0x5
  00165836: pop {r4,r6,r7,pc}
```
