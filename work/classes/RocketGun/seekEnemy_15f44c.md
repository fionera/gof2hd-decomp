# RocketGun::seekEnemy
elf 0x15f44c body 324
Sig: undefined __stdcall seekEnemy(int param_1, int param_2)

## decompile
```c

/* RocketGun::seekEnemy(int, int) */

void RocketGun::seekEnemy(int param_1,int param_2)

{
  int iVar1;
  int iVar2;
  Player *pPVar3;
  PlayerEgo *this;
  int in_r2;
  int *piVar4;
  Vector *this_00;
  code *pcVar5;
  float fVar6;
  Vector aVStack_4c [12];
  Vector aVStack_40 [12];
  Vector aVStack_34 [12];
  int local_28;
  
  piVar4 = *(int **)(DAT_0016f590 + 0x16f460);
  local_28 = *piVar4;
  iVar1 = Gun::getEnemies(*(Gun **)(param_1 + 8));
  if ((((*(int *)(*(int *)(param_1 + 8) + 4) == 0) || (iVar2 = Gun::isPlayerGun(), iVar2 != 0)) ||
      (pPVar3 = *(Player **)(*(int *)(param_1 + 8) + 4), pPVar3 == (Player *)0x0)) ||
     ((iVar2 = Player::getKIPlayer(pPVar3), *(int *)(iVar2 + 0x38) < 0 ||
      (iVar2 = Player::getEnemies(), iVar2 == 0)))) {
    if ((((iVar1 == 0) ||
         ((*(int *)(param_1 + 0xb0) == 0 ||
          (iVar1 = *(int *)(*(int *)(param_1 + 0xb0) + 4), iVar1 == 0)))) ||
        (*(char *)(iVar1 + 0x76) == '\0')) || (*(char *)(iVar1 + 0x74) != '\0')) goto LAB_0016f57c;
    this = (PlayerEgo *)Level::getPlayer();
    iVar1 = PlayerEgo::isInFreeLookMode(this);
    if (iVar1 != 0) goto LAB_0016f57c;
    iVar1 = *(int *)(*(int *)(*(int *)(param_1 + 0xb0) + 4) + 4);
  }
  else {
    pPVar3 = *(Player **)(*(int *)(param_1 + 8) + 4);
    iVar1 = Player::getKIPlayer(pPVar3);
    iVar1 = Player::getEnemy(pPVar3,*(int *)(iVar1 + 0x38));
  }
  if (iVar1 != 0) {
    Player::getPosition();
    AbyssEngine::AEMath::operator-(aVStack_40,aVStack_4c);
    pcVar5 = *(code **)(DAT_0016f594 + 0x16f510);
    (*pcVar5)(aVStack_34,aVStack_40);
    this_00 = (Vector *)(param_1 + 0xb4);
    AbyssEngine::AEMath::Vector::operator=(this_00,aVStack_34);
    (*pcVar5)(aVStack_34,*(int *)(*(int *)(param_1 + 8) + 0x18) + in_r2 * 0xc);
    AbyssEngine::AEMath::Vector::operator-=(this_00,aVStack_34);
    AbyssEngine::AEMath::Vector::operator/=(this_00,*(float *)(param_1 + 200) * 20.0);
    AbyssEngine::AEMath::Vector::operator+=(aVStack_34,this_00);
    (*pcVar5)(aVStack_40,aVStack_34);
    fVar6 = (float)AbyssEngine::AEMath::Vector::operator=
                             ((Vector *)(*(int *)(*(int *)(param_1 + 8) + 0x18) + in_r2 * 0xc),
                              aVStack_40);
    AbyssEngine::AEMath::Vector::operator*=
              ((Vector *)(*(int *)(*(int *)(param_1 + 8) + 0x18) + in_r2 * 0xc),fVar6);
  }
LAB_0016f57c:
  if (*piVar4 != local_28) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  0016f44c: push {r4,r5,r6,r7,lr}
  0016f44e: add r7,sp,#0xc
  0016f450: push {r8,r9,r10,r11}
  0016f454: sub sp,#0x2c
  0016f456: mov r4,r0
  0016f458: ldr r0,[0x0016f590]
  0016f45a: mov r8,r2
  0016f45c: add r0,pc
  0016f45e: ldr r5,[r0,#0x0]
  0016f460: ldr r0,[r5,#0x0]
  0016f462: str r0,[sp,#0x28]
  0016f464: ldr r0,[r4,#0x8]
  0016f466: blx 0x00077980
  0016f46a: mov r6,r0
  0016f46c: ldr r0,[r4,#0x8]
  0016f46e: ldr r1,[r0,#0x4]
  0016f470: cbz r1,0x0016f4a8
  0016f472: blx 0x0007798c
  0016f476: cbnz r0,0x0016f4a8
  0016f478: ldr r0,[r4,#0x8]
  0016f47a: ldr r0,[r0,#0x4]
  0016f47c: cbz r0,0x0016f4a8
  0016f47e: blx 0x00075850
  0016f482: ldr r0,[r0,#0x38]
  0016f484: cmp r0,#0x0
  0016f486: blt 0x0016f4a8
  0016f488: ldr r0,[r4,#0x8]
  0016f48a: ldr r0,[r0,#0x4]
  0016f48c: blx 0x0007582c
  0016f490: cbz r0,0x0016f4a8
  0016f492: ldr r0,[r4,#0x8]
  0016f494: ldr r6,[r0,#0x4]
  0016f496: mov r0,r6
  0016f498: blx 0x00075850
  0016f49c: ldr r1,[r0,#0x38]
  0016f49e: mov r0,r6
  0016f4a0: blx 0x00075874
  0016f4a4: mov r1,r0
  0016f4a6: b 0x0016f4e0
  0016f4a8: cmp r6,#0x0
  0016f4aa: beq 0x0016f57c
  0016f4ac: ldr.w r0,[r4,#0xb0]
  0016f4b0: cmp r0,#0x0
  0016f4b2: beq 0x0016f57c
  0016f4b4: ldr r1,[r0,#0x4]
  0016f4b6: cmp r1,#0x0
  0016f4b8: beq 0x0016f57c
  0016f4ba: ldrb.w r2,[r1,#0x76]
  0016f4be: cmp r2,#0x0
  0016f4c0: beq 0x0016f57c
  0016f4c2: ldrb.w r1,[r1,#0x74]
  0016f4c6: cmp r1,#0x0
  0016f4c8: bne 0x0016f57c
  0016f4ca: ldr r0,[r0,#0x0]
  0016f4cc: blx 0x00072034
  0016f4d0: blx 0x00077998
  0016f4d4: cmp r0,#0x0
  0016f4d6: bne 0x0016f57c
  0016f4d8: ldr.w r0,[r4,#0xb0]
  0016f4dc: ldr r0,[r0,#0x4]
  0016f4de: ldr r1,[r0,#0x4]
  0016f4e0: cmp r1,#0x0
  0016f4e2: beq 0x0016f57c
  0016f4e4: add r6,sp,#0x4
  0016f4e6: mov r0,r6
  0016f4e8: blx 0x00072964
  0016f4ec: ldr r0,[r4,#0x8]
  0016f4ee: add.w r10,r8,r8, lsl #0x1
  0016f4f2: add.w r8,sp,#0x10
  0016f4f6: mov r1,r6
  0016f4f8: ldr r0,[r0,#0xc]
  0016f4fa: add.w r2,r0,r10, lsl #0x2
  0016f4fe: mov r0,r8
  0016f500: blx 0x0006ec38
  0016f504: ldr r0,[0x0016f594]
  0016f506: add.w r9,sp,#0x1c
  0016f50a: mov r1,r8
  0016f50c: add r0,pc
  0016f50e: ldr.w r11,[r0,#0x0]
  0016f512: mov r0,r9
  0016f514: blx r11
  0016f516: add.w r6,r4,#0xb4
  0016f51a: mov r1,r9
  0016f51c: mov r0,r6
  0016f51e: blx 0x0006eb3c
  0016f522: ldr r0,[r4,#0x8]
  0016f524: add.w r8,sp,#0x1c
  0016f528: ldr r0,[r0,#0x18]
  0016f52a: add.w r1,r0,r10, lsl #0x2
  0016f52e: mov r0,r8
  0016f530: blx r11
  0016f532: mov r0,r6
  0016f534: mov r1,r8
  0016f536: blx 0x00072694
  0016f53a: vmov.f32 s0,0x41a00000
  0016f53e: vldr.32 s2,[r4,#0xc8]
  0016f542: mov r0,r6
  0016f544: vmul.f32 s0,s2,s0
  0016f548: vmov r1,s0
  0016f54c: blx 0x000779a4
  0016f550: mov r0,r8
  0016f552: mov r1,r6
  0016f554: blx 0x00073534
  0016f558: add r6,sp,#0x10
  0016f55a: mov r1,r8
  0016f55c: mov r0,r6
  0016f55e: blx r11
  0016f560: ldr r0,[r4,#0x8]
  0016f562: mov r1,r6
  0016f564: ldr r0,[r0,#0x18]
  0016f566: add.w r0,r0,r10, lsl #0x2
  0016f56a: blx 0x0006eb3c
  0016f56e: ldr r0,[r4,#0x8]
  0016f570: ldr r2,[r0,#0x18]
  0016f572: ldr r1,[r0,#0x50]
  0016f574: add.w r0,r2,r10, lsl #0x2
  0016f578: blx 0x00072628
  0016f57c: ldr r0,[sp,#0x28]
  0016f57e: ldr r1,[r5,#0x0]
  0016f580: subs r0,r1,r0
  0016f582: ittt eq
  0016f584: add.eq sp,#0x2c
  0016f586: pop.eq.w {r8,r9,r10,r11}
  0016f58a: pop.eq {r4,r5,r6,r7,pc}
  0016f58c: blx 0x0006e824
```
