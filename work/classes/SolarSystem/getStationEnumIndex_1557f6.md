# SolarSystem::getStationEnumIndex
elf 0x1557f6 body 34
Sig: undefined __thiscall getStationEnumIndex(SolarSystem * this, int param_1)

## decompile
```c

/* SolarSystem::getStationEnumIndex(int) */

uint __thiscall SolarSystem::getStationEnumIndex(SolarSystem *this,int param_1)

{
  uint uVar1;
  
  uVar1 = 0;
  while( true ) {
    if (**(uint **)(this + 0x38) <= uVar1) {
      return 0xffffffff;
    }
    if (*(int *)((*(uint **)(this + 0x38))[1] + uVar1 * 4) == param_1) break;
    uVar1 = uVar1 + 1;
  }
  return uVar1;
}

```

## target disasm
```
  001657f6: ldr r2,[r0,#0x38]
  001657f8: movs r0,#0x0
  001657fa: ldr.w r12,[r2,#0x0]
  001657fe: b 0x0016580e
  00165800: ldr r3,[r2,#0x4]
  00165802: ldr.w r3,[r3,r0,lsl #0x2]
  00165806: cmp r3,r1
  00165808: it eq
  0016580a: bx.eq lr
  0016580c: adds r0,#0x1
  0016580e: cmp r0,r12
  00165810: bcc 0x00165800
  00165812: mov.w r0,#0xffffffff
  00165816: bx lr
```
