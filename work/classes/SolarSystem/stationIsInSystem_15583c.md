# SolarSystem::stationIsInSystem
elf 0x15583c body 32
Sig: undefined __thiscall stationIsInSystem(SolarSystem * this, Station * param_1)

## decompile
```c

/* SolarSystem::stationIsInSystem(Station*) */

undefined4 __thiscall SolarSystem::stationIsInSystem(SolarSystem *this,Station *param_1)

{
  undefined4 uVar1;
  
  if (param_1 == (Station *)0x0) {
    return 0;
  }
  uVar1 = Station::getIndex(param_1);
  uVar1 = (*(code *)(DAT_001ac584 + 0x1ac588))(this,uVar1);
  return uVar1;
}

```

## target disasm
```
  0016583c: push {r4,r6,r7,lr}
  0016583e: add r7,sp,#0x8
  00165840: mov r4,r0
  00165842: cbz r1,0x00165856
  00165844: mov r0,r1
  00165846: blx 0x00071824
  0016584a: mov r1,r0
  0016584c: mov r0,r4
  0016584e: pop.w {r4,r6,r7,lr}
  00165852: b.w 0x001ac578
  00165856: movs r0,#0x0
  00165858: pop {r4,r6,r7,pc}
  001ac578: bx pc
```
