# StarSystem::switchPlanetForIntro
elf 0x134dcc body 120
Sig: undefined __thiscall switchPlanetForIntro(StarSystem * this)

## decompile
```c

/* StarSystem::switchPlanetForIntro() */

void __thiscall StarSystem::switchPlanetForIntro(StarSystem *this)

{
  int *piVar1;
  float fVar2;
  Vector aVStack_28 [12];
  int local_1c;
  
  piVar1 = *(int **)(DAT_00144e44 + 0x144de0);
  local_1c = *piVar1;
  AbyssEngine::PaintCanvas::TextureCreate
            ((ushort)**(undefined4 **)(DAT_00144e48 + 0x144de8),(uint *)0x273a,
             (bool)((char)*(undefined4 *)(*(int *)(this + 0x14) + 4) +
                   (char)(*(int *)(this + 0x50) << 2)));
  fVar2 = (float)AEGeometry::getScaling();
  AbyssEngine::AEMath::operator*(aVStack_28,fVar2);
  AEGeometry::setScaling
            (*(AEGeometry **)(*(int *)(*(int *)(this + 0x1c) + 4) + *(int *)(this + 0x50) * 4),
             aVStack_28);
  if (*piVar1 != local_1c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  00144dcc: push {r4,r5,r6,r7,lr}
  00144dce: add r7,sp,#0xc
  00144dd0: push.w r8
  00144dd4: sub sp,#0x20
  00144dd6: mov r4,r0
  00144dd8: ldr r0,[0x00144e44]
  00144dda: movs r3,#0x0
  00144ddc: add r0,pc
  00144dde: ldr r6,[r0,#0x0]
  00144de0: ldr r0,[0x00144e48]
  00144de2: ldr r1,[r6,#0x0]
  00144de4: add r0,pc
  00144de6: str r1,[sp,#0x1c]
  00144de8: ldr r1,[r4,#0x14]
  00144dea: ldr r0,[r0,#0x0]
  00144dec: ldr r2,[r4,#0x50]
  00144dee: ldr r1,[r1,#0x4]
  00144df0: ldr r0,[r0,#0x0]
  00144df2: add.w r2,r1,r2, lsl #0x2
  00144df6: movw r1,#0x273a
  00144dfa: blx 0x0006fbb0
  00144dfe: ldr r0,[r4,#0x1c]
  00144e00: add r5,sp,#0x4
  00144e02: ldr r1,[r4,#0x50]
  00144e04: ldr r0,[r0,#0x4]
  00144e06: ldr.w r1,[r0,r1,lsl #0x2]
  00144e0a: mov r0,r5
  00144e0c: blx 0x00072760
  00144e10: add.w r8,sp,#0x10
  00144e14: mov r1,r5
  00144e16: mov.w r2,#0x40000000
  00144e1a: mov r0,r8
  00144e1c: blx 0x0006ec74
  00144e20: ldr r0,[r4,#0x1c]
  00144e22: ldr r1,[r4,#0x50]
  00144e24: ldr r0,[r0,#0x4]
  00144e26: ldr.w r0,[r0,r1,lsl #0x2]
  00144e2a: mov r1,r8
  00144e2c: blx 0x00073de0
  00144e30: ldr r0,[sp,#0x1c]
  00144e32: ldr r1,[r6,#0x0]
  00144e34: subs r0,r1,r0
  00144e36: ittt eq
  00144e38: add.eq sp,#0x20
  00144e3a: pop.eq.w r8
  00144e3e: pop.eq {r4,r5,r6,r7,pc}
  00144e40: blx 0x0006e824
```
