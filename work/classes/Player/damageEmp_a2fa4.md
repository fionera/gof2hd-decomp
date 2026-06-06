# Player::damageEmp
elf 0xa2fa4 body 518
Sig: undefined __thiscall damageEmp(Player * this, int param_1, bool param_2)

## decompile
```c

/* Player::damageEmp(int, bool) */

void __thiscall Player::damageEmp(Player *this,int param_1,bool param_2)

{
  int iVar1;
  int iVar2;
  PlayerEgo *this_00;
  Hud *this_01;
  Standing *this_02;
  undefined4 *puVar3;
  int *piVar4;
  uint in_fpscr;
  undefined4 uVar5;
  
  if ((this[0xc2] == (Player)0x0) || (this[0xc0] == (Player)0x0)) {
    return;
  }
  iVar1 = *(int *)(this + 0x7c);
  if (0 < iVar1) {
    iVar1 = *(int *)(this + 0x78);
  }
  if (iVar1 < 1) {
    return;
  }
  if (((!param_2) && (*(int *)(this + 0xd0) != 0)) && (this[0xec] == (Player)0x0)) {
    if (*(int *)(*(int *)(this + 0xd0) + 0x28) - 9U < 2) {
LAB_000b3016:
      if (((this[0xec] == (Player)0x0) && (iVar1 = KIPlayer::isWingMan(), iVar1 == 0)) &&
         ((1 < *(int *)(*(int *)(this + 0xd0) + 0x28) - 9U &&
          (iVar1 = Status::getSystem(), iVar1 != 0)))) {
        iVar2 = *(int *)(*(int *)(this + 0xd0) + 0x28);
        Status::getSystem();
        iVar1 = SolarSystem::getRace();
        if (iVar2 == iVar1) {
          iVar2 = *(int *)(this + 0xdc);
          *(int *)(this + 0xdc) = iVar2 + param_1;
          iVar1 = __aeabi_idiv(*(undefined4 *)(this + 0x80),3);
          if (iVar1 < iVar2 + param_1) {
            this[0xe0] = (Player)0x1;
            Level::friendTurnedEnemy(*(int *)(*(int *)(this + 0xd0) + 0x54));
          }
        }
      }
    }
    else {
      iVar1 = Status::getSystem();
      iVar2 = *(int *)(this + 0xd0);
      if ((iVar1 == 0) || (*(char *)(iVar2 + 0x42) == '\0')) {
        if (iVar2 != 0) goto LAB_000b3016;
      }
      else if (0 < param_1) {
        Level::attackWanted(*(Level **)(iVar2 + 0x54),*(int *)(iVar2 + 0x48));
      }
    }
  }
  iVar1 = *(int *)(this + 0x7c);
  *(int *)(this + 0x7c) = iVar1 - param_1;
  if (0 < iVar1 - param_1) goto LAB_000b317e;
  if ((!param_2) && (*(int *)(this + 0xd0) != 0)) {
    if ((this[0xec] == (Player)0x0) &&
       ((1 < *(int *)(*(int *)(this + 0xd0) + 0x28) - 9U &&
        (iVar1 = Status::getSystem(), iVar1 != 0)))) {
      iVar2 = *(int *)(*(int *)(this + 0xd0) + 0x28);
      Status::getSystem();
      iVar1 = SolarSystem::getRace();
      if (iVar2 == iVar1) {
        Level::alarmAllFriends
                  (*(Level **)(*(int *)(this + 0xd0) + 0x54),*(int *)(*(int *)(this + 0xd0) + 0x28),
                   false);
      }
    }
    iVar1 = *(int *)(this + 0xd0);
    if (*(char *)(iVar1 + 0x3c) == '\0') {
      if ((*(char *)(iVar1 + 0x3d) == '\0') && (*(char *)(iVar1 + 0x42) == '\0')) {
        this_02 = (Standing *)Status::getStanding();
        Standing::applyDisable(this_02,*(int *)(*(int *)(this + 0xd0) + 0x28));
        iVar1 = *(int *)(this + 0xd0);
        goto LAB_000b30e2;
      }
    }
    else {
LAB_000b30e2:
      if (iVar1 == 0) goto LAB_000b3164;
    }
    if (this[0x68] == (Player)0x0) {
      piVar4 = *(int **)(DAT_000b31b8 + 0xb3102);
      if (0x7fffffff < *(uint *)(*piVar4 + 0x134)) {
        *(undefined4 *)(*piVar4 + 0x134) = 0;
      }
      puVar3 = *(undefined4 **)(DAT_000b31bc + 0xb3120);
      iVar1 = Achievements::hasMedal((Achievements *)*puVar3,0x2a,1);
      if (iVar1 == 0) {
        iVar2 = *(int *)(*piVar4 + 0x134) + 1;
        *(int *)(*piVar4 + 0x134) = iVar2;
        iVar1 = Achievements::getValue((Achievements *)*puVar3,0x2a,1);
        if (iVar1 <= iVar2) {
          this_00 = (PlayerEgo *)Level::getPlayer();
          this_01 = (Hud *)PlayerEgo::getHUD(this_00);
          Hud::hudEventMedal(this_01,0x2a,100);
          *(undefined1 *)(*piVar4 + 0x138) = 1;
        }
      }
    }
  }
LAB_000b3164:
  uVar5 = VectorSignedToFloat(*(undefined4 *)(this + 0xe4),(byte)(in_fpscr >> 0x16) & 3);
  this[0x68] = (Player)0x1;
  *(undefined4 *)(this + 0x7c) = 0;
  *(undefined4 *)(this + 0xe8) = 0;
  *(undefined4 *)(this + 0xd8) = uVar5;
LAB_000b317e:
  (*(code *)(DAT_001abce4 + 0x1abce8))(this);
  return;
}

```
## target disasm
```
  000b2fa4: push {r4,r5,r6,r7,lr}
  000b2fa6: add r7,sp,#0xc
  000b2fa8: push {r8,r9,r11}
  000b2fac: mov r4,r0
  000b2fae: ldrb.w r0,[r0,#0xc2]
  000b2fb2: mov r5,r2
  000b2fb4: mov r6,r1
  000b2fb6: cbz r0,0x000b2fca
  000b2fb8: ldrb.w r0,[r4,#0xc0]
  000b2fbc: cbz r0,0x000b2fca
  000b2fbe: ldr r0,[r4,#0x7c]
  000b2fc0: cmp r0,#0x1
  000b2fc2: itt ge
  000b2fc4: ldr.ge r0,[r4,#0x78]
  000b2fc6: cmp.ge r0,#0x1
  000b2fc8: bge 0x000b2fd0
  000b2fca: pop.w {r8,r9,r11}
  000b2fce: pop {r4,r5,r6,r7,pc}
  000b2fd0: cmp r5,#0x0
  000b2fd2: bne 0x000b3084
  000b2fd4: ldr.w r0,[r4,#0xd0]
  000b2fd8: cmp r0,#0x0
  000b2fda: beq 0x000b3084
  000b2fdc: ldrb.w r1,[r4,#0xec]
  000b2fe0: cmp r1,#0x0
  000b2fe2: bne 0x000b3084
  000b2fe4: ldr r1,[r0,#0x28]
  000b2fe6: subs r1,#0x9
  000b2fe8: cmp r1,#0x2
  000b2fea: bcc 0x000b3016
  000b2fec: ldr r0,[0x000b31a8]
  000b2fee: add r0,pc
  000b2ff0: ldr r0,[r0,#0x0]
  000b2ff2: ldr r0,[r0,#0x0]
  000b2ff4: blx 0x00071a10
  000b2ff8: mov r1,r0
  000b2ffa: ldr.w r0,[r4,#0xd0]
  000b2ffe: cbz r1,0x000b3014
  000b3000: ldrb.w r1,[r0,#0x42]
  000b3004: cbz r1,0x000b3014
  000b3006: cmp r6,#0x1
  000b3008: blt 0x000b3084
  000b300a: ldr r1,[r0,#0x48]
  000b300c: ldr r0,[r0,#0x54]
  000b300e: blx 0x00072ec8
  000b3012: b 0x000b3084
  000b3014: cbz r0,0x000b3084
  000b3016: ldrb.w r1,[r4,#0xec]
  000b301a: cbnz r1,0x000b3084
  000b301c: blx 0x00072ed4
  000b3020: cbnz r0,0x000b3084
  000b3022: ldr.w r0,[r4,#0xd0]
  000b3026: ldr r0,[r0,#0x28]
  000b3028: subs r0,#0x9
  000b302a: cmp r0,#0x2
  000b302c: bcc 0x000b3084
  000b302e: ldr r0,[0x000b31ac]
  000b3030: add r0,pc
  000b3032: ldr.w r8,[r0,#0x0]
  000b3036: ldr.w r0,[r8,#0x0]
  000b303a: blx 0x00071a10
  000b303e: cbz r0,0x000b3084
  000b3040: ldr.w r0,[r4,#0xd0]
  000b3044: ldr.w r9,[r0,#0x28]
  000b3048: ldr.w r0,[r8,#0x0]
  000b304c: blx 0x00071a10
  000b3050: blx 0x00071950
  000b3054: cmp r9,r0
  000b3056: bne 0x000b3084
  000b3058: ldr.w r1,[r4,#0xdc]
  000b305c: ldr.w r0,[r4,#0x80]
  000b3060: add.w r8,r1,r6
  000b3064: movs r1,#0x3
  000b3066: str.w r8,[r4,#0xdc]
  000b306a: blx 0x0007198c
  000b306e: cmp r8,r0
  000b3070: ble 0x000b3084
  000b3072: ldr.w r0,[r4,#0xd0]
  000b3076: movs r1,#0x1
  000b3078: strb.w r1,[r4,#0xe0]
  000b307c: ldr r1,[r0,#0x28]
  000b307e: ldr r0,[r0,#0x54]
  000b3080: blx 0x00072ee0
  000b3084: ldr r0,[r4,#0x7c]
  000b3086: subs r0,r0,r6
  000b3088: str r0,[r4,#0x7c]
  000b308a: cmp r0,#0x0
  000b308c: bgt 0x000b317e
  000b308e: cmp r5,#0x0
  000b3090: bne 0x000b3164
  000b3092: ldr.w r0,[r4,#0xd0]
  000b3096: cmp r0,#0x0
  000b3098: beq 0x000b3164
  000b309a: ldrb.w r1,[r4,#0xec]
  000b309e: cbnz r1,0x000b30d8
  000b30a0: ldr r0,[r0,#0x28]
  000b30a2: subs r0,#0x9
  000b30a4: cmp r0,#0x2
  000b30a6: bcc 0x000b30d8
  000b30a8: ldr r0,[0x000b31b0]
  000b30aa: add r0,pc
  000b30ac: ldr r5,[r0,#0x0]
  000b30ae: ldr r0,[r5,#0x0]
  000b30b0: blx 0x00071a10
  000b30b4: cbz r0,0x000b30d8
  000b30b6: ldr.w r0,[r4,#0xd0]
  000b30ba: ldr r6,[r0,#0x28]
  000b30bc: ldr r0,[r5,#0x0]
  000b30be: blx 0x00071a10
  000b30c2: blx 0x00071950
  000b30c6: cmp r6,r0
  000b30c8: bne 0x000b30d8
  000b30ca: ldr.w r0,[r4,#0xd0]
  000b30ce: movs r2,#0x0
  000b30d0: ldr r1,[r0,#0x28]
  000b30d2: ldr r0,[r0,#0x54]
  000b30d4: blx 0x00072eec
  000b30d8: ldr.w r0,[r4,#0xd0]
  000b30dc: ldrb.w r1,[r0,#0x3c]
  000b30e0: cbz r1,0x000b30e6
  000b30e2: cbnz r0,0x000b30f4
  000b30e4: b 0x000b3164
  000b30e6: ldrb.w r1,[r0,#0x3d]
  000b30ea: cbnz r1,0x000b30f4
  000b30ec: ldrb.w r0,[r0,#0x42]
  000b30f0: cmp r0,#0x0
  000b30f2: beq 0x000b318c
  000b30f4: ldrb.w r0,[r4,#0x68]
  000b30f8: cbnz r0,0x000b3164
  000b30fa: ldr r0,[0x000b31b8]
  000b30fc: movs r2,#0x1
  000b30fe: add r0,pc
  000b3100: ldr.w r8,[r0,#0x0]
  000b3104: ldr.w r0,[r8,#0x0]
  000b3108: ldr.w r1,[r0,#0x134]
  000b310c: cmp.w r1,#0xffffffff
  000b3110: itt le
  000b3112: mov.le r1,#0x0
  000b3114: str.w.le r1,[r0,#0x134]
  000b3118: ldr r0,[0x000b31bc]
  000b311a: movs r1,#0x2a
  000b311c: add r0,pc
  000b311e: ldr r6,[r0,#0x0]
  000b3120: ldr r0,[r6,#0x0]
  000b3122: blx 0x00072ef8
  000b3126: cbnz r0,0x000b3164
  000b3128: ldr.w r0,[r8,#0x0]
  000b312c: movs r2,#0x1
  000b312e: ldr.w r1,[r0,#0x134]
  000b3132: adds r5,r1,#0x1
  000b3134: str.w r5,[r0,#0x134]
  000b3138: ldr r0,[r6,#0x0]
  000b313a: movs r1,#0x2a
  000b313c: movs r6,#0x1
  000b313e: blx 0x00072f04
  000b3142: cmp r5,r0
  000b3144: blt 0x000b3164
  000b3146: ldr.w r0,[r4,#0xd0]
  000b314a: ldr r0,[r0,#0x54]
  000b314c: blx 0x00072034
  000b3150: blx 0x00072f10
  000b3154: movs r1,#0x2a
  000b3156: movs r2,#0x64
  000b3158: blx 0x00072f1c
  000b315c: ldr.w r0,[r8,#0x0]
  000b3160: strb.w r6,[r0,#0x138]
  000b3164: vldr.32 s0,[r4,#0xe4]
  000b3168: movs r0,#0x1
  000b316a: vcvt.f32.s32 s0,s0
  000b316e: strb.w r0,[r4,#0x68]
  000b3172: movs r0,#0x0
  000b3174: str r0,[r4,#0x7c]
  000b3176: str.w r0,[r4,#0xe8]
  000b317a: vstr.32 s0,[r4,#0xd8]
  000b317e: mov r0,r4
  000b3180: pop.w {r8,r9,r11}
  000b3184: pop.w {r4,r5,r6,r7,lr}
  000b3188: b.w 0x001abcd8
  000b318c: ldr r0,[0x000b31b4]
  000b318e: add r0,pc
  000b3190: ldr r0,[r0,#0x0]
  000b3192: ldr r0,[r0,#0x0]
  000b3194: blx 0x00071b84
  000b3198: ldr.w r1,[r4,#0xd0]
  000b319c: ldr r1,[r1,#0x28]
  000b319e: blx 0x00072f28
  000b31a2: ldr.w r0,[r4,#0xd0]
  000b31a6: b 0x000b30e2
  001abcd8: bx pc
```
