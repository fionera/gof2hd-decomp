# PlayerFighter::setWingmanCommand
elf 0xdcc2c body 160
Sig: undefined __thiscall setWingmanCommand(PlayerFighter * this, int param_1, KIPlayer * param_2)

## decompile
```c

/* PlayerFighter::setWingmanCommand(int, KIPlayer*) */

void __thiscall PlayerFighter::setWingmanCommand(PlayerFighter *this,int param_1,KIPlayer *param_2)

{
  int iVar1;
  undefined4 uVar2;
  
  uVar2 = *(undefined4 *)(this + 0xe4);
  KIPlayer::setWingmanCommand((KIPlayer *)this,param_1,param_2);
  if (param_1 - 2U < 2) {
    *(undefined4 *)(this + 0x1b8) = 0x1389;
    if (*(float *)(this + 0x1e8) != 5.5) {
      *(undefined4 *)(this + 0x1c0) = 0x1389;
    }
    if (param_1 == 2) {
      iVar1 = Level::getPlayerRoute();
      if (iVar1 != 0) {
        Level::getPlayerRoute();
        uVar2 = Route::getExactClone();
        *(undefined4 *)(this + 0x14c) = uVar2;
        uVar2 = Route::getCurrent();
        *(undefined4 *)(this + 0x1e4) = uVar2;
        goto LAB_000eccb6;
      }
    }
    else if (param_2 != (KIPlayer *)0x0) goto LAB_000eccb6;
    *(undefined4 *)(this + 0xe4) = uVar2;
  }
  else if (param_1 == 0) {
    *(undefined4 *)(this + 0xe4) = uVar2;
    *(uint *)(this + 0x140) = (uint)(*(int *)(this + 0x140) == 0);
  }
  else if (param_1 == 1) {
    this[0x13d] = (PlayerFighter)0x0;
    this[0x1e0] = (PlayerFighter)0x0;
  }
LAB_000eccb6:
  *(undefined4 *)(this + 0x1e8) = *(undefined4 *)(this + 0x1ac);
  *(undefined4 *)(this + 0x1f0) = *(undefined4 *)(this + 0x1a4);
  return;
}

```

## target disasm
```
  000ecc2c: push {r4,r5,r6,r7,lr}
  000ecc2e: add r7,sp,#0xc
  000ecc30: push.w r8
  000ecc34: mov r6,r2
  000ecc36: mov r5,r1
  000ecc38: mov r4,r0
  000ecc3a: ldr.w r8,[r0,#0xe4]
  000ecc3e: blx 0x0007312c
  000ecc42: subs r0,r5,#0x2
  000ecc44: cmp r0,#0x2
  000ecc46: bcc 0x000ecc5a
  000ecc48: cbz r5,0x000ecc9c
  000ecc4a: cmp r5,#0x1
  000ecc4c: ittt eq
  000ecc4e: mov.eq r0,#0x0
  000ecc50: strb.eq.w r0,[r4,#0x13d]
  000ecc54: strb.eq.w r0,[r4,#0x1e0]
  000ecc58: b 0x000eccb6
  000ecc5a: vmov.f32 s0,0x40b00000
  000ecc5e: vldr.32 s2,[r4,#0x1e8]
  000ecc62: movw r0,#0x1389
  000ecc66: str.w r0,[r4,#0x1b8]
  000ecc6a: vcmpe.f32 s2,s0
  000ecc6e: vmrs apsr,fpscr
  000ecc72: it ne
  000ecc74: str.w.ne r0,[r4,#0x1c0]
  000ecc78: cmp r5,#0x2
  000ecc7a: bne 0x000eccb0
  000ecc7c: ldr r0,[r4,#0x54]
  000ecc7e: blx 0x00071ed8
  000ecc82: cbz r0,0x000eccb2
  000ecc84: ldr r0,[r4,#0x54]
  000ecc86: blx 0x00071ed8
  000ecc8a: blx 0x000741a0
  000ecc8e: str.w r0,[r4,#0x14c]
  000ecc92: blx 0x00072988
  000ecc96: str.w r0,[r4,#0x1e4]
  000ecc9a: b 0x000eccb6
  000ecc9c: ldr.w r0,[r4,#0x140]
  000ecca0: str.w r8,[r4,#0xe4]
  000ecca4: clz r0,r0
  000ecca8: lsrs r0,r0,#0x5
  000eccaa: str.w r0,[r4,#0x140]
  000eccae: b 0x000eccb6
  000eccb0: cbnz r6,0x000eccb6
  000eccb2: str.w r8,[r4,#0xe4]
  000eccb6: ldr.w r0,[r4,#0x1a4]
  000eccba: ldr.w r1,[r4,#0x1ac]
  000eccbe: str.w r1,[r4,#0x1e8]
  000eccc2: str.w r0,[r4,#0x1f0]
  000eccc6: pop.w r8
  000eccca: pop {r4,r5,r6,r7,pc}
```
