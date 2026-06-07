# SolarSystem::systemIsInSystemRoutes
elf 0x1558ae body 40
Sig: undefined __thiscall systemIsInSystemRoutes(SolarSystem * this, int param_1)

## decompile
```c

/* SolarSystem::systemIsInSystemRoutes(int) */

undefined4 __thiscall SolarSystem::systemIsInSystemRoutes(SolarSystem *this,int param_1)

{
  int iVar1;
  uint *puVar2;
  uint uVar3;
  
  if (*(int *)(this + 0x18) != param_1) {
    puVar2 = *(uint **)(this + 0x40);
    if (puVar2 == (uint *)0x0) {
      return 0;
    }
    uVar3 = 0;
    do {
      if (*puVar2 <= uVar3) {
        return 0;
      }
      iVar1 = uVar3 * 4;
      uVar3 = uVar3 + 1;
    } while (*(int *)(puVar2[1] + iVar1) != param_1);
  }
  return 1;
}

```

## target disasm
```
  001658ae: ldr r2,[r0,#0x18]
  001658b0: cmp r2,r1
  001658b2: beq 0x001658ce
  001658b4: ldr r0,[r0,#0x40]
  001658b6: cbz r0,0x001658d2
  001658b8: ldr.w r12,[r0,#0x0]
  001658bc: movs r3,#0x0
  001658be: cmp r3,r12
  001658c0: bcs 0x001658d2
  001658c2: ldr r2,[r0,#0x4]
  001658c4: ldr.w r2,[r2,r3,lsl #0x2]
  001658c8: adds r3,#0x1
  001658ca: cmp r2,r1
  001658cc: bne 0x001658be
  001658ce: movs r0,#0x1
  001658d0: bx lr
  001658d2: movs r0,#0x0
  001658d4: bx lr
```
