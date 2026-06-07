# KIPlayer::setShipGroup
elf 0xa5d40 body 134
Sig: undefined __stdcall setShipGroup(AEGeometry * param_1, int param_2, bool param_3)

## decompile
```c

/* KIPlayer::setShipGroup(AEGeometry*, int, bool) */

void KIPlayer::setShipGroup(AEGeometry *param_1,int param_2,bool param_3)

{
  void *pvVar1;
  Matrix *pMVar2;
  int in_r3;
  AEGeometry *this;
  float in_s0;
  float extraout_s0;
  float extraout_s0_00;
  float in_s1;
  float extraout_s1;
  float extraout_s1_00;
  float in_s2;
  float extraout_s2;
  float extraout_s2_00;
  
  *(uint *)(param_1 + 0x7c) = (uint)param_3;
  if ((param_2 == 0) || (in_r3 == 0)) {
    *(int *)(param_1 + 8) = param_2;
    if (*(AEGeometry **)(param_1 + 0xc) != (AEGeometry *)0x0) {
      pvVar1 = (void *)AEGeometry::~AEGeometry(*(AEGeometry **)(param_1 + 0xc));
      operator_delete(pvVar1);
      in_s0 = extraout_s0_00;
      in_s1 = extraout_s1_00;
      in_s2 = extraout_s2_00;
    }
    *(undefined4 *)(param_1 + 0xc) = 0;
  }
  else {
    this = *(AEGeometry **)(param_1 + 8);
    *(int *)(param_1 + 0xc) = param_2;
    if (this == (AEGeometry *)0x0) {
      this = operator_new(0xc0);
      AEGeometry::AEGeometry(this,(PaintCanvas *)**(undefined4 **)(DAT_000b5dd4 + 0xb5d62));
      *(AEGeometry **)(param_1 + 8) = this;
    }
    AEGeometry::addChild((uint)this);
    *(undefined4 *)(*(int *)(param_1 + 0xc) + 0x24) = *(undefined4 *)(*(int *)(param_1 + 8) + 0xc);
    in_s0 = extraout_s0;
    in_s1 = extraout_s1;
    in_s2 = extraout_s2;
  }
  AEGeometry::setPosition(in_s0,in_s1,in_s2);
  pMVar2 = (Matrix *)AEGeometry::getMatrix();
  AbyssEngine::AEMath::Matrix::operator=((Matrix *)(*(int *)(param_1 + 4) + 4),pMVar2);
  if (*(int *)(param_1 + 0xc) != 0) {
    pMVar2 = (Matrix *)AEGeometry::getMatrix();
    AbyssEngine::AEMath::Matrix::operator*=((Matrix *)(*(int *)(param_1 + 4) + 4),pMVar2);
  }
  return;
}

```

## target disasm
```
  000b5d40: push {r4,r5,r7,lr}
  000b5d42: add r7,sp,#0x8
  000b5d44: mov r4,r0
  000b5d46: cmp r1,#0x0
  000b5d48: str r2,[r0,#0x7c]
  000b5d4a: beq 0x000b5d80
  000b5d4c: cbz r3,0x000b5d80
  000b5d4e: ldr r5,[r4,#0x8]
  000b5d50: str r1,[r4,#0xc]
  000b5d52: cbnz r5,0x000b5d6e
  000b5d54: movs r0,#0xc0
  000b5d56: blx 0x0006eb24
  000b5d5a: mov r5,r0
  000b5d5c: ldr r0,[0x000b5dd4]
  000b5d5e: add r0,pc
  000b5d60: ldr r0,[r0,#0x0]
  000b5d62: ldr r1,[r0,#0x0]
  000b5d64: mov r0,r5
  000b5d66: blx 0x000720f4
  000b5d6a: ldr r1,[r4,#0xc]
  000b5d6c: str r5,[r4,#0x8]
  000b5d6e: ldr r1,[r1,#0xc]
  000b5d70: mov r0,r5
  000b5d72: blx 0x000720e8
  000b5d76: ldrd r1,r0,[r4,#0x8]
  000b5d7a: ldr r2,[r1,#0xc]
  000b5d7c: str r2,[r0,#0x24]
  000b5d7e: b 0x000b5d94
  000b5d80: ldr r0,[r4,#0xc]
  000b5d82: str r1,[r4,#0x8]
  000b5d84: cbz r0,0x000b5d90
  000b5d86: blx 0x00071fc8
  000b5d8a: blx 0x0006eb48
  000b5d8e: ldr r1,[r4,#0x8]
  000b5d90: movs r0,#0x0
  000b5d92: str r0,[r4,#0xc]
  000b5d94: ldrd r5,r2,[r4,#0x58]
  000b5d98: mov r0,r1
  000b5d9a: ldr r3,[r4,#0x60]
  000b5d9c: mov r1,r5
  000b5d9e: blx 0x00073048
  000b5da2: ldr r0,[r4,#0x8]
  000b5da4: blx 0x000721cc
  000b5da8: mov r1,r0
  000b5daa: ldr r0,[r4,#0x4]
  000b5dac: adds r0,#0x4
  000b5dae: blx 0x0006f148
  000b5db2: ldr r0,[r4,#0xc]
  000b5db4: cbz r0,0x000b5dc4
  000b5db6: blx 0x000721cc
  000b5dba: mov r1,r0
  000b5dbc: ldr r0,[r4,#0x4]
  000b5dbe: adds r0,#0x4
  000b5dc0: blx 0x0006f4a8
  000b5dc4: pop {r4,r5,r7,pc}
```
