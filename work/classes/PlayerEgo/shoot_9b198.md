# PlayerEgo::shoot
elf 0x9b198 body 256
Sig: undefined __stdcall shoot(int param_1, int param_2)

## decompile
```c

/* PlayerEgo::shoot(int, int) */

void PlayerEgo::shoot(int param_1,int param_2)

{
  Matrix *pMVar1;
  int iVar2;
  int in_r2;
  int *piVar3;
  undefined4 local_64;
  undefined4 local_60;
  undefined4 local_5c;
  undefined4 local_58;
  undefined4 local_54;
  undefined4 uStack_50;
  undefined4 local_4c;
  undefined4 uStack_48;
  undefined4 local_44;
  undefined4 uStack_40;
  undefined4 local_3c;
  undefined4 uStack_38;
  undefined4 local_34;
  undefined4 uStack_30;
  undefined4 uStack_2c;
  int local_28;
  
  piVar3 = *(int **)(DAT_000ab298 + 0xab1ae);
  local_28 = *piVar3;
  if (*(char *)(param_1 + 0x1a0) == '\0') {
    iVar2 = isDead((PlayerEgo *)param_1);
    if (iVar2 == 0) {
      if (in_r2 == 1) {
        iVar2 = Player::shoot(*(int *)param_1,1,
                              CONCAT44(param_2 >> 0x1f,*(undefined4 *)(param_1 + 0x10c)),
                              SUB41(param_2,0));
        if (iVar2 == 0) {
          *(undefined4 *)(param_1 + 0x10c) = 0xffffffff;
        }
      }
      else {
        Player::shoot(*(int *)param_1,(longlong)param_2,false);
      }
    }
  }
  else {
    pMVar1 = (Matrix *)AEGeometry::getMatrix();
    AEGeometry::getMatrix();
    AbyssEngine::AEMath::operator*((Matrix *)&local_64,pMVar1);
    Player::shoot(*(undefined4 *)param_1,2,param_2,param_2 >> 0x1f,0,local_64,local_60,local_5c,
                  local_58,local_54,uStack_50,local_4c,uStack_48,local_44,uStack_40,local_3c,
                  uStack_38,local_34,uStack_30,uStack_2c);
  }
  if (*piVar3 != local_28) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000ab198: push {r4,r5,r6,r7,lr}
  000ab19a: add r7,sp,#0xc
  000ab19c: push {r8,r9,r10,r11}
  000ab1a0: sub sp,#0x94
  000ab1a2: mov r5,r0
  000ab1a4: ldr r0,[0x000ab298]
  000ab1a6: mov r6,r2
  000ab1a8: mov r8,r1
  000ab1aa: add r0,pc
  000ab1ac: ldr.w r11,[r0,#0x0]
  000ab1b0: ldr.w r0,[r11,#0x0]
  000ab1b4: str r0,[sp,#0x90]
  000ab1b6: ldrb.w r0,[r5,#0x1a0]
  000ab1ba: cmp r0,#0x0
  000ab1bc: beq 0x000ab242
  000ab1be: ldr r0,[r5,#0x8]
  000ab1c0: blx 0x000721cc
  000ab1c4: mov r6,r0
  000ab1c6: ldr r0,[r5,#0x28]
  000ab1c8: blx 0x000721cc
  000ab1cc: mov r2,r0
  000ab1ce: add r0,sp,#0x54
  000ab1d0: mov r1,r6
  000ab1d2: blx 0x0006f82c
  000ab1d6: ldr r0,[sp,#0x54]
  000ab1d8: add.w r12,sp,#0x84
  000ab1dc: str r0,[sp,#0x4c]
  000ab1de: mov lr,r8
  000ab1e0: ldr r0,[sp,#0x58]
  000ab1e2: str r0,[sp,#0x48]
  000ab1e4: ldr r0,[sp,#0x5c]
  000ab1e6: str r0,[sp,#0x44]
  000ab1e8: ldr r0,[sp,#0x60]
  000ab1ea: str r0,[sp,#0x40]
  000ab1ec: ldr r0,[r5,#0x0]
  000ab1ee: str r0,[sp,#0x50]
  000ab1f0: ldrd r5,r4,[sp,#0x64]
  000ab1f4: ldrd r10,r2,[sp,#0x6c]
  000ab1f8: ldrd r3,r6,[sp,#0x74]
  000ab1fc: ldrd r8,r9,[sp,#0x7c]
  000ab200: ldm.w r12,{r0,r1,r12}
  000ab204: str r2,[sp,#0x20]
  000ab206: add r2,sp,#0x34
  000ab208: str r3,[sp,#0x24]
  000ab20a: asr.w r3,lr, asr #0x1f
  000ab20e: str r6,[sp,#0x28]
  000ab210: str.w r8,[sp,#0x2c]
  000ab214: str.w r9,[sp,#0x30]
  000ab218: stm r2,{r0,r1,r12}
  000ab21c: movs r0,#0x0
  000ab21e: movs r1,#0x2
  000ab220: str r0,[sp,#0x0]
  000ab222: mov r2,lr
  000ab224: ldr r0,[sp,#0x4c]
  000ab226: str r0,[sp,#0x4]
  000ab228: ldr r0,[sp,#0x48]
  000ab22a: str r0,[sp,#0x8]
  000ab22c: ldr r0,[sp,#0x44]
  000ab22e: str r0,[sp,#0xc]
  000ab230: ldr r0,[sp,#0x40]
  000ab232: strd r0,r5,[sp,#0x10]
  000ab236: strd r4,r10,[sp,#0x18]
  000ab23a: ldr r0,[sp,#0x50]
  000ab23c: blx 0x000724fc
  000ab240: b 0x000ab282
  000ab242: mov r0,r5
  000ab244: blx 0x000724b4
  000ab248: cbnz r0,0x000ab282
  000ab24a: ldr r0,[r5,#0x0]
  000ab24c: cmp r6,#0x1
  000ab24e: bne 0x000ab272
  000ab250: ldr.w r2,[r5,#0x10c]
  000ab254: asr.w r3,r8, asr #0x1f
  000ab258: movs r1,#0x0
  000ab25a: strd r8,r3,[sp,#0x0]
  000ab25e: str r1,[sp,#0x8]
  000ab260: movs r1,#0x1
  000ab262: blx 0x00072508
  000ab266: cbnz r0,0x000ab282
  000ab268: mov.w r0,#0xffffffff
  000ab26c: str.w r0,[r5,#0x10c]
  000ab270: b 0x000ab282
  000ab272: movs r1,#0x0
  000ab274: asr.w r3,r8, asr #0x1f
  000ab278: str r1,[sp,#0x0]
  000ab27a: mov r1,r6
  000ab27c: mov r2,r8
  000ab27e: blx 0x00072514
  000ab282: ldr r0,[sp,#0x90]
  000ab284: ldr.w r1,[r11,#0x0]
  000ab288: subs r0,r1,r0
  000ab28a: ittt eq
  000ab28c: add.eq sp,#0x94
  000ab28e: pop.eq.w {r8,r9,r10,r11}
  000ab292: pop.eq {r4,r5,r6,r7,pc}
  000ab294: blx 0x0006e824
```
