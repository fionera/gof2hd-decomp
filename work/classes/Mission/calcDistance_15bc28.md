# Mission::calcDistance
elf 0x15bc28 body 122
Sig: undefined __thiscall calcDistance(Mission * this)

## decompile
```c

/* Mission::calcDistance() */

void __thiscall Mission::calcDistance(Mission *this)

{
  Station *this_00;
  int iVar1;
  Station *this_01;
  int iVar2;
  float fVar3;
  int *piVar4;
  SolarSystem *pSVar5;
  SolarSystem *pSVar6;
  
  piVar4 = *(int **)(DAT_0016bca4 + 0x16bc3c);
  this_00 = (Station *)Galaxy::getStation(*piVar4);
  iVar1 = Galaxy::getSystems((Galaxy *)*piVar4);
  pSVar6 = (SolarSystem *)*piVar4;
  this_01 = (Station *)Status::getStation();
  iVar2 = Station::getSystem(this_01);
  pSVar5 = *(SolarSystem **)(*(int *)(iVar1 + 4) + iVar2 * 4);
  Station::getSystem(this_00);
  fVar3 = (float)Galaxy::distance(pSVar6,pSVar5);
  *(int *)(this + 0x60) = (int)fVar3;
  if (this_00 == (Station *)0x0) {
    return;
  }
  Station::~Station(this_00);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  0016bc28: push {r4,r5,r6,r7,lr}
  0016bc2a: add r7,sp,#0xc
  0016bc2c: push {r8,r9,r11}
  0016bc30: mov r8,r0
  0016bc32: ldr r0,[0x0016bca4]
  0016bc34: ldr.w r1,[r8,#0x3c]
  0016bc38: add r0,pc
  0016bc3a: ldr r5,[r0,#0x0]
  0016bc3c: ldr r0,[r5,#0x0]
  0016bc3e: blx 0x00071c44
  0016bc42: mov r4,r0
  0016bc44: ldr r0,[r5,#0x0]
  0016bc46: blx 0x00071884
  0016bc4a: mov r6,r0
  0016bc4c: ldr r0,[0x0016bca8]
  0016bc4e: ldr.w r9,[r5,#0x0]
  0016bc52: add r0,pc
  0016bc54: ldr r0,[r0,#0x0]
  0016bc56: ldr r0,[r0,#0x0]
  0016bc58: blx 0x00071c14
  0016bc5c: blx 0x0007189c
  0016bc60: ldr r1,[r6,#0x4]
  0016bc62: ldr.w r5,[r1,r0,lsl #0x2]
  0016bc66: mov r0,r4
  0016bc68: blx 0x0007189c
  0016bc6c: ldr r1,[r6,#0x4]
  0016bc6e: ldr.w r2,[r1,r0,lsl #0x2]
  0016bc72: mov r0,r9
  0016bc74: mov r1,r5
  0016bc76: blx 0x00071c50
  0016bc7a: vmov s0,r0
  0016bc7e: cmp r4,#0x0
  0016bc80: vcvt.s32.f32 s0,s0
  0016bc84: vstr.32 s0,[r8,#0x60]
  0016bc88: beq 0x0016bc9c
  0016bc8a: mov r0,r4
  0016bc8c: blx 0x0007360c
  0016bc90: pop.w {r8,r9,r11}
  0016bc94: pop.w {r4,r5,r6,r7,lr}
  0016bc98: b.w 0x001ab098
  0016bc9c: pop.w {r8,r9,r11}
  0016bca0: pop {r4,r5,r6,r7,pc}
```
