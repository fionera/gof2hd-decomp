# SolarSystem::hasPirateBase
elf 0x155720 body 68
Sig: undefined __thiscall hasPirateBase(SolarSystem * this)

## decompile
```c

/* SolarSystem::hasPirateBase() */

undefined4 __thiscall SolarSystem::hasPirateBase(SolarSystem *this)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  int iVar4;
  
  uVar3 = 0;
  iVar4 = **(int **)(DAT_00165764 + 0x165732);
  iVar2 = DAT_00165768 + 0x16573c;
  while( true ) {
    if (3 < uVar3) {
      return 0;
    }
    iVar1 = stationIsInSystem(this,*(int *)(iVar2 + uVar3 * 4));
    if ((iVar1 != 0) && (*(char *)(*(int *)(*(int *)(iVar4 + 0x4c) + 4) + uVar3) == '\0')) break;
    uVar3 = uVar3 + 1;
  }
  return 1;
}

```

## target disasm
```
  00165720: push {r4,r5,r6,r7,lr}
  00165722: add r7,sp,#0xc
  00165724: push.w r8
  00165728: mov r4,r0
  0016572a: ldr r0,[0x00165764]
  0016572c: movs r6,#0x0
  0016572e: add r0,pc
  00165730: ldr r0,[r0,#0x0]
  00165732: ldr.w r8,[r0,#0x0]
  00165736: ldr r5,[0x00165768]
  00165738: add r5,pc
  0016573a: b 0x00165754
  0016573c: ldr.w r1,[r5,r6,lsl #0x2]
  00165740: mov r0,r4
  00165742: blx 0x00071a94
  00165746: cbz r0,0x00165752
  00165748: ldr.w r0,[r8,#0x4c]
  0016574c: ldr r0,[r0,#0x4]
  0016574e: ldrb r0,[r0,r6]
  00165750: cbz r0,0x0016575c
  00165752: adds r6,#0x1
  00165754: cmp r6,#0x3
  00165756: bls 0x0016573c
  00165758: movs r0,#0x0
  0016575a: b 0x0016575e
  0016575c: movs r0,#0x1
  0016575e: pop.w r8
  00165762: pop {r4,r5,r6,r7,pc}
```
