# Ship::getHandling
elf 0x174a68 body 66
Sig: undefined __thiscall getHandling(Ship * this)

## decompile
```c

/* Ship::getHandling() */

float __thiscall Ship::getHandling(Ship *this)

{
  int iVar1;
  int iVar2;
  int *piVar3;
  float fVar4;
  
  piVar3 = *(int **)(this + 0x78);
  fVar4 = DAT_00184aac;
  if (piVar3 != (int *)0x0) {
    iVar2 = 0;
    while (*piVar3 != iVar2) {
      iVar1 = iVar2 * 4;
      iVar2 = iVar2 + 1;
      if (*(int *)(piVar3[1] + iVar1) == 3) {
        fVar4 = fVar4 + DAT_00184ab0;
      }
    }
  }
  return fVar4 + *(float *)(this + 0x18);
}

```
## target disasm
```
  00184a68: ldr.w r12,[r0,#0x78]
  00184a6c: vldr.32 s0,[pc,#0x3c]
  00184a70: cmp.w r12,#0x0
  00184a74: beq 0x00184a9c
  00184a76: ldr.w r2,[r12,#0x0]
  00184a7a: movs r3,#0x0
  00184a7c: vldr.32 s2,[pc,#0x30]
  00184a80: b 0x00184a98
  00184a82: vadd.f32 s4,s0,s2
  00184a86: ldr.w r1,[r12,#0x4]
  00184a8a: ldr.w r1,[r1,r3,lsl #0x2]
  00184a8e: adds r3,#0x1
  00184a90: cmp r1,#0x3
  00184a92: it eq
  00184a94: vmov.eq.f32 s0,s4
  00184a98: cmp r2,r3
  00184a9a: bne 0x00184a82
  00184a9c: vldr.32 s2,[r0,#0x18]
  00184aa0: vadd.f32 s0,s0,s2
  00184aa4: vmov r0,s0
  00184aa8: bx lr
```
