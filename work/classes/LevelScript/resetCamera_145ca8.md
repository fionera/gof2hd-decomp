# LevelScript::resetCamera
elf 0x145ca8 body 108
Sig: undefined __stdcall resetCamera(Level * param_1)

## decompile
```c

/* LevelScript::resetCamera(Level*) */

void LevelScript::resetCamera(Level *param_1)

{
  int iVar1;
  int iVar2;
  int *piVar3;
  AEGeometry *pAVar4;
  
  piVar3 = *(int **)(DAT_00155d20 + 0x155cbc);
  iVar1 = *piVar3;
  iVar2 = Level::getPlayer();
  if (iVar2 != 0) {
    pAVar4 = *(AEGeometry **)(param_1 + 0x14);
    Level::getPlayer();
    TargetFollowCamera::setTarget(pAVar4);
    TargetFollowCamera::setTargetOffset(*(Vector **)(param_1 + 0x14));
    TargetFollowCamera::setCamOffset(*(Vector **)(param_1 + 0x14));
  }
  if (*piVar3 != iVar1) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  00155ca8: push {r4,r5,r6,r7,lr}
  00155caa: add r7,sp,#0xc
  00155cac: push.w r8
  00155cb0: sub sp,#0x10
  00155cb2: mov r4,r0
  00155cb4: ldr r0,[0x00155d20]
  00155cb6: mov r5,r1
  00155cb8: add r0,pc
  00155cba: ldr r6,[r0,#0x0]
  00155cbc: ldr r0,[r6,#0x0]
  00155cbe: str r0,[sp,#0xc]
  00155cc0: mov r0,r1
  00155cc2: blx 0x00072034
  00155cc6: cbz r0,0x00155d00
  00155cc8: mov r0,r5
  00155cca: ldr.w r8,[r4,#0x14]
  00155cce: blx 0x00072034
  00155cd2: ldr r1,[r0,#0x8]
  00155cd4: mov r0,r8
  00155cd6: blx 0x00076ac8
  00155cda: ldr r2,[0x00155d18]
  00155cdc: movs r5,#0x0
  00155cde: ldr r0,[r4,#0x14]
  00155ce0: ldr r1,[0x00155d14]
  00155ce2: str r2,[sp,#0x4]
  00155ce4: mov r8,r2
  00155ce6: str r5,[sp,#0x0]
  00155ce8: str r1,[sp,#0x8]
  00155cea: mov r1,sp
  00155cec: blx 0x00072640
  00155cf0: ldr r0,[r4,#0x14]
  00155cf2: ldr r1,[0x00155d1c]
  00155cf4: strd r5,r8,[sp,#0x0]
  00155cf8: str r1,[sp,#0x8]
  00155cfa: mov r1,sp
  00155cfc: blx 0x00076d50
  00155d00: ldr r0,[sp,#0xc]
  00155d02: ldr r1,[r6,#0x0]
  00155d04: subs r0,r1,r0
  00155d06: ittt eq
  00155d08: add.eq sp,#0x10
  00155d0a: pop.eq.w r8
  00155d0e: pop.eq {r4,r5,r6,r7,pc}
  00155d10: blx 0x0006e824
```
