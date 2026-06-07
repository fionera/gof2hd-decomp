# BoundingSphere::outerCollide
elf 0x151da0 body 108
Sig: undefined __thiscall outerCollide(BoundingSphere * this, float param_1, float param_2, float param_3)

## decompile
```c

/* BoundingSphere::outerCollide(float, float, float) */

void __thiscall
BoundingSphere::outerCollide(BoundingSphere *this,float param_1,float param_2,float param_3)

{
  float fVar1;
  int *piVar2;
  Vector local_40 [12];
  Vector local_34 [12];
  Vector aVStack_28 [12];
  int local_1c;
  
  piVar2 = *(int **)(DAT_00161e0c + 0x161db4);
  local_1c = *piVar2;
  AbyssEngine::AEMath::operator+(local_40,(Vector *)(this + 8));
  AbyssEngine::AEMath::operator-(aVStack_28,local_34);
  fVar1 = (float)AbyssEngine::AEMath::VectorDot(aVStack_28,aVStack_28);
  if (*piVar2 != local_1c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail((int)((uint)(fVar1 < *(float *)(this + 0x38) * *(float *)(this + 0x38)) << 0x1f
                          ) < 0);
  }
  return;
}

```

## target disasm
```
  00161da0: push {r4,r5,r6,r7,lr}
  00161da2: add r7,sp,#0xc
  00161da4: push.w r8
  00161da8: sub sp,#0x28
  00161daa: mov r4,r0
  00161dac: ldr r0,[0x00161e0c]
  00161dae: mov r8,sp
  00161db0: add r0,pc
  00161db2: ldr r5,[r0,#0x0]
  00161db4: ldr r0,[r5,#0x0]
  00161db6: str r0,[sp,#0x24]
  00161db8: add r0,sp,#0xc
  00161dba: stmia r0!,{r1,r2,r3}
  00161dbc: add.w r1,r4,#0x8
  00161dc0: add.w r2,r4,#0x14
  00161dc4: mov r0,r8
  00161dc6: blx 0x0006f1cc
  00161dca: add r6,sp,#0x18
  00161dcc: add r1,sp,#0xc
  00161dce: mov r2,r8
  00161dd0: mov r0,r6
  00161dd2: blx 0x0006ec38
  00161dd6: mov r0,r6
  00161dd8: mov r1,r6
  00161dda: blx 0x0006ec68
  00161dde: vldr.32 s0,[r4,#0x38]
  00161de2: vmov s2,r0
  00161de6: movs r0,#0x0
  00161de8: vmul.f32 s0,s0,s0
  00161dec: vcmpe.f32 s2,s0
  00161df0: vmrs apsr,fpscr
  00161df4: it mi
  00161df6: mov.mi r0,#0x1
  00161df8: ldr r1,[sp,#0x24]
  00161dfa: ldr r2,[r5,#0x0]
  00161dfc: subs r1,r2,r1
  00161dfe: ittt eq
  00161e00: add.eq sp,#0x28
  00161e02: pop.eq.w r8
  00161e06: pop.eq {r4,r5,r6,r7,pc}
  00161e08: blx 0x0006e824
```
