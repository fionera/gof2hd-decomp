# StarSystem::scaleSunDuringSupernovaIntro
elf 0x134ffc body 94
Sig: undefined __thiscall scaleSunDuringSupernovaIntro(StarSystem * this, int param_1)

## decompile
```c

/* StarSystem::scaleSunDuringSupernovaIntro(int) */

void __thiscall StarSystem::scaleSunDuringSupernovaIntro(StarSystem *this,int param_1)

{
  int iVar1;
  int *piVar2;
  uint in_fpscr;
  float fVar3;
  float extraout_s1;
  
  piVar2 = *(int **)(DAT_00145060 + 0x145010);
  iVar1 = *piVar2;
  AEGeometry::getScaling();
  fVar3 = (float)VectorSignedToFloat(param_1,(byte)(in_fpscr >> 0x16) & 3);
  AEGeometry::setScaling(fVar3,extraout_s1,DAT_0014505c);
  if (*piVar2 != iVar1) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  00144ffc: push {r4,r5,r6,r7,lr}
  00144ffe: add r7,sp,#0xc
  00145000: push.w r11
  00145004: sub sp,#0x10
  00145006: mov r5,r0
  00145008: ldr r0,[0x00145060]
  0014500a: mov r4,r1
  0014500c: add r0,pc
  0014500e: ldr r6,[r0,#0x0]
  00145010: ldr r0,[r6,#0x0]
  00145012: str r0,[sp,#0xc]
  00145014: ldr r0,[r5,#0x1c]
  00145016: ldr r0,[r0,#0x4]
  00145018: ldr r1,[r0,#0x0]
  0014501a: mov r0,sp
  0014501c: blx 0x00072760
  00145020: vmov s0,r4
  00145024: vldr.32 s2,[pc,#0x34]
  00145028: vcvt.f32.s32 s0,s0
  0014502c: vldr.32 s4,[sp]
  00145030: ldr r0,[r5,#0x1c]
  00145032: ldr r0,[r0,#0x4]
  00145034: vmla.f32 s4,s0,s2
  00145038: ldr r0,[r0,#0x0]
  0014503a: vmov r1,s4
  0014503e: mov r2,r1
  00145040: mov r3,r1
  00145042: blx 0x000727b4
  00145046: ldr r0,[sp,#0xc]
  00145048: ldr r1,[r6,#0x0]
  0014504a: subs r0,r1,r0
  0014504c: ittt eq
  0014504e: add.eq sp,#0x10
  00145050: pop.eq.w r11
  00145054: pop.eq {r4,r5,r6,r7,pc}
  00145056: blx 0x0006e824
```
