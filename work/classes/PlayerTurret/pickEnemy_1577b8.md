# PlayerTurret::pickEnemy
elf 0x1577b8 body 252
Sig: undefined __thiscall pickEnemy(PlayerTurret * this)

## decompile
```c

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Removing unreachable block (ram,0xfe9c2252) */
/* PlayerTurret::pickEnemy() */

void __thiscall PlayerTurret::pickEnemy(PlayerTurret *this)

{
  uint *puVar1;
  int iVar2;
  float fVar3;
  undefined4 extraout_r2;
  undefined4 extraout_r3;
  Player *this_00;
  int iVar4;
  uint uVar5;
  undefined1 *unaff_r10;
  undefined4 extraout_r12;
  undefined8 uVar6;
  int *local_48;
  Vector *local_44;
  undefined1 auStack_40 [12];
  Vector aVStack_34 [12];
  int local_28;
  
  local_48 = *(int **)(DAT_00167904 + 0x1677ce);
  local_28 = *local_48;
  if (3000 < *(int *)(this + 0x130)) {
    iVar4 = *(int *)(this + 0x164);
    *(undefined4 *)(this + 0x130) = 0;
    *(undefined4 *)(this + 0x14c) = 0;
    puVar1 = (uint *)Player::getEnemies();
    if (puVar1 != (uint *)0x0) {
      local_44 = (Vector *)(this + 0x2c);
      for (uVar5 = 0; uVar5 < *puVar1; uVar5 = uVar5 + 1) {
        this_00 = *(Player **)(puVar1[1] + uVar5 * 4);
        iVar2 = Player::isDead();
        if ((iVar2 == 0) && (iVar2 = Player::isActive(), iVar2 != 0)) {
          if ((this_00[0x69] == (Player)0x0) || (*(char *)(*(int *)(this + 4) + 0x5c) == '\0')) {
            if (this[0x3f] == (PlayerTurret)0x0) {
              uVar6 = Player::getKIPlayer(this_00);
              iVar2 = (int)((ulonglong)uVar6 >> 0x20);
              if ((int)uVar6 != 0) {
                *(undefined **)this = &UNK_0016784e;
                *(undefined4 *)(this + -4) = 0x167841;
                *(int ***)(this + -8) = &local_48;
                *(undefined4 *)(this + -0xc) = extraout_r12;
                *(undefined1 **)(this + -0x10) = unaff_r10;
                *(uint *)(this + -0x14) = uVar5;
                *(int *)(this + -0x18) = iVar4;
                *(undefined4 *)(this + -0x1c) = extraout_r3;
                *(undefined4 *)(this + -0x20) = extraout_r2;
                *(int *)(this + -0x24) = iVar2;
                if ((((((this_00 == (Player *)0x8) && (iVar2 != 8)) ||
                      ((this_00 != (Player *)0x8 && (iVar2 == 8)))) ||
                     ((this_00 == (Player *)0xa && (iVar2 != 10)))) ||
                    ((this_00 != (Player *)0xa && (iVar2 == 10)))) ||
                   ((((this_00 == (Player *)0x9 && (iVar2 != 9)) ||
                     ((this_00 != (Player *)0x9 && (iVar2 == 9)))) ||
                    ((((this_00 == (Player *)0x0 && (iVar2 == 1)) ||
                      (this_00 == (Player *)0x1 && iVar2 == 0)) ||
                     ((this_00 == (Player *)0x3 && iVar2 == 2 ||
                      (this_00 == (Player *)0x2 && iVar2 == 3)))))))) goto LAB_0016789c;
              }
            }
            else if (this_00[0x5c] != (Player)0x0) goto LAB_0016789c;
          }
          else {
LAB_0016789c:
            unaff_r10 = auStack_40;
            Player::getPosition();
            AbyssEngine::AEMath::operator-(aVStack_34,local_44);
            fVar3 = (float)AbyssEngine::AEMath::VectorLength(aVStack_34);
            if (((int)fVar3 < iVar4) &&
               ((*(int *)(this + 0x14c) == 0 || (*(int *)(this + 0x14c) != *(int *)(this + 0x150))))
               ) {
              *(Player **)(this + 0x14c) = this_00;
              iVar4 = (int)fVar3;
            }
          }
        }
      }
    }
  }
  if (*local_48 != local_28) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  001677b8: push {r4,r5,r6,r7,lr}
  001677ba: add r7,sp,#0xc
  001677bc: push {r8,r9,r10,r11}
  001677c0: sub sp,#0x24
  001677c2: mov r4,r0
  001677c4: ldr r0,[0x00167904]
  001677c6: movw r1,#0xbb9
  001677ca: add r0,pc
  001677cc: ldr r0,[r0,#0x0]
  001677ce: str r0,[sp,#0x0]
  001677d0: ldr r0,[r0,#0x0]
  001677d2: str r0,[sp,#0x20]
  001677d4: ldr.w r0,[r4,#0x130]
  001677d8: cmp r0,r1
  001677da: blt.w 0x001678ec
  001677de: ldr r0,[r4,#0x4]
  001677e0: movs r1,#0x0
  001677e2: ldr.w r8,[r4,#0x164]
  001677e6: str.w r1,[r4,#0x130]
  001677ea: str.w r1,[r4,#0x14c]
  001677ee: blx 0x0007582c
  001677f2: mov r5,r0
  001677f4: cmp r0,#0x0
  001677f6: beq 0x001678ec
  001677f8: mov.w r9,#0x0
  001677fc: add.w r0,r4,#0x2c
  00167800: str r0,[sp,#0x4]
  00167802: b 0x001678e4
  00167804: ldr r0,[r5,#0x4]
  00167806: ldr.w r6,[r0,r9,lsl #0x2]
  0016780a: mov r0,r6
  0016780c: blx 0x00075838
  00167810: cmp r0,#0x0
  00167812: bne 0x001678e0
  00167814: mov r0,r6
  00167816: blx 0x00071f14
  0016781a: cmp r0,#0x0
  0016781c: beq 0x001678e0
  0016781e: ldrb.w r0,[r6,#0x69]
  00167822: cbz r0,0x0016782c
  00167824: ldr r0,[r4,#0x4]
  00167826: ldrb.w r0,[r0,#0x5c]
  0016782a: cbnz r0,0x0016789c
  0016782c: ldrb.w r0,[r4,#0x3f]
  00167830: cbz r0,0x0016783a
  00167832: ldrb.w r0,[r6,#0x5c]
  00167836: cbnz r0,0x0016789c
  00167838: b 0x001678e0
  0016783a: mov r0,r6
  0016783c: blx 0x00075850
  00167840: cmp r0,#0x0
  00167842: beq 0x001678e0
  00167844: mov r0,r6
  00167846: stmda r4,{r1,r2,r3,r8,r9,r10,r12,sp,lr,pc}
  0016784a: bvs 0xfe9c2252
  0016789c: add.w r10,sp,#0x8
  001678a0: mov r1,r6
  001678a2: mov r0,r10
  001678a4: blx 0x00072964
  001678a8: add.w r11,sp,#0x14
  001678ac: ldr r1,[sp,#0x4]
  001678ae: mov r2,r10
  001678b0: mov r0,r11
  001678b2: blx 0x0006ec38
  001678b6: mov r0,r11
  001678b8: blx 0x0006ec44
  001678bc: vmov s0,r0
  001678c0: vcvt.s32.f32 s0,s0
  001678c4: vmov r0,s0
  001678c8: cmp r8,r0
  001678ca: ble 0x001678e0
  001678cc: ldr.w r1,[r4,#0x14c]
  001678d0: cbz r1,0x001678da
  001678d2: ldr.w r2,[r4,#0x150]
  001678d6: cmp r1,r2
  001678d8: beq 0x001678e0
  001678da: mov r8,r0
  001678dc: str.w r6,[r4,#0x14c]
  001678e0: add.w r9,r9,#0x1
  001678e4: ldr r0,[r5,#0x0]
  001678e6: cmp r9,r0
  001678e8: bcc.w 0x00167804
  001678ec: ldr r0,[sp,#0x20]
  001678ee: ldr r1,[sp,#0x0]
  001678f0: ldr r1,[r1,#0x0]
  001678f2: subs r0,r1,r0
  001678f4: ittt eq
  001678f6: add.eq sp,#0x24
  001678f8: pop.eq.w {r8,r9,r10,r11}
  001678fc: pop.eq {r4,r5,r6,r7,pc}
  001678fe: blx 0x0006e824
```
