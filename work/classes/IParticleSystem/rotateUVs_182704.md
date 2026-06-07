# IParticleSystem::rotateUVs
elf 0x182704 body 122
Sig: undefined __stdcall rotateUVs(float * param_1, int param_2, float * param_3)

## decompile
```c

/* IParticleSystem::rotateUVs(float*, int, float*) */

void IParticleSystem::rotateUVs(float *param_1,int param_2,float *param_3)

{
  int iVar1;
  uint uVar2;
  undefined4 *in_r3;
  int *piVar3;
  AERandom aAStack_28 [12];
  int local_1c;
  
  piVar3 = *(int **)(DAT_0019278c + 0x19271a);
  local_1c = *piVar3;
  iVar1 = AbyssEngine::AERandom::AERandom(aAStack_28,(longlong)(int)param_3);
  uVar2 = AbyssEngine::AERandom::nextInt(iVar1);
  *in_r3 = *(undefined4 *)(param_2 + (uVar2 & 1) * 4);
  in_r3[1] = *(undefined4 *)(param_2 + (~uVar2 & 1) * 4);
  in_r3[2] = *(undefined4 *)(param_2 + ((uVar2 & 2) << 1 | 8));
  in_r3[3] = *(undefined4 *)(param_2 + ((~uVar2 & 2) << 1 | 8));
  AbyssEngine::AERandom::~AERandom(aAStack_28);
  if (*piVar3 != local_1c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  00192704: push {r4,r5,r6,r7,lr}
  00192706: add r7,sp,#0xc
  00192708: push.w r11
  0019270c: sub sp,#0x10
  0019270e: ldr r0,[0x0019278c]
  00192710: mov r4,r3
  00192712: asrs r3,r2,#0x1f
  00192714: mov r5,r1
  00192716: add r0,pc
  00192718: ldr r6,[r0,#0x0]
  0019271a: ldr r0,[r6,#0x0]
  0019271c: str r0,[sp,#0xc]
  0019271e: mov r0,sp
  00192720: blx 0x00078880
  00192724: movw r1,#0x9c40
  00192728: blx 0x00071848
  0019272c: and r1,r0,#0x1
  00192730: ldr.w r1,[r5,r1,lsl #0x2]
  00192734: str r1,[r4,#0x0]
  00192736: mvn r1,#0x3
  0019273a: orn.w r1,r1,r0
  0019273e: and r2,r1,#0x1
  00192742: ldr.w r2,[r5,r2,lsl #0x2]
  00192746: str r2,[r4,#0x4]
  00192748: movs r2,#0x4
  0019274a: and.w r0,r2,r0, lsl #0x1
  0019274e: orr r0,r0,#0x8
  00192752: ldr r0,[r5,r0]
  00192754: str r0,[r4,#0x8]
  00192756: and.w r0,r2,r1, lsl #0x1
  0019275a: orr r0,r0,#0x8
  0019275e: ldr r0,[r5,r0]
  00192760: str r0,[r4,#0xc]
  00192762: mov r0,sp
  00192764: blx 0x00075cc4
  00192768: ldr r0,[sp,#0xc]
  0019276a: ldr r1,[r6,#0x0]
  0019276c: subs r0,r1,r0
  0019276e: itttt eq
  00192770: mov.eq r0,r4
  00192772: add.eq sp,#0x10
  00192774: pop.eq.w r11
  00192778: pop.eq {r4,r5,r6,r7,pc}
  0019277a: blx 0x0006e824
```
