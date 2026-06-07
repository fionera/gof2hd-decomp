# PlayerFighter::setPosition
elf 0xdcbb8 body 110
Sig: undefined __stdcall setPosition(float param_1, float param_2, float param_3)

## decompile
```c

/* PlayerFighter::setPosition(float, float, float) */

void PlayerFighter::setPosition(float param_1,float param_2,float param_3)

{
  int in_r0;
  int iVar1;
  Matrix *pMVar2;
  undefined4 in_r1;
  int iVar3;
  undefined4 in_r2;
  undefined4 in_r3;
  int *piVar4;
  
  piVar4 = *(int **)(DAT_000ecc28 + 0xecbce);
  iVar1 = *piVar4;
  *(undefined4 *)(in_r0 + 0x58) = in_r1;
  *(undefined4 *)(in_r0 + 0x5c) = in_r2;
  *(undefined4 *)(in_r0 + 0x60) = in_r3;
  AEGeometry::setPosition(param_1,param_2,param_3);
  AbyssEngine::AEMath::Vector::operator=((Vector *)(in_r0 + 0x158),(Vector *)&stack0xffffffd0);
  if (*(int *)(in_r0 + 0x154) != 0) {
    Trail::reset(*(int *)(in_r0 + 0x154),*(undefined4 *)(in_r0 + 0x158),
                 *(undefined4 *)(in_r0 + 0x15c),*(undefined4 *)(in_r0 + 0x160));
  }
  pMVar2 = (Matrix *)AEGeometry::getMatrix();
  AbyssEngine::AEMath::Matrix::operator=((Matrix *)(*(int *)(in_r0 + 4) + 4),pMVar2);
  iVar3 = *piVar4;
  iVar1 = iVar3 - iVar1;
  if (iVar1 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar1,iVar3);
  }
  return;
}

```

## target disasm
```
  000ecbb8: push {r4,r5,r6,r7,lr}
  000ecbba: add r7,sp,#0xc
  000ecbbc: push {r4,r5,r6,r7,r8,r9,r11}
  000ecbc0: mov r4,r0
  000ecbc2: ldr r0,[0x000ecc28]
  000ecbc4: add.w r12,r4,#0x58
  000ecbc8: mov r8,r3
  000ecbca: add r0,pc
  000ecbcc: mov r6,r2
  000ecbce: mov r5,r1
  000ecbd0: ldr.w r9,[r0,#0x0]
  000ecbd4: ldr.w r0,[r9,#0x0]
  000ecbd8: str r0,[sp,#0xc]
  000ecbda: ldr r0,[r4,#0x8]
  000ecbdc: stm.w r12!,{r1,r2,r3}
  000ecbe0: blx 0x00073048
  000ecbe4: add.w r0,r4,#0x158
  000ecbe8: mov r1,sp
  000ecbea: stm sp,{r5,r6,r8}
  000ecbee: blx 0x0006eb3c
  000ecbf2: ldr.w r0,[r4,#0x154]
  000ecbf6: cbz r0,0x000ecc02
  000ecbf8: add.w r3,r4,#0x158
  000ecbfc: ldmia r3,{r1,r2,r3}
  000ecbfe: blx 0x000757d8
  000ecc02: ldr r0,[r4,#0x8]
  000ecc04: blx 0x000721cc
  000ecc08: mov r1,r0
  000ecc0a: ldr r0,[r4,#0x4]
  000ecc0c: adds r0,#0x4
  000ecc0e: blx 0x0006f148
  000ecc12: ldr r0,[sp,#0xc]
  000ecc14: ldr.w r1,[r9,#0x0]
  000ecc18: subs r0,r1,r0
  000ecc1a: itt eq
  000ecc1c: pop.eq.w {r0,r1,r2,r3,r8,r9,r11}
  000ecc20: pop.eq {r4,r5,r6,r7,pc}
  000ecc22: blx 0x0006e824
```
