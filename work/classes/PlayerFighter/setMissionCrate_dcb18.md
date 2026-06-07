# PlayerFighter::setMissionCrate
elf 0xdcb18 body 78
Sig: undefined __thiscall setMissionCrate(PlayerFighter * this, bool param_1)

## decompile
```c

/* PlayerFighter::setMissionCrate(bool) */

void __thiscall PlayerFighter::setMissionCrate(PlayerFighter *this,bool param_1)

{
  void *pvVar1;
  int iVar2;
  int iVar3;
  
  this[0xd0] = (PlayerFighter)param_1;
  if (!param_1) {
    return;
  }
  *(undefined4 *)(this + 0x50) = 0;
  pvVar1 = operator_new(0xc);
  Array<int>::Array();
  *(void **)(this + 0x50) = pvVar1;
  Status::getMission();
  iVar2 = Mission::getType();
  iVar3 = 0x75;
  if (iVar2 == 5) {
    iVar3 = 0x74;
  }
  ArrayAdd<int>(iVar3,*(Array **)(this + 0x50));
  (*(code *)(DAT_001ac1b4 + 0x1ac1b8))(1,*(undefined4 *)(this + 0x50));
  return;
}

```

## target disasm
```
  000ecb18: push {r4,r5,r7,lr}
  000ecb1a: add r7,sp,#0x8
  000ecb1c: mov r4,r0
  000ecb1e: cmp r1,#0x0
  000ecb20: strb.w r1,[r0,#0xd0]
  000ecb24: beq 0x000ecb64
  000ecb26: movs r0,#0x0
  000ecb28: str r0,[r4,#0x50]
  000ecb2a: movs r0,#0xc
  000ecb2c: blx 0x0006eb24
  000ecb30: mov r5,r0
  000ecb32: blx 0x000701f8
  000ecb36: ldr r0,[0x000ecb74]
  000ecb38: str r5,[r4,#0x50]
  000ecb3a: add r0,pc
  000ecb3c: ldr r0,[r0,#0x0]
  000ecb3e: ldr r0,[r0,#0x0]
  000ecb40: blx 0x00072850
  000ecb44: blx 0x00072874
  000ecb48: movs r2,#0x75
  000ecb4a: ldr r1,[r4,#0x50]
  000ecb4c: cmp r0,#0x5
  000ecb4e: it eq
  000ecb50: mov.eq r2,#0x74
  000ecb52: mov r0,r2
  000ecb54: blx 0x0007021c
  000ecb58: ldr r1,[r4,#0x50]
  000ecb5a: movs r0,#0x1
  000ecb5c: pop.w {r4,r5,r7,lr}
  000ecb60: b.w 0x001ac1a8
  000ecb64: pop {r4,r5,r7,pc}
```
