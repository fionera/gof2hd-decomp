# KIPlayer::createCrate
elf 0xa622c body 194
Sig: undefined __stdcall createCrate(int param_1)

## decompile
```c

/* KIPlayer::createCrate(int) */

void KIPlayer::createCrate(int param_1)

{
  ushort uVar1;
  AEGeometry *this;
  Matrix *pMVar2;
  int in_r1;
  int iVar3;
  int *piVar4;
  Vector aVStack_28 [12];
  int local_1c;
  
  piVar4 = *(int **)(DAT_000b62fc + 0xb6240);
  local_1c = *piVar4;
  this = operator_new(0xc0);
  if (in_r1 == 1) {
    uVar1 = 0x421e;
  }
  else if (in_r1 == 2) {
    uVar1 = 0x421f;
  }
  else if (in_r1 == 3) {
    uVar1 = 0x4218;
  }
  else {
    iVar3 = *(int *)(param_1 + 0x28);
    if (iVar3 == 1) {
      uVar1 = 0x425f;
    }
    else if (iVar3 == 3) {
      uVar1 = 0x425e;
    }
    else if (iVar3 == 9) {
      uVar1 = 0x4214;
    }
    else {
      uVar1 = 0x4261;
      if (iVar3 == 0) {
        uVar1 = 0x4260;
      }
    }
  }
  AEGeometry::AEGeometry(this,uVar1,(PaintCanvas *)**(undefined4 **)(DAT_000b6300 + 0xb62a0),false);
  *(AEGeometry **)(param_1 + 0x78) = this;
  AEGeometry::getPosition();
  AEGeometry::setPosition(this,aVStack_28);
  pMVar2 = (Matrix *)AEGeometry::getMatrix();
  AbyssEngine::AEMath::Matrix::operator=((Matrix *)(*(int *)(param_1 + 4) + 4),pMVar2);
  Player::setKIPlayer(*(Player **)(param_1 + 4),(KIPlayer *)param_1);
  if (*piVar4 != local_1c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000b622c: push {r4,r5,r6,r7,lr}
  000b622e: add r7,sp,#0xc
  000b6230: push.w r8
  000b6234: sub sp,#0x10
  000b6236: mov r4,r0
  000b6238: ldr r0,[0x000b62fc]
  000b623a: mov r6,r1
  000b623c: add r0,pc
  000b623e: ldr.w r8,[r0,#0x0]
  000b6242: ldr.w r0,[r8,#0x0]
  000b6246: str r0,[sp,#0xc]
  000b6248: movs r0,#0xc0
  000b624a: blx 0x0006eb24
  000b624e: mov r5,r0
  000b6250: cmp r6,#0x1
  000b6252: beq 0x000b6262
  000b6254: cmp r6,#0x2
  000b6256: beq 0x000b6268
  000b6258: cmp r6,#0x3
  000b625a: bne 0x000b626e
  000b625c: movw r0,#0x4218
  000b6260: b 0x000b629a
  000b6262: movw r0,#0x421e
  000b6266: b 0x000b629a
  000b6268: movw r0,#0x421f
  000b626c: b 0x000b629a
  000b626e: ldr r1,[r4,#0x28]
  000b6270: cmp r1,#0x1
  000b6272: beq 0x000b6288
  000b6274: cmp r1,#0x3
  000b6276: beq 0x000b6282
  000b6278: cmp r1,#0x9
  000b627a: bne 0x000b628e
  000b627c: movw r0,#0x4214
  000b6280: b 0x000b629a
  000b6282: movw r0,#0x425e
  000b6286: b 0x000b629a
  000b6288: movw r0,#0x425f
  000b628c: b 0x000b629a
  000b628e: movw r0,#0x4261
  000b6292: cmp r1,#0x0
  000b6294: it eq
  000b6296: movw.eq r0,#0x4260
  000b629a: ldr r1,[0x000b6300]
  000b629c: add r1,pc
  000b629e: ldr r1,[r1,#0x0]
  000b62a0: ldr r2,[r1,#0x0]
  000b62a2: uxth r1,r0
  000b62a4: mov r0,r5
  000b62a6: movs r3,#0x0
  000b62a8: blx 0x0007207c
  000b62ac: str r5,[r4,#0x78]
  000b62ae: mov r6,sp
  000b62b0: ldr r1,[r4,#0x8]
  000b62b2: mov r0,r6
  000b62b4: blx 0x000720b8
  000b62b8: mov r0,r5
  000b62ba: mov r1,r6
  000b62bc: blx 0x00072148
  000b62c0: ldr r0,[r4,#0x78]
  000b62c2: blx 0x000721cc
  000b62c6: mov r1,r0
  000b62c8: ldr r0,[r4,#0x4]
  000b62ca: adds r0,#0x4
  000b62cc: blx 0x0006f148
  000b62d0: ldr r0,[r4,#0x4]
  000b62d2: mov r1,r4
  000b62d4: blx 0x00073288
  000b62d8: ldr r0,[sp,#0xc]
  000b62da: ldr.w r1,[r8,#0x0]
  000b62de: subs r0,r1,r0
  000b62e0: ittt eq
  000b62e2: add.eq sp,#0x10
  000b62e4: pop.eq.w r8
  000b62e8: pop.eq {r4,r5,r6,r7,pc}
  000b62ea: blx 0x0006e824
```
