# PendingProduct::PendingProduct
elf 0x12d3c8 body 98
Sig: undefined __thiscall PendingProduct(PendingProduct * this, BluePrint * param_1)

## decompile
```c

/* PendingProduct::PendingProduct(BluePrint*) */

void __thiscall PendingProduct::PendingProduct(PendingProduct *this,BluePrint *param_1)

{
  undefined4 uVar1;
  int iVar2;
  int *piVar3;
  String aSStack_28 [12];
  int local_1c;
  
  piVar3 = *(int **)(DAT_0013d444 + 0x13d3dc);
  local_1c = *piVar3;
  AbyssEngine::String::String((String *)this);
  uVar1 = BluePrint::getIndex(param_1);
  *(undefined4 *)(this + 0x14) = uVar1;
  BluePrint::getStationName();
  AbyssEngine::String::operator=((String *)this,aSStack_28);
  AbyssEngine::String::~String(aSStack_28);
  uVar1 = BluePrint::getStationIndex(param_1);
  *(undefined4 *)(this + 0xc) = uVar1;
  uVar1 = BluePrint::getQuantity(param_1);
  *(undefined4 *)(this + 0x10) = uVar1;
  iVar2 = *piVar3 - local_1c;
  if (iVar2 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar2);
  }
  return;
}

```

## target disasm
```
  0013d3c8: push {r4,r5,r6,r7,lr}
  0013d3ca: add r7,sp,#0xc
  0013d3cc: push.w r11
  0013d3d0: sub sp,#0x10
  0013d3d2: mov r4,r0
  0013d3d4: ldr r0,[0x0013d444]
  0013d3d6: mov r5,r1
  0013d3d8: add r0,pc
  0013d3da: ldr r6,[r0,#0x0]
  0013d3dc: ldr r0,[r6,#0x0]
  0013d3de: str r0,[sp,#0xc]
  0013d3e0: mov r0,r4
  0013d3e2: blx 0x0006efbc
  0013d3e6: mov r0,r5
  0013d3e8: blx 0x000733fc
  0013d3ec: str r0,[r4,#0x14]
  0013d3ee: mov r0,sp
  0013d3f0: mov r1,r5
  0013d3f2: blx 0x0007672c
  0013d3f6: mov r1,sp
  0013d3f8: mov r0,r4
  0013d3fa: blx 0x0006f2b0
  0013d3fe: mov r0,sp
  0013d400: blx 0x0006ee30
  0013d404: mov r0,r5
  0013d406: blx 0x00073624
  0013d40a: str r0,[r4,#0xc]
  0013d40c: mov r0,r5
  0013d40e: blx 0x00073654
  0013d412: str r0,[r4,#0x10]
  0013d414: ldr r0,[sp,#0xc]
  0013d416: ldr r1,[r6,#0x0]
  0013d418: subs r0,r1,r0
  0013d41a: itttt eq
  0013d41c: mov.eq r0,r4
  0013d41e: add.eq sp,#0x10
  0013d420: pop.eq.w r11
  0013d424: pop.eq {r4,r5,r6,r7,pc}
  0013d426: blx 0x0006e824
```
