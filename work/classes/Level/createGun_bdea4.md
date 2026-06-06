# Level::createGun
elf 0xbdea4 body 1768
Sig: undefined __thiscall createGun(Level * this, int param_1, int param_2, int param_3, int param_4, int param_5, int param_6, int param_7, int param_8)

## decompile
```c

/* Level::createGun(int, int, int, int, int, int, int, int) */

Gun * __thiscall
Level::createGun(Level *this,int param_1,int param_2,int param_3,int param_4,int param_5,int param_6
                ,int param_7,int param_8)

{
  int iVar1;
  int *piVar2;
  Array<AbstractGun*> *this_00;
  int iVar3;
  ObjectGun *this_01;
  Gun *this_02;
  uint in_fpscr;
  undefined4 uVar4;
  undefined4 uVar5;
  
  this_01 = (ObjectGun *)0x0;
  this_02 = (Gun *)0x0;
  switch(param_3) {
  case 0:
  case 1:
  case 3:
    iVar1 = *(int *)(DAT_000ce2b4 + 0xcdf14 + param_1 * 4);
    if (iVar1 < 0) {
      this_02 = operator_new(0x114);
      uVar4 = VectorSignedToFloat(param_8,(byte)(in_fpscr >> 0x16) & 3);
      Gun::Gun(this_02,param_2,param_5,1,param_4,param_7,param_6,uVar4,0,0,0,0,0,0);
      Gun::setIndex(this_02,param_1);
      *(int *)(this_02 + 0x5c) = param_3;
      Gun::setPlayerGun(this_02,true);
      this_01 = operator_new(0x24);
      BeamGun::BeamGun((BeamGun *)this_01,param_2,this_02,param_1,this);
    }
    else {
      uVar4 = 0x14;
      uVar5 = VectorSignedToFloat(param_8,(byte)(in_fpscr >> 0x16) & 3);
      if (param_1 - 9U < 3) {
        uVar4 = 1;
      }
      if (param_1 == 0xe4) {
        uVar4 = 1;
      }
      this_02 = operator_new(0x114);
      if (param_3 == 3) {
        Gun::Gun(this_02,param_2,param_5,uVar4,param_4,param_7,param_6,uVar5,0,0,DAT_000ce2b8,0,0,0)
        ;
        Gun::setIndex(this_02,param_1);
        *(undefined4 *)(this_02 + 0x5c) = 3;
        Gun::setPlayerGun(this_02,true);
        Gun::setErrorMagnitudePercentage(this_02,0x14);
        this_01 = operator_new(0xe8);
        RocketGun::RocketGun((RocketGun *)this_01,param_2,this_02,iVar1,0,0,0,true,this);
      }
      else {
        Gun::Gun(this_02,param_2,param_5,uVar4,param_4,param_7,param_6,uVar5,0,0,0,0,0,0);
        Gun::setIndex(this_02,param_1);
        *(int *)(this_02 + 0x5c) = param_3;
        Gun::setPlayerGun(this_02,true);
        this_01 = operator_new(0xb0);
        ObjectGun::ObjectGun(this_01,param_2,this_02,iVar1,1000,this);
      }
    }
    break;
  case 2:
  case 0x19:
    this_02 = operator_new(0x114);
    uVar4 = VectorSignedToFloat(param_8,(byte)(in_fpscr >> 0x16) & 3);
    Gun::Gun(this_02,param_2,param_5,0x19,param_4,param_7,param_6,uVar4,0,0,DAT_000ce2bc,0,0,0);
    Gun::setIndex(this_02,param_1);
    *(int *)(this_02 + 0x5c) = param_3;
    Gun::setPlayerGun(this_02,true);
    this_01 = operator_new(0xb0);
    ObjectGun::ObjectGun
              (this_01,param_2,this_02,*(int *)(DAT_000ce2d4 + 0xce186 + param_1 * 4),1000,this);
    Gun::setErrorMagnitudePercentage(this_02,2);
    break;
  case 4:
  case 5:
switchD_000cded2_caseD_4:
    this_02 = operator_new(0x114);
    uVar4 = VectorSignedToFloat(param_8,(byte)(in_fpscr >> 0x16) & 3);
    iVar1 = 5;
    if (param_3 == 0x28) {
      iVar1 = param_1 + -0xd3;
    }
    Gun::Gun(this_02,param_2,param_5,iVar1,param_4,param_7,param_6,uVar4,0,0,0,0,0,0);
    Gun::setIndex(this_02,param_1);
    *(int *)(this_02 + 0x5c) = param_3;
    Gun::setPlayerGun(this_02,true);
    this_01 = operator_new(0xe8);
    RocketGun::RocketGun
              ((RocketGun *)this_01,param_2,this_02,*(int *)(DAT_000ce2c8 + 0xce0e0 + param_1 * 4),0
               ,0,param_3,param_3 == 0x28 || param_3 == 5,this);
    Globals::addSoundResourceToList(**(int **)(DAT_000ce2cc + 0xce114));
    break;
  case 6:
  case 7:
switchD_000cded2_caseD_6:
    this_02 = operator_new(0x114);
    uVar4 = VectorSignedToFloat(param_8,(byte)(in_fpscr >> 0x16) & 3);
    Gun::Gun(this_02,param_2,param_5,1,param_4,param_7,param_6,uVar4,0,0,DAT_000ce2bc,0,0,0);
    Gun::setIndex(this_02,param_1);
    *(int *)(this_02 + 0x5c) = param_3;
    Gun::setPlayerGun(this_02,true);
    iVar1 = Item::getAttribute(*(int *)(*(int *)(**(int **)(DAT_000ce2c0 + 0xce038) + 4) +
                                       param_1 * 4));
    this_01 = operator_new(300);
    BombGun::BombGun((BombGun *)this_01,this_02,*(uint *)(DAT_000ce2c4 + 0xce056 + param_1 * 4),1,
                     param_3,iVar1 == 1,this);
LAB_000ce3ce:
    piVar2 = *(int **)(DAT_000ce64c + 0xce3d6);
    goto LAB_000ce3dc;
  case 8:
    iVar1 = DAT_000ce644;
    if ((param_1 == 0xb5) || (iVar1 = DAT_000ce2b8, param_1 != 0x30)) {
      iVar3 = 0;
    }
    else {
      iVar3 = DAT_000ce2b8 + -0xf60000;
    }
    this_02 = operator_new(0x114);
    uVar4 = VectorSignedToFloat(param_8,(byte)(in_fpscr >> 0x16) & 3);
    Gun::Gun(this_02,param_2,param_5,0xf,param_4,param_7,param_6,uVar4,iVar3,0,iVar1,0,0,0);
    Gun::setIndex(this_02,param_1);
    *(undefined4 *)(this_02 + 0x5c) = 8;
    Gun::setPlayerGun(this_02,true);
    if (((param_1 == 0x30 || param_1 == 0xe0) || (param_1 == 0xb5)) &&
       (this_02[0xa4] = (Gun)0x1, param_1 == 0xe0)) {
      this_02[0xa5] = (Gun)0x1;
    }
    this_01 = operator_new(0xb0);
    ObjectGun::ObjectGun
              (this_01,param_2,this_02,*(int *)(DAT_000ce648 + 0xce562 + param_1 * 4),1000,this);
    break;
  case 9:
  case 10:
  case 0xc:
  case 0xd:
  case 0xe:
  case 0xf:
  case 0x10:
  case 0x11:
  case 0x12:
  case 0x13:
  case 0x14:
  case 0x15:
  case 0x16:
  case 0x17:
  case 0x18:
    break;
  case 0xb:
    this_02 = operator_new(0x114);
    Gun::Gun(this_02,param_2,param_5,10,param_4,param_7,param_6,0x40000000,0,0,0,0,0,0);
    Gun::setIndex(this_02,param_1);
    *(undefined4 *)(this_02 + 0x5c) = 0xb;
    Gun::setPlayerGun(this_02,true);
    this_01 = operator_new(0xd4);
    MineGun::MineGun((MineGun *)this_01,this_02,*(int *)(DAT_000ce2d8 + 0xce284 + param_1 * 4),1,0xb
                     ,this);
LAB_000ce344:
    piVar2 = *(int **)(DAT_000ce654 + 0xce34c);
LAB_000ce3dc:
    Globals::addSoundResourceToList(*piVar2);
    break;
  default:
    this_02 = (Gun *)0x0;
    switch(param_3) {
    case 0x22:
      goto switchD_000cded2_caseD_6;
    case 0x23:
      iVar1 = DAT_000ce644;
      if ((param_1 == 0xb5) || (iVar1 = DAT_000ce2b8, param_1 != 0x30)) {
        iVar3 = 0;
      }
      else {
        iVar3 = DAT_000ce2b8 + -0xf60000;
      }
      this_02 = operator_new(0x114);
      uVar4 = VectorSignedToFloat(param_8,(byte)(in_fpscr >> 0x16) & 3);
      Gun::Gun(this_02,param_2,param_5,0xf,param_4,param_7,param_6,uVar4,iVar3,0,iVar1,0,0,0);
      Gun::setIndex(this_02,param_1);
      *(undefined4 *)(this_02 + 0x5c) = 0x23;
      Gun::setPlayerGun(this_02,true);
      this_01 = operator_new(0xb0);
      ObjectGun::ObjectGun
                (this_01,param_2,this_02,*(int *)(DAT_000ce660 + 0xce5e4 + param_1 * 4),1000,this);
      break;
    case 0x24:
    case 0x25:
    case 0x26:
    case 0x29:
      break;
    case 0x27:
      this_02 = operator_new(0x114);
      Gun::Gun(this_02,param_2,0,3,param_4,param_7,param_6,0x40000000,0,0,0,0,0,0);
      Gun::setIndex(this_02,param_1);
      *(undefined4 *)(this_02 + 0x5c) = 0x27;
      Gun::setPlayerGun(this_02,true);
      this_01 = operator_new(0xb4);
      SentryGun::SentryGun
                ((SentryGun *)this_01,this_02,*(int *)(DAT_000ce65c + 0xce334 + param_1 * 4),1,0x27,
                 this);
      goto LAB_000ce344;
    case 0x28:
      goto switchD_000cded2_caseD_4;
    case 0x2a:
      this_02 = operator_new(0x114);
      uVar4 = VectorSignedToFloat(param_8,(byte)(in_fpscr >> 0x16) & 3);
      Gun::Gun(this_02,param_2,param_5,1,param_4,1,param_6,uVar4,0,0,0,0,0,0);
      Gun::setIndex(this_02,param_1);
      *(undefined4 *)(this_02 + 0x5c) = 0x2a;
      Gun::setPlayerGun(this_02,true);
      this_01 = operator_new(300);
      BombGun::BombGun((BombGun *)this_01,this_02,*(uint *)(DAT_000ce664 + 0xce3ba + param_1 * 4),1,
                       0x2a,false,this);
      goto LAB_000ce3ce;
    default:
      this_02 = (Gun *)0x0;
    }
  }
  switch(param_1) {
  case 0x29:
    Globals::addSoundResourceToList(**(int **)(DAT_000ce668 + 0xce3fc));
  case 0x2a:
    Globals::addSoundResourceToList(**(int **)(DAT_000ce66c + 0xce40a));
  case 0x2b:
    Globals::addSoundResourceToList(**(int **)(DAT_000ce670 + 0xce418));
  case 0x2c:
    Globals::addSoundResourceToList(**(int **)(DAT_000ce674 + 0xce426));
  case 0x2d:
    Globals::addSoundResourceToList(**(int **)(DAT_000ce678 + 0xce434));
  case 0x2e:
    Globals::addSoundResourceToList(**(int **)(DAT_000ce67c + 0xce442));
  default:
    Gun::setLevel(this_02,this);
    this_00 = *(Array<AbstractGun*> **)(this + 0xe4);
    if (this_00 == (Array<AbstractGun*> *)0x0) {
      this_00 = operator_new(0xc);
      Array<AbstractGun*>::Array(this_00);
      *(Array<AbstractGun*> **)(this + 0xe4) = this_00;
    }
    ArrayAdd<AbstractGun*>((AbstractGun *)this_01,(Array *)this_00);
    return this_02;
  }
}

```
## target disasm
```
  000cdea4: push {r4,r5,r6,r7,lr}
  000cdea6: add r7,sp,#0xc
  000cdea8: push {r7,r8,r9,r10,r11}
  000cdeac: vpush {d1,d2,d3,d4,d5,d6,d7,d8}
  000cdeb0: add.w r12,r7,#0x10
  000cdeb4: str r1,[sp,#0x34]
  000cdeb6: mov r9,r2
  000cdeb8: mov r5,r3
  000cdeba: ldm.w r12,{r2,r6,r12}
  000cdebe: mov r8,r0
  000cdec0: mov.w r10,#0x0
  000cdec4: ldrd r4,r1,[r7,#0x8]
  000cdec8: cmp r3,#0x19
  000cdeca: bhi.w 0x000cdfba
  000cdece: mov.w r11,#0x0
  000cded2: tbh [pc,r5]
  000cdf0a: ldr r0,[0x000ce2b4]
  000cdf0c: mov r10,r4
  000cdf0e: ldr r6,[sp,#0x34]
  000cdf10: add r0,pc
  000cdf12: ldr.w r0,[r0,r6,lsl #0x2]
  000cdf16: cmp r0,#0x0
  000cdf18: blt.w 0x000ce1a4
  000cdf1c: vmov s0,r12
  000cdf20: strd r0,r8,[sp,#0x2c]
  000cdf24: sub.w r0,r6,#0x9
  000cdf28: movs r4,#0x14
  000cdf2a: cmp r0,#0x3
  000cdf2c: vcvt.f32.s32 s16,s0
  000cdf30: it cc
  000cdf32: mov.cc r4,#0x1
  000cdf34: mov.w r0,#0x114
  000cdf38: cmp r6,#0xe4
  000cdf3a: it eq
  000cdf3c: mov.eq r4,#0x1
  000cdf3e: blx 0x0006eb24
  000cdf42: mov r11,r0
  000cdf44: cmp r5,#0x3
  000cdf46: bne.w 0x000ce47c
  000cdf4a: ldr r0,[0x000ce2b8]
  000cdf4c: movs r5,#0x0
  000cdf4e: ldr r2,[r7,#0xc]
  000cdf50: strd r5,r5,[sp,#0x10]
  000cdf54: mov r1,r9
  000cdf56: strd r0,r5,[sp,#0x18]
  000cdf5a: mov r3,r4
  000cdf5c: ldr r0,[r7,#0x14]
  000cdf5e: strd r5,r5,[sp,#0x20]
  000cdf62: str r0,[sp,#0x4]
  000cdf64: ldr r0,[r7,#0x10]
  000cdf66: str r0,[sp,#0x8]
  000cdf68: mov r0,r11
  000cdf6a: vstr.32 s16,[sp,#0xc]
  000cdf6e: str.w r10,[sp,#0x0]
  000cdf72: blx 0x00074218
  000cdf76: mov r0,r11
  000cdf78: mov r1,r6
  000cdf7a: blx 0x0007423c
  000cdf7e: movs r0,#0x3
  000cdf80: movs r1,#0x1
  000cdf82: str.w r0,[r11,#0x5c]
  000cdf86: mov r0,r11
  000cdf88: movs r4,#0x1
  000cdf8a: blx 0x000742cc
  000cdf8e: mov r0,r11
  000cdf90: movs r1,#0x14
  000cdf92: blx 0x000742d8
  000cdf96: movs r0,#0xe8
  000cdf98: blx 0x0006eb24
  000cdf9c: mov r10,r0
  000cdf9e: strd r5,r5,[sp,#0x0]
  000cdfa2: mov r1,r9
  000cdfa4: strd r5,r4,[sp,#0x8]
  000cdfa8: mov r2,r11
  000cdfaa: ldr r3,[sp,#0x2c]
  000cdfac: ldr.w r8,[sp,#0x30]
  000cdfb0: str.w r8,[sp,#0x10]
  000cdfb4: blx 0x00074260
  000cdfb8: b 0x000ce3e2
  000cdfba: sub.w r0,r5,#0x22
  000cdfbe: cmp r0,#0x8
  000cdfc0: bhi.w 0x000ce4de
  000cdfc4: mov.w r11,#0x0
  000cdfc8: tbh [pc,r0]
  000cdfde: mov.w r0,#0x114
  000cdfe2: mov r10,r12
  000cdfe4: blx 0x0006eb24
  000cdfe8: mov r11,r0
  000cdfea: ldr r0,[0x000ce2bc]
  000cdfec: vmov s0,r10
  000cdff0: movs r1,#0x0
  000cdff2: vcvt.f32.s32 s0,s0
  000cdff6: ldr r2,[r7,#0xc]
  000cdff8: movs r3,#0x1
  000cdffa: strd r1,r1,[sp,#0x10]
  000cdffe: strd r0,r1,[sp,#0x18]
  000ce002: ldr r0,[r7,#0x10]
  000ce004: strd r1,r1,[sp,#0x20]
  000ce008: mov r1,r9
  000ce00a: strd r4,r6,[sp,#0x0]
  000ce00e: str r0,[sp,#0x8]
  000ce010: mov r0,r11
  000ce012: vstr.32 s0,[sp,#0xc]
  000ce016: blx 0x00074218
  000ce01a: ldr r6,[sp,#0x34]
  000ce01c: mov r0,r11
  000ce01e: mov r1,r6
  000ce020: blx 0x0007423c
  000ce024: mov r0,r11
  000ce026: movs r1,#0x1
  000ce028: str.w r5,[r11,#0x5c]
  000ce02c: blx 0x000742cc
  000ce030: ldr r0,[0x000ce2c0]
  000ce032: movs r1,#0xf
  000ce034: add r0,pc
  000ce036: ldr r0,[r0,#0x0]
  000ce038: ldr r0,[r0,#0x0]
  000ce03a: ldr r0,[r0,#0x4]
  000ce03c: ldr.w r0,[r0,r6,lsl #0x2]
  000ce040: blx 0x000718e4
  000ce044: mov r4,r0
  000ce046: mov.w r0,#0x12c
  000ce04a: blx 0x0006eb24
  000ce04e: mov r10,r0
  000ce050: ldr r0,[0x000ce2c4]
  000ce052: add r0,pc
  000ce054: ldr.w r2,[r0,r6,lsl #0x2]
  000ce058: subs r0,r4,#0x1
  000ce05a: mov r1,r11
  000ce05c: clz r0,r0
  000ce060: movs r3,#0x1
  000ce062: lsrs r0,r0,#0x5
  000ce064: strd r5,r0,[sp,#0x0]
  000ce068: mov r0,r10
  000ce06a: str.w r8,[sp,#0x8]
  000ce06e: blx 0x000742e4
  000ce072: b 0x000ce3ce
  000ce074: mov.w r0,#0x114
  000ce078: str.w r8,[sp,#0x30]
  000ce07c: mov r8,r4
  000ce07e: mov r10,r12
  000ce080: blx 0x0006eb24
  000ce084: vmov s0,r10
  000ce088: mov r11,r0
  000ce08a: movs r4,#0x0
  000ce08c: vcvt.f32.s32 s0,s0
  000ce090: strd r4,r4,[sp,#0x10]
  000ce094: movs r3,#0x5
  000ce096: strd r4,r4,[sp,#0x18]
  000ce09a: cmp r5,#0x28
  000ce09c: strd r4,r4,[sp,#0x20]
  000ce0a0: mov r1,r9
  000ce0a2: strd r8,r6,[sp,#0x0]
  000ce0a6: ldr r6,[sp,#0x34]
  000ce0a8: ldr r2,[r7,#0xc]
  000ce0aa: ldr r0,[r7,#0x10]
  000ce0ac: str r0,[sp,#0x8]
  000ce0ae: mov r0,r11
  000ce0b0: vstr.32 s0,[sp,#0xc]
  000ce0b4: it eq
  000ce0b6: sub.eq.w r3,r6,#0xd3
  000ce0ba: blx 0x00074218
  000ce0be: mov r0,r11
  000ce0c0: mov r1,r6
  000ce0c2: blx 0x0007423c
  000ce0c6: mov r0,r11
  000ce0c8: movs r1,#0x1
  000ce0ca: str.w r5,[r11,#0x5c]
  000ce0ce: blx 0x000742cc
  000ce0d2: movs r0,#0xe8
  000ce0d4: blx 0x0006eb24
  000ce0d8: mov r10,r0
  000ce0da: ldr r0,[0x000ce2c8]
  000ce0dc: add r0,pc
  000ce0de: ldr.w r3,[r0,r6,lsl #0x2]
  000ce0e2: sub.w r0,r5,#0x28
  000ce0e6: subs r1,r5,#0x5
  000ce0e8: clz r0,r0
  000ce0ec: clz r1,r1
  000ce0f0: strd r4,r4,[sp,#0x0]
  000ce0f4: mov r2,r11
  000ce0f6: lsrs r0,r0,#0x5
  000ce0f8: lsrs r1,r1,#0x5
  000ce0fa: orrs r0,r1
  000ce0fc: mov r1,r9
  000ce0fe: strd r5,r0,[sp,#0x8]
  000ce102: ldr r0,[sp,#0x30]
  000ce104: str r0,[sp,#0x10]
  000ce106: mov r0,r10
  000ce108: blx 0x00074260
  000ce10c: ldr r0,[0x000ce2cc]
  000ce10e: ldr r1,[0x000ce2d0]
  000ce110: add r0,pc
  000ce112: add r1,pc
  000ce114: ldr r0,[r0,#0x0]
  000ce116: ldr.w r1,[r1,r6,lsl #0x2]
  000ce11a: ldr r0,[r0,#0x0]
  000ce11c: blx 0x00072298
  000ce120: ldr.w r8,[sp,#0x30]
  000ce124: b 0x000ce3e2
  000ce126: mov.w r0,#0x114
  000ce12a: mov r10,r12
  000ce12c: blx 0x0006eb24
  000ce130: mov r11,r0
  000ce132: ldr r0,[0x000ce2bc]
  000ce134: vmov s0,r10
  000ce138: movs r1,#0x0
  000ce13a: vcvt.f32.s32 s0,s0
  000ce13e: ldr r2,[r7,#0xc]
  000ce140: movs r3,#0x19
  000ce142: strd r1,r1,[sp,#0x10]
  000ce146: strd r0,r1,[sp,#0x18]
  000ce14a: ldr r0,[r7,#0x10]
  000ce14c: strd r1,r1,[sp,#0x20]
  000ce150: mov r1,r9
  000ce152: strd r4,r6,[sp,#0x0]
  000ce156: str r0,[sp,#0x8]
  000ce158: mov r0,r11
  000ce15a: vstr.32 s0,[sp,#0xc]
  000ce15e: blx 0x00074218
  000ce162: ldr r4,[sp,#0x34]
  000ce164: mov r0,r11
  000ce166: mov r1,r4
  000ce168: blx 0x0007423c
  000ce16c: mov r0,r11
  000ce16e: movs r1,#0x1
  000ce170: str.w r5,[r11,#0x5c]
  000ce174: blx 0x000742cc
  000ce178: movs r0,#0xb0
  000ce17a: blx 0x0006eb24
  000ce17e: mov r10,r0
  000ce180: ldr r0,[0x000ce2d4]
  000ce182: add r0,pc
  000ce184: ldr.w r3,[r0,r4,lsl #0x2]
  000ce188: mov.w r0,#0x3e8
  000ce18c: mov r1,r9
  000ce18e: strd r0,r8,[sp,#0x0]
  000ce192: mov r0,r10
  000ce194: mov r2,r11
  000ce196: blx 0x0007426c
  000ce19a: mov r0,r11
  000ce19c: movs r1,#0x2
  000ce19e: blx 0x000742d8
  000ce1a2: b 0x000ce3e2
  000ce1a4: mov.w r0,#0x114
  000ce1a8: mov r4,r12
  000ce1aa: blx 0x0006eb24
  000ce1ae: vmov s0,r4
  000ce1b2: mov r11,r0
  000ce1b4: movs r0,#0x0
  000ce1b6: vcvt.f32.s32 s0,s0
  000ce1ba: strd r0,r0,[sp,#0x10]
  000ce1be: mov r1,r9
  000ce1c0: strd r0,r0,[sp,#0x18]
  000ce1c4: movs r3,#0x1
  000ce1c6: strd r0,r0,[sp,#0x20]
  000ce1ca: ldr r0,[r7,#0x14]
  000ce1cc: ldr r2,[r7,#0xc]
  000ce1ce: str r0,[sp,#0x4]
  000ce1d0: ldr r0,[r7,#0x10]
  000ce1d2: str r0,[sp,#0x8]
  000ce1d4: mov r0,r11
  000ce1d6: str.w r10,[sp,#0x0]
  000ce1da: vstr.32 s0,[sp,#0xc]
  000ce1de: blx 0x00074218
  000ce1e2: mov r0,r11
  000ce1e4: mov r1,r6
  000ce1e6: blx 0x0007423c
  000ce1ea: mov r0,r11
  000ce1ec: movs r1,#0x1
  000ce1ee: str.w r5,[r11,#0x5c]
  000ce1f2: blx 0x000742cc
  000ce1f6: movs r0,#0x24
  000ce1f8: blx 0x0006eb24
  000ce1fc: mov r10,r0
  000ce1fe: mov r1,r9
  000ce200: mov r2,r11
  000ce202: mov r3,r6
  000ce204: str.w r8,[sp,#0x0]
  000ce208: blx 0x000742f0
  000ce20c: b 0x000ce3e2
  000ce20e: ldr r0,[sp,#0x34]
  000ce210: str.w r9,[sp,#0x2c]
  000ce214: cmp r0,#0xb5
  000ce216: beq.w 0x000ce4e4
  000ce21a: ldr r5,[0x000ce2b8]
  000ce21c: mov r10,r12
  000ce21e: cmp r0,#0x30
  000ce220: bne.w 0x000ce4e8
  000ce224: sub.w r9,r5,#0xf60000
  000ce228: b 0x000ce4ec
  000ce22a: mov.w r0,#0x114
  000ce22e: mov r10,r1
  000ce230: mov r5,r2
  000ce232: blx 0x0006eb24
  000ce236: mov r11,r0
  000ce238: movs r0,#0x0
  000ce23a: mov.w r1,#0x40000000
  000ce23e: strd r0,r0,[sp,#0x20]
  000ce242: mov r2,r10
  000ce244: strd r4,r6,[sp,#0x0]
  000ce248: movs r3,#0xa
  000ce24a: strd r5,r1,[sp,#0x8]
  000ce24e: mov r1,r9
  000ce250: strd r0,r0,[sp,#0x10]
  000ce254: strd r0,r0,[sp,#0x18]
  000ce258: mov r0,r11
  000ce25a: blx 0x00074218
  000ce25e: ldr r5,[sp,#0x34]
  000ce260: mov r0,r11
  000ce262: mov r1,r5
  000ce264: blx 0x0007423c
  000ce268: movs r4,#0xb
  000ce26a: mov r0,r11
  000ce26c: movs r1,#0x1
  000ce26e: str.w r4,[r11,#0x5c]
  000ce272: blx 0x000742cc
  000ce276: movs r0,#0xd4
  000ce278: blx 0x0006eb24
  000ce27c: mov r10,r0
  000ce27e: ldr r0,[0x000ce2d8]
  000ce280: add r0,pc
  000ce282: ldr.w r2,[r0,r5,lsl #0x2]
  000ce286: mov r0,r10
  000ce288: mov r1,r11
  000ce28a: movs r3,#0x1
  000ce28c: strd r4,r8,[sp,#0x0]
  000ce290: blx 0x000742fc
  000ce294: b 0x000ce344
  000ce296: strd r9,r8,[sp,#0x2c]
  000ce29a: ldr r0,[sp,#0x34]
  000ce29c: cmp r0,#0xb5
  000ce29e: beq.w 0x000ce57e
  000ce2a2: ldr r5,[0x000ce2b8]
  000ce2a4: mov r10,r12
  000ce2a6: mov r9,r2
  000ce2a8: cmp r0,#0x30
  000ce2aa: bne.w 0x000ce584
  000ce2ae: sub.w r8,r5,#0xf60000
  000ce2b2: b 0x000ce588
  000ce2dc: mov.w r0,#0x114
  000ce2e0: mov r5,r2
  000ce2e2: blx 0x0006eb24
  000ce2e6: mov r11,r0
  000ce2e8: movs r0,#0x0
  000ce2ea: mov.w r1,#0x40000000
  000ce2ee: strd r0,r0,[sp,#0x20]
  000ce2f2: movs r2,#0x0
  000ce2f4: strd r4,r6,[sp,#0x0]
  000ce2f8: movs r3,#0x3
  000ce2fa: strd r5,r1,[sp,#0x8]
  000ce2fe: mov r1,r9
  000ce300: strd r0,r0,[sp,#0x10]
  000ce304: strd r0,r0,[sp,#0x18]
  000ce308: mov r0,r11
  000ce30a: blx 0x00074218
  000ce30e: ldr r5,[sp,#0x34]
  000ce310: mov r0,r11
  000ce312: mov r1,r5
  000ce314: blx 0x0007423c
  000ce318: movs r4,#0x27
  000ce31a: mov r0,r11
  000ce31c: movs r1,#0x1
  000ce31e: str.w r4,[r11,#0x5c]
  000ce322: blx 0x000742cc
  000ce326: movs r0,#0xb4
  000ce328: blx 0x0006eb24
  000ce32c: mov r10,r0
  000ce32e: ldr r0,[0x000ce65c]
  000ce330: add r0,pc
  000ce332: ldr.w r2,[r0,r5,lsl #0x2]
  000ce336: mov r0,r10
  000ce338: mov r1,r11
  000ce33a: movs r3,#0x1
  000ce33c: strd r4,r8,[sp,#0x0]
  000ce340: blx 0x00074308
  000ce344: ldr r0,[0x000ce654]
  000ce346: ldr r1,[0x000ce658]
  000ce348: add r0,pc
  000ce34a: add r1,pc
  000ce34c: ldr r0,[r0,#0x0]
  000ce34e: ldr.w r1,[r1,r5,lsl #0x2]
  000ce352: b 0x000ce3dc
  000ce354: mov.w r0,#0x114
  000ce358: mov r5,r2
  000ce35a: mov r10,r4
  000ce35c: mov r6,r12
  000ce35e: blx 0x0006eb24
  000ce362: vmov s0,r6
  000ce366: mov r11,r0
  000ce368: movs r4,#0x0
  000ce36a: vcvt.f32.s32 s0,s0
  000ce36e: ldr r2,[r7,#0xc]
  000ce370: movs r0,#0x1
  000ce372: strd r4,r4,[sp,#0x10]
  000ce376: mov r1,r9
  000ce378: strd r4,r4,[sp,#0x18]
  000ce37c: movs r3,#0x1
  000ce37e: strd r4,r4,[sp,#0x20]
  000ce382: strd r10,r0,[sp,#0x0]
  000ce386: mov r0,r11
  000ce388: str r5,[sp,#0x8]
  000ce38a: vstr.32 s0,[sp,#0xc]
  000ce38e: blx 0x00074218
  000ce392: ldr r6,[sp,#0x34]
  000ce394: mov r0,r11
  000ce396: mov r1,r6
  000ce398: blx 0x0007423c
  000ce39c: movs r5,#0x2a
  000ce39e: mov r0,r11
  000ce3a0: movs r1,#0x1
  000ce3a2: str.w r5,[r11,#0x5c]
  000ce3a6: blx 0x000742cc
  000ce3aa: mov.w r0,#0x12c
  000ce3ae: blx 0x0006eb24
  000ce3b2: mov r10,r0
  000ce3b4: ldr r0,[0x000ce664]
  000ce3b6: add r0,pc
  000ce3b8: ldr.w r2,[r0,r6,lsl #0x2]
  000ce3bc: mov r0,r10
  000ce3be: mov r1,r11
  000ce3c0: movs r3,#0x1
  000ce3c2: strd r5,r4,[sp,#0x0]
  000ce3c6: str.w r8,[sp,#0x8]
  000ce3ca: blx 0x000742e4
  000ce3ce: ldr r0,[0x000ce64c]
  000ce3d0: ldr r1,[0x000ce650]
  000ce3d2: add r0,pc
  000ce3d4: add r1,pc
  000ce3d6: ldr r0,[r0,#0x0]
  000ce3d8: ldr.w r1,[r1,r6,lsl #0x2]
  000ce3dc: ldr r0,[r0,#0x0]
  000ce3de: blx 0x00072298
  000ce3e2: ldr r0,[sp,#0x34]
  000ce3e4: subs r0,#0x29
  000ce3e6: cmp r0,#0x5
  000ce3e8: bhi 0x000ce448
  000ce3ea: tbb [pc,r0]
  000ce3f4: ldr r0,[0x000ce668]
  000ce3f6: movs r1,#0xf
  000ce3f8: add r0,pc
  000ce3fa: ldr r0,[r0,#0x0]
  000ce3fc: ldr r0,[r0,#0x0]
  000ce3fe: blx 0x00072298
  000ce402: ldr r0,[0x000ce66c]
  000ce404: movs r1,#0x10
  000ce406: add r0,pc
  000ce408: ldr r0,[r0,#0x0]
  000ce40a: ldr r0,[r0,#0x0]
  000ce40c: blx 0x00072298
  000ce410: ldr r0,[0x000ce670]
  000ce412: movs r1,#0x11
  000ce414: add r0,pc
  000ce416: ldr r0,[r0,#0x0]
  000ce418: ldr r0,[r0,#0x0]
  000ce41a: blx 0x00072298
  000ce41e: ldr r0,[0x000ce674]
  000ce420: movs r1,#0xe
  000ce422: add r0,pc
  000ce424: ldr r0,[r0,#0x0]
  000ce426: ldr r0,[r0,#0x0]
  000ce428: blx 0x00072298
  000ce42c: ldr r0,[0x000ce678]
  000ce42e: movs r1,#0xd
  000ce430: add r0,pc
  000ce432: ldr r0,[r0,#0x0]
  000ce434: ldr r0,[r0,#0x0]
  000ce436: blx 0x00072298
  000ce43a: ldr r0,[0x000ce67c]
  000ce43c: movs r1,#0xc
  000ce43e: add r0,pc
  000ce440: ldr r0,[r0,#0x0]
  000ce442: ldr r0,[r0,#0x0]
  000ce444: blx 0x00072298
  000ce448: mov r0,r11
  000ce44a: mov r1,r8
  000ce44c: blx 0x00074230
  000ce450: ldr.w r5,[r8,#0xe4]
  000ce454: cbnz r5,0x000ce466
  000ce456: movs r0,#0xc
  000ce458: blx 0x0006eb24
  000ce45c: mov r5,r0
  000ce45e: blx 0x000741f4
  000ce462: str.w r5,[r8,#0xe4]
  000ce466: mov r0,r10
  000ce468: mov r1,r5
  000ce46a: blx 0x00074284
  000ce46e: mov r0,r11
  000ce470: vpop {d1,d2,d3,d4,d5,d6,d7,d8}
  000ce474: add sp,#0x4
  000ce476: pop.w {r8,r9,r10,r11}
  000ce47a: pop {r4,r5,r6,r7,pc}
  000ce47c: movs r0,#0x0
  000ce47e: ldr r2,[r7,#0xc]
  000ce480: strd r0,r0,[sp,#0x10]
  000ce484: mov r1,r9
  000ce486: strd r0,r0,[sp,#0x18]
  000ce48a: mov r3,r4
  000ce48c: strd r0,r0,[sp,#0x20]
  000ce490: ldr r0,[r7,#0x14]
  000ce492: str r0,[sp,#0x4]
  000ce494: ldr r0,[r7,#0x10]
  000ce496: str r0,[sp,#0x8]
  000ce498: mov r0,r11
  000ce49a: vstr.32 s16,[sp,#0xc]
  000ce49e: str.w r10,[sp,#0x0]
  000ce4a2: blx 0x00074218
  000ce4a6: mov r0,r11
  000ce4a8: mov r1,r6
  000ce4aa: blx 0x0007423c
  000ce4ae: mov r0,r11
  000ce4b0: movs r1,#0x1
  000ce4b2: str.w r5,[r11,#0x5c]
  000ce4b6: blx 0x000742cc
  000ce4ba: movs r0,#0xb0
  000ce4bc: blx 0x0006eb24
  000ce4c0: mov r10,r0
  000ce4c2: ldr r3,[sp,#0x2c]
  000ce4c4: mov.w r0,#0x3e8
  000ce4c8: str r0,[sp,#0x0]
  000ce4ca: mov r0,r10
  000ce4cc: ldr.w r8,[sp,#0x30]
  000ce4d0: mov r1,r9
  000ce4d2: mov r2,r11
  000ce4d4: str.w r8,[sp,#0x4]
  000ce4d8: blx 0x0007426c
  000ce4dc: b 0x000ce3e2
  000ce4de: mov.w r11,#0x0
  000ce4e2: b 0x000ce3e2
  000ce4e4: ldr r5,[0x000ce644]
  000ce4e6: mov r10,r12
  000ce4e8: mov.w r9,#0x0
  000ce4ec: mov.w r0,#0x114
  000ce4f0: blx 0x0006eb24
  000ce4f4: vmov s0,r10
  000ce4f8: mov r11,r0
  000ce4fa: movs r0,#0x0
  000ce4fc: vcvt.f32.s32 s0,s0
  000ce500: strd r9,r0,[sp,#0x10]
  000ce504: movs r3,#0xf
  000ce506: strd r5,r0,[sp,#0x18]
  000ce50a: strd r0,r0,[sp,#0x20]
  000ce50e: strd r4,r6,[sp,#0x0]
  000ce512: ldr r1,[sp,#0x2c]
  000ce514: ldr r2,[r7,#0xc]
  000ce516: ldr r0,[r7,#0x10]
  000ce518: str r0,[sp,#0x8]
  000ce51a: mov r0,r11
  000ce51c: vstr.32 s0,[sp,#0xc]
  000ce520: blx 0x00074218
  000ce524: ldr r5,[sp,#0x34]
  000ce526: mov r0,r11
  000ce528: mov r1,r5
  000ce52a: blx 0x0007423c
  000ce52e: movs r0,#0x8
  000ce530: movs r1,#0x1
  000ce532: str.w r0,[r11,#0x5c]
  000ce536: mov r0,r11
  000ce538: movs r4,#0x1
  000ce53a: blx 0x000742cc
  000ce53e: cmp r5,#0x30
  000ce540: it ne
  000ce542: cmp.ne r5,#0xe0
  000ce544: bne 0x000ce578
  000ce546: strb.w r4,[r11,#0xa4]
  000ce54a: cmp r5,#0xe0
  000ce54c: itt eq
  000ce54e: mov.eq r0,#0x1
  000ce550: strb.eq.w r0,[r11,#0xa5]
  000ce554: movs r0,#0xb0
  000ce556: blx 0x0006eb24
  000ce55a: mov r10,r0
  000ce55c: ldr r0,[0x000ce648]
  000ce55e: add r0,pc
  000ce560: ldr.w r3,[r0,r5,lsl #0x2]
  000ce564: mov.w r0,#0x3e8
  000ce568: strd r0,r8,[sp,#0x0]
  000ce56c: ldr r1,[sp,#0x2c]
  000ce56e: mov r0,r10
  000ce570: mov r2,r11
  000ce572: blx 0x0007426c
  000ce576: b 0x000ce3e2
  000ce578: cmp r5,#0xb5
  000ce57a: beq 0x000ce546
  000ce57c: b 0x000ce554
  000ce57e: ldr r5,[0x000ce644]
  000ce580: mov r10,r12
  000ce582: mov r9,r2
  000ce584: mov.w r8,#0x0
  000ce588: mov.w r0,#0x114
  000ce58c: blx 0x0006eb24
  000ce590: vmov s0,r10
  000ce594: mov r11,r0
  000ce596: movs r0,#0x0
  000ce598: vcvt.f32.s32 s0,s0
  000ce59c: strd r8,r0,[sp,#0x10]
  000ce5a0: movs r3,#0xf
  000ce5a2: strd r5,r0,[sp,#0x18]
  000ce5a6: strd r0,r0,[sp,#0x20]
  000ce5aa: mov r0,r11
  000ce5ac: stm sp,{r4,r6,r9}
  000ce5b0: ldr r4,[sp,#0x2c]
  000ce5b2: ldr r2,[r7,#0xc]
  000ce5b4: vstr.32 s0,[sp,#0xc]
  000ce5b8: mov r1,r4
  000ce5ba: blx 0x00074218
  000ce5be: ldr r5,[sp,#0x34]
  000ce5c0: mov r0,r11
  000ce5c2: mov r1,r5
  000ce5c4: blx 0x0007423c
  000ce5c8: movs r0,#0x23
  000ce5ca: movs r1,#0x1
  000ce5cc: str.w r0,[r11,#0x5c]
  000ce5d0: mov r0,r11
  000ce5d2: blx 0x000742cc
  000ce5d6: movs r0,#0xb0
  000ce5d8: blx 0x0006eb24
  000ce5dc: mov r10,r0
  000ce5de: ldr r0,[0x000ce660]
  000ce5e0: add r0,pc
  000ce5e2: ldr.w r3,[r0,r5,lsl #0x2]
  000ce5e6: mov.w r0,#0x3e8
  000ce5ea: ldr.w r8,[sp,#0x30]
  000ce5ee: str r0,[sp,#0x0]
  000ce5f0: mov r0,r10
  000ce5f2: mov r1,r4
  000ce5f4: mov r2,r11
  000ce5f6: str.w r8,[sp,#0x4]
  000ce5fa: blx 0x0007426c
  000ce5fe: b 0x000ce3e2
```
