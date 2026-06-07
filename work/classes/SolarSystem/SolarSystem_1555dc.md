# SolarSystem::SolarSystem
elf 0x1555dc body 156
Sig: undefined __thiscall SolarSystem(SolarSystem * this, int param_1, String param_2, int param_3, bool param_4, int param_5, int param_6, int param_7, int param_8, int param_9, int param_10, int * param_11, Array * param_12, Array * param_13, Array * param_14)

## decompile
```c

/* SolarSystem::SolarSystem(int, AbyssEngine::String, int, bool, int, int, int, int, int, int, int*,
   Array<int>*, Array<int>*, Array<int>*) */

void __thiscall
SolarSystem::SolarSystem
          (SolarSystem *this,undefined4 param_1,String *param_3,undefined4 param_4,
          SolarSystem param_5,undefined4 param_6,undefined4 param_7,undefined4 param_8,
          undefined4 param_9,undefined4 param_10,undefined4 param_11,undefined4 *param_12,
          undefined4 param_13,undefined4 param_14,undefined4 param_15)

{
  int iVar1;
  undefined4 uVar2;
  int *piVar3;
  
  piVar3 = *(int **)(DAT_00165690 + 0x1655f2);
  iVar1 = *piVar3;
  AbyssEngine::String::String((String *)(this + 0xc));
  *(undefined4 *)(this + 0x18) = param_1;
  AbyssEngine::String::String((String *)&stack0xffffffcc,param_3,false);
  AbyssEngine::String::operator=((String *)(this + 0xc),&stack0xffffffcc);
  AbyssEngine::String::~String((String *)&stack0xffffffcc);
  this[0x44] = param_5;
  *(undefined4 *)(this + 0x1c) = param_4;
  *(undefined4 *)(this + 0x20) = param_6;
  *(undefined4 *)(this + 0x24) = param_7;
  *(undefined4 *)(this + 0x28) = param_8;
  *(undefined4 *)(this + 0x2c) = param_9;
  *(undefined4 *)(this + 0x30) = param_10;
  *(undefined4 *)(this + 0x34) = param_11;
  *(undefined4 *)this = *param_12;
  *(undefined4 *)(this + 4) = param_12[1];
  uVar2 = param_12[2];
  *(undefined4 *)(this + 0x38) = param_13;
  *(undefined4 *)(this + 8) = uVar2;
  *(undefined4 *)(this + 0x3c) = param_15;
  *(undefined4 *)(this + 0x40) = param_14;
  iVar1 = *piVar3 - iVar1;
  if (iVar1 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar1);
  }
  return;
}

```

## target disasm
```
  001655dc: push {r4,r5,r6,r7,lr}
  001655de: add r7,sp,#0xc
  001655e0: push {r3,r4,r5,r6,r7,r8,r9,r10,r11}
  001655e4: mov r4,r0
  001655e6: ldr r0,[0x00165690]
  001655e8: add.w r9,r4,#0xc
  001655ec: mov r8,r3
  001655ee: add r0,pc
  001655f0: mov r5,r2
  001655f2: mov r6,r1
  001655f4: ldr.w r10,[r0,#0x0]
  001655f8: ldr.w r0,[r10,#0x0]
  001655fc: str r0,[sp,#0x10]
  001655fe: mov r0,r9
  00165600: blx 0x0006efbc
  00165604: str r6,[r4,#0x18]
  00165606: add r0,sp,#0x4
  00165608: mov r1,r5
  0016560a: movs r2,#0x0
  0016560c: blx 0x0006f028
  00165610: add r1,sp,#0x4
  00165612: mov r0,r9
  00165614: blx 0x0006f2b0
  00165618: add r0,sp,#0x4
  0016561a: ldr.w r9,[r7,#0x24]
  0016561e: ldr r5,[r7,#0x14]
  00165620: ldrd r11,r6,[r7,#0xc]
  00165624: blx 0x0006ee30
  00165628: ldr r0,[r7,#0x8]
  0016562a: strb.w r0,[r4,#0x44]
  0016562e: ldr r0,[r7,#0x18]
  00165630: strd r8,r11,[r4,#0x1c]
  00165634: strd r6,r5,[r4,#0x24]
  00165638: str r0,[r4,#0x2c]
  0016563a: ldr r0,[r7,#0x1c]
  0016563c: str r0,[r4,#0x30]
  0016563e: ldr r0,[r7,#0x20]
  00165640: str r0,[r4,#0x34]
  00165642: ldr.w r0,[r9,#0x0]
  00165646: str r0,[r4,#0x0]
  00165648: ldr.w r0,[r9,#0x4]
  0016564c: str r0,[r4,#0x4]
  0016564e: ldr.w r0,[r9,#0x8]
  00165652: ldr r1,[r7,#0x28]
  00165654: str r1,[r4,#0x38]
  00165656: str r0,[r4,#0x8]
  00165658: ldr r0,[r7,#0x30]
  0016565a: str r0,[r4,#0x3c]
  0016565c: ldr r0,[r7,#0x2c]
  0016565e: str r0,[r4,#0x40]
  00165660: ldr r0,[sp,#0x10]
  00165662: ldr.w r1,[r10,#0x0]
  00165666: subs r0,r1,r0
  00165668: itttt eq
  0016566a: mov.eq r0,r4
  0016566c: add.eq sp,#0x14
  0016566e: pop.eq.w {r8,r9,r10,r11}
  00165672: pop.eq {r4,r5,r6,r7,pc}
  00165674: blx 0x0006e824
```
