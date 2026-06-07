# SolarSystem::hasHiddenBlueprint
elf 0x155790 body 68
Sig: undefined __thiscall hasHiddenBlueprint(SolarSystem * this)

## decompile
```c

/* SolarSystem::hasHiddenBlueprint() */

undefined4 __thiscall SolarSystem::hasHiddenBlueprint(SolarSystem *this)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  int iVar4;
  
  uVar3 = 0;
  iVar4 = **(int **)(DAT_001657d4 + 0x1657a2);
  iVar2 = DAT_001657d8 + 0x1657ac;
  while( true ) {
    if (4 < uVar3) {
      return 0;
    }
    iVar1 = stationIsInSystem(this,*(int *)(iVar2 + uVar3 * 4));
    if ((iVar1 != 0) && (*(char *)(*(int *)(*(int *)(iVar4 + 0x58) + 4) + uVar3) == '\0')) break;
    uVar3 = uVar3 + 1;
  }
  return 1;
}

```

## target disasm
```
  00165790: push {r4,r5,r6,r7,lr}
  00165792: add r7,sp,#0xc
  00165794: push.w r8
  00165798: mov r4,r0
  0016579a: ldr r0,[0x001657d4]
  0016579c: movs r6,#0x0
  0016579e: add r0,pc
  001657a0: ldr r0,[r0,#0x0]
  001657a2: ldr.w r8,[r0,#0x0]
  001657a6: ldr r5,[0x001657d8]
  001657a8: add r5,pc
  001657aa: b 0x001657c4
  001657ac: ldr.w r1,[r5,r6,lsl #0x2]
  001657b0: mov r0,r4
  001657b2: blx 0x00071a94
  001657b6: cbz r0,0x001657c2
  001657b8: ldr.w r0,[r8,#0x58]
  001657bc: ldr r0,[r0,#0x4]
  001657be: ldrb r0,[r0,r6]
  001657c0: cbz r0,0x001657cc
  001657c2: adds r6,#0x1
  001657c4: cmp r6,#0x4
  001657c6: bls 0x001657ac
  001657c8: movs r0,#0x0
  001657ca: b 0x001657ce
  001657cc: movs r0,#0x1
  001657ce: pop.w r8
  001657d2: pop {r4,r5,r6,r7,pc}
```
