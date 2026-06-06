# Ship::getHandlingForShop
elf 0x174a1c body 66
Sig: undefined __thiscall getHandlingForShop(Ship * this)

## decompile
```c

/* Ship::getHandlingForShop() */

float __thiscall Ship::getHandlingForShop(Ship *this)

{
  int iVar1;
  int iVar2;
  int *piVar3;
  float fVar4;
  
  piVar3 = *(int **)(this + 0x78);
  fVar4 = DAT_00184a60;
  if (piVar3 != (int *)0x0) {
    iVar2 = 0;
    while (*piVar3 != iVar2) {
      iVar1 = iVar2 * 4;
      iVar2 = iVar2 + 1;
      if (*(int *)(piVar3[1] + iVar1) == 3) {
        fVar4 = fVar4 + DAT_00184a64;
      }
    }
  }
  return fVar4 + *(float *)(this + 0x18);
}

```
## target disasm
```
  00184a1c: ldr.w r12,[r0,#0x78]
  00184a20: vldr.32 s0,[pc,#0x3c]
  00184a24: cmp.w r12,#0x0
  00184a28: beq 0x00184a50
  00184a2a: ldr.w r2,[r12,#0x0]
  00184a2e: movs r3,#0x0
  00184a30: vldr.32 s2,[pc,#0x30]
  00184a34: b 0x00184a4c
  00184a36: vadd.f32 s4,s0,s2
  00184a3a: ldr.w r1,[r12,#0x4]
  00184a3e: ldr.w r1,[r1,r3,lsl #0x2]
  00184a42: adds r3,#0x1
  00184a44: cmp r1,#0x3
  00184a46: it eq
  00184a48: vmov.eq.f32 s0,s4
  00184a4c: cmp r2,r3
  00184a4e: bne 0x00184a36
  00184a50: vldr.32 s2,[r0,#0x18]
  00184a54: vadd.f32 s0,s0,s2
  00184a58: vmov r0,s0
  00184a5c: bx lr
```
