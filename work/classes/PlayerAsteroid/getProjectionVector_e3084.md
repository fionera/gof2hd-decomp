# PlayerAsteroid::getProjectionVector
elf 0xe3084 body 114
Sig: undefined __stdcall getProjectionVector(Vector * param_1)

## decompile
```c

/* PlayerAsteroid::getProjectionVector(AbyssEngine::AEMath::Vector const&) */

void PlayerAsteroid::getProjectionVector(Vector *param_1)

{
  int iVar1;
  int iVar2;
  undefined8 *in_r2;
  code *pcVar3;
  Vector *this;
  int *piVar4;
  undefined8 uVar5;
  
  piVar4 = *(int **)(DAT_000f30f8 + 0xf3096);
  iVar1 = *piVar4;
  uVar5 = *in_r2;
  *(undefined4 *)(param_1 + 8) = *(undefined4 *)(in_r2 + 1);
  *(undefined8 *)param_1 = uVar5;
  AEGeometry::getPosition();
  this = *(Vector **)(DAT_000f30fc + 0xf30ba);
  pcVar3 = *(code **)(DAT_000f3100 + 0xf30bc);
  (*pcVar3)(this,(AEMath *)&stack0xffffffd0);
  AbyssEngine::AEMath::Vector::operator-=(this,param_1);
  (*pcVar3)(param_1,this);
  AbyssEngine::AEMath::VectorNormalize((AEMath *)&stack0xffffffd0,param_1);
  (*pcVar3)(param_1,(AEMath *)&stack0xffffffd0);
  iVar2 = *piVar4;
  iVar1 = iVar2 - iVar1;
  if (iVar1 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar1,iVar2);
  }
  return;
}

```

## target disasm
```
  000f3084: push {r4,r5,r6,r7,lr}
  000f3086: add r7,sp,#0xc
  000f3088: push {r4,r5,r6,r7,r8,r9,r11}
  000f308c: mov r4,r0
  000f308e: ldr r0,[0x000f30f8]
  000f3090: mov r9,sp
  000f3092: add r0,pc
  000f3094: ldr.w r8,[r0,#0x0]
  000f3098: ldr.w r0,[r8,#0x0]
  000f309c: str r0,[sp,#0xc]
  000f309e: vldr.64 d16,[r2]
  000f30a2: ldr r0,[r2,#0x8]
  000f30a4: str r0,[r4,#0x8]
  000f30a6: mov r0,r9
  000f30a8: vstr.64 d16,[r4]
  000f30ac: ldr r1,[r1,#0x8]
  000f30ae: blx 0x000720b8
  000f30b2: ldr r0,[0x000f30fc]
  000f30b4: ldr r1,[0x000f3100]
  000f30b6: add r0,pc
  000f30b8: add r1,pc
  000f30ba: ldr r6,[r0,#0x0]
  000f30bc: ldr r5,[r1,#0x0]
  000f30be: mov r1,r9
  000f30c0: mov r0,r6
  000f30c2: blx r5
  000f30c4: mov r0,r6
  000f30c6: mov r1,r4
  000f30c8: blx 0x00072694
  000f30cc: mov r0,r4
  000f30ce: mov r1,r6
  000f30d0: blx r5
  000f30d2: mov r6,sp
  000f30d4: mov r1,r4
  000f30d6: mov r0,r6
  000f30d8: blx 0x0006ec80
  000f30dc: mov r0,r4
  000f30de: mov r1,r6
  000f30e0: blx r5
  000f30e2: ldr r0,[sp,#0xc]
  000f30e4: ldr.w r1,[r8,#0x0]
  000f30e8: subs r0,r1,r0
  000f30ea: itt eq
  000f30ec: pop.eq.w {r0,r1,r2,r3,r8,r9,r11}
  000f30f0: pop.eq {r4,r5,r6,r7,pc}
  000f30f2: blx 0x0006e824
```
