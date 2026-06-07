# SolarSystem::stationIsInSystem
elf 0x15576c body 34
Sig: undefined __thiscall stationIsInSystem(SolarSystem * this, int param_1)

## decompile
```c

/* SolarSystem::stationIsInSystem(int) */

void __thiscall SolarSystem::stationIsInSystem(SolarSystem *this,int param_1)

{
  int iVar1;
  uint uVar2;
  
  uVar2 = 0;
  do {
    if (**(uint **)(this + 0x38) <= uVar2) {
      return;
    }
    iVar1 = uVar2 * 4;
    uVar2 = uVar2 + 1;
  } while (*(int *)((*(uint **)(this + 0x38))[1] + iVar1) != param_1);
  return;
}

```

## target disasm
```
  0016576c: ldr r0,[r0,#0x38]
  0016576e: movs r3,#0x0
  00165770: ldr.w r12,[r0,#0x0]
  00165774: cmp r3,r12
  00165776: itt cs
  00165778: mov.cs r0,#0x0
  0016577a: bx.cs lr
  0016577c: ldr r2,[r0,#0x4]
  0016577e: ldr.w r2,[r2,r3,lsl #0x2]
  00165782: adds r3,#0x1
  00165784: cmp r2,r1
  00165786: itt eq
  00165788: mov.eq r0,#0x1
  0016578a: bx.eq lr
  0016578c: b 0x00165774
```
