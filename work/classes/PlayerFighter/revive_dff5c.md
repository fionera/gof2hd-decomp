# PlayerFighter::revive
elf 0xdff5c body 278
Sig: undefined __thiscall revive(PlayerFighter * this)

## decompile
```c

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* PlayerFighter::revive() */

void __thiscall PlayerFighter::revive(PlayerFighter *this)

{
  int iVar1;
  undefined4 uVar2;
  void *pvVar3;
  Generator *this_00;
  int *piVar4;
  String aSStack_28 [12];
  int local_1c;
  
  piVar4 = *(int **)(_LAB_000f008c + 0xeff6e);
  local_1c = *piVar4;
  iVar1 = Player::turnedEnemy();
  Player::reset();
  if (iVar1 != 0) {
    Player::turnEnemy();
  }
  AbyssEngine::String::String(aSStack_28);
  AbyssEngine::String::operator=((String *)(this + 0x18),aSStack_28);
  AbyssEngine::String::~String(aSStack_28);
  *(undefined4 *)(this + 0x78) = 0;
  *(undefined4 *)(this + 0x88) = 1;
  this[0x12e] = (PlayerFighter)0x0;
  *(undefined4 *)(this + 0x38) = 0xffffffff;
  Route::reset();
  uVar2 = Player::getHitpoints();
  *(undefined4 *)(this + 0x1d8) = uVar2;
  *(undefined4 *)(this + 0x1dc) = 0;
  this[0x1e0] = (PlayerFighter)0x0;
  *(undefined4 *)(this + 0xd8) = 0;
  KIPlayer::setActive(SUB41(this,0));
  *(undefined4 *)(this + 0xf0) = 0;
  *(undefined4 *)(this + 0x1e8) = *(undefined4 *)(this + 0x1ac);
  Explosion::reset(*(Explosion **)(this + 0x124));
  *(undefined4 *)(this + 0x104) = 0;
  this[0x24] = (PlayerFighter)0x0;
  setExhaustVisible(SUB41(this,0));
  iVar1 = *(int *)(this + 0xc);
  this[0xf5] = (PlayerFighter)0x1;
  if (iVar1 == 0) {
    iVar1 = *(int *)(this + 8);
  }
  AEGeometry::setVisible(SUB41(iVar1,0));
  if (*(int *)(this + 0x28) - 9U < 2) {
    if (*(Array<int> **)(this + 0x50) != (Array<int> *)0x0) {
      pvVar3 = (void *)Array<int>::~Array(*(Array<int> **)(this + 0x50));
      operator_delete(pvVar3);
    }
    *(undefined4 *)(this + 0x50) = 0;
  }
  else {
    this_00 = operator_new(1);
    Generator::Generator(this_00);
    if (*(Array<int> **)(this + 0x50) != (Array<int> *)0x0) {
      pvVar3 = (void *)Array<int>::~Array(*(Array<int> **)(this + 0x50));
      operator_delete(pvVar3);
      *(undefined4 *)(this + 0x50) = 0;
    }
    uVar2 = Generator::getLootList((int)this_00,-1);
    *(undefined4 *)(this + 0x50) = uVar2;
    pvVar3 = (void *)Generator::~Generator(this_00);
    operator_delete(pvVar3);
  }
  if (*piVar4 != local_1c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000eff5c: push {r4,r5,r6,r7,lr}
  000eff5e: add r7,sp,#0xc
  000eff60: push.w r8
  000eff64: sub sp,#0x10
  000eff66: mov r4,r0
  000eff68: ldr r0,[0x000f008c]
  000eff6a: add r0,pc
  000eff6c: ldr.w r8,[r0,#0x0]
  000eff70: ldr.w r0,[r8,#0x0]
  000eff74: str r0,[sp,#0xc]
  000eff76: ldr r0,[r4,#0x4]
  000eff78: blx 0x00075814
  000eff7c: mov r5,r0
  000eff7e: ldr r0,[r4,#0x4]
  000eff80: blx 0x000732d0
  000eff84: cbz r5,0x000eff8c
  000eff86: ldr r0,[r4,#0x4]
  000eff88: blx 0x000744e8
  000eff8c: mov r5,sp
  000eff8e: mov r0,r5
  000eff90: blx 0x0006efbc
  000eff94: add.w r0,r4,#0x18
  000eff98: mov r1,r5
  000eff9a: blx 0x0006f2b0
  000eff9e: mov r0,sp
  000effa0: blx 0x0006ee30
  000effa4: movs r6,#0x0
  000effa6: movs r5,#0x1
  000effa8: str r6,[r4,#0x78]
  000effaa: mov.w r1,#0xffffffff
  000effae: str.w r5,[r4,#0x88]
  000effb2: strb.w r6,[r4,#0x12e]
  000effb6: ldr r0,[r4,#0x6c]
  000effb8: str r1,[r4,#0x38]
  000effba: blx 0x000732e8
  000effbe: ldr r0,[r4,#0x4]
  000effc0: blx 0x000724f0
  000effc4: strd r0,r6,[r4,#0x1d8]
  000effc8: mov r0,r4
  000effca: movs r1,#0x1
  000effcc: strb.w r6,[r4,#0x1e0]
  000effd0: str.w r6,[r4,#0xd8]
  000effd4: blx 0x000732f4
  000effd8: ldr.w r0,[r4,#0x124]
  000effdc: ldr.w r1,[r4,#0x1ac]
  000effe0: str.w r6,[r4,#0xf0]
  000effe4: str.w r1,[r4,#0x1e8]
  000effe8: blx 0x00072c88
  000effec: mov r0,r4
  000effee: movs r1,#0x1
  000efff0: str.w r6,[r4,#0x104]
  000efff4: strb.w r6,[r4,#0x24]
  000efff8: blx 0x000721f0
  000efffc: ldr r0,[r4,#0xc]
  000efffe: strb.w r5,[r4,#0xf5]
  000f0002: cbnz r0,0x000f0006
  000f0004: ldr r0,[r4,#0x8]
  000f0006: movs r1,#0x1
  000f0008: blx 0x00072d24
  000f000c: ldr r0,[r4,#0x28]
  000f000e: subs r0,#0x9
  000f0010: cmp r0,#0x1
  000f0012: bhi 0x000f0026
  000f0014: ldr r0,[r4,#0x50]
  000f0016: cbz r0,0x000f0020
  000f0018: blx 0x00070204
  000f001c: blx 0x0006eb48
  000f0020: movs r0,#0x0
  000f0022: str r0,[r4,#0x50]
  000f0024: b 0x000f005c
  000f0026: movs r0,#0x1
  000f0028: blx 0x0006eb24
  000f002c: mov r5,r0
  000f002e: blx 0x000736cc
  000f0032: ldr r0,[r4,#0x50]
  000f0034: cbz r0,0x000f0042
  000f0036: blx 0x00070204
  000f003a: blx 0x0006eb48
  000f003e: movs r0,#0x0
  000f0040: str r0,[r4,#0x50]
  000f0042: mov r0,r5
  000f0044: mov.w r1,#0xffffffff
  000f0048: mov.w r2,#0xffffffff
  000f004c: blx 0x000740c8
  000f0050: str r0,[r4,#0x50]
  000f0052: mov r0,r5
  000f0054: blx 0x000740d4
  000f0058: blx 0x0006eb48
  000f005c: ldr r0,[sp,#0xc]
  000f005e: ldr.w r1,[r8,#0x0]
  000f0062: subs r0,r1,r0
  000f0064: ittt eq
  000f0066: add.eq sp,#0x10
  000f0068: pop.eq.w r8
  000f006c: pop.eq {r4,r5,r6,r7,pc}
  000f006e: blx 0x0006e824
```
