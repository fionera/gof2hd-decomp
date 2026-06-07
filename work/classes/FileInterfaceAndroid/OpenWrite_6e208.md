# FileInterfaceAndroid::OpenWrite
elf 0x6e208 body 172
Sig: undefined __stdcall OpenWrite(String param_1, int param_2, bool param_3, uint param_4)

## decompile
```c

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* FileInterfaceAndroid::OpenWrite(AbyssEngine::String, int, bool, unsigned int) */

void FileInterfaceAndroid::OpenWrite(int param_1)

{
  ushort uVar1;
  int iVar2;
  ushort *puVar3;
  ushort *puVar4;
  undefined4 uVar5;
  int iVar6;
  int *piVar7;
  undefined4 *puVar8;
  int *piVar9;
  int *piVar10;
  String aSStack_40 [12];
  String aSStack_34 [12];
  String aSStack_28 [12];
  int local_1c;
  
  piVar10 = *(int **)(_LAB_0007e2d4 + 0x7e21a);
  local_1c = *piVar10;
  puVar3 = (ushort *)AbyssEngine::String::GetAEWChar();
  puVar4 = puVar3;
  do {
    uVar1 = *puVar4;
    puVar4 = puVar4 + 1;
  } while (uVar1 != 0);
  AbyssEngine::String::String(aSStack_28,*(char **)(param_1 + 0x30),false);
  AbyssEngine::String::String(aSStack_40,puVar3,false);
  AbyssEngine::operator+(aSStack_34,aSStack_28);
  AbyssEngine::String::~String(aSStack_40);
  uVar5 = AbyssEngine::String::GetAEChar();
  iVar6 = fopen(uVar5,_LAB_0007e2d8 + 0x7e25e);
  if (iVar6 != 0) {
    piVar7 = operator_new(0x38);
    iVar2 = _LAB_0007e2e0;
    puVar8 = (undefined4 *)(_LAB_0007e2dc + 0x7e276);
    piVar7[2] = iVar6;
    piVar7[3] = 0;
    piVar7[4] = 0;
    piVar9 = (int *)*puVar8;
    iVar6 = *(int *)(iVar2 + 0x7e27c);
    *(undefined1 *)(piVar7 + 5) = 1;
    *piVar7 = iVar6 + 8;
    *piVar9 = *piVar9 + 1;
  }
  AbyssEngine::String::~String(aSStack_34);
  AbyssEngine::String::~String(aSStack_28);
  iVar6 = *piVar10 - local_1c;
  if (iVar6 == 0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar6);
}

```

## target disasm
```
  0007e208: push {r4,r5,r6,r7,lr}
  0007e20a: add r7,sp,#0xc
  0007e20c: push.w r11
  0007e210: sub sp,#0x28
  0007e212: mov r5,r0
  0007e214: ldr r0,[0x0007e2d4]
  0007e216: add r0,pc
  0007e218: ldr r6,[r0,#0x0]
  0007e21a: ldr r0,[r6,#0x0]
  0007e21c: str r0,[sp,#0x24]
  0007e21e: mov r0,r1
  0007e220: blx 0x0006ef44
  0007e224: mov r4,r0
  0007e226: ldrh.w r1,[r0],#0x2
  0007e22a: cmp r1,#0x0
  0007e22c: bne 0x0007e226
  0007e22e: ldr r1,[r5,#0x30]
  0007e230: add r0,sp,#0x18
  0007e232: movs r2,#0x0
  0007e234: blx 0x0006ee18
  0007e238: mov r0,sp
  0007e23a: mov r1,r4
  0007e23c: movs r2,#0x0
  0007e23e: blx 0x0006ef50
  0007e242: add r0,sp,#0xc
  0007e244: add r1,sp,#0x18
  0007e246: mov r2,sp
  0007e248: blx 0x0006ef98
  0007e24c: mov r0,sp
  0007e24e: blx 0x0006ee30
  0007e252: add r0,sp,#0xc
  0007e254: blx 0x0006ef68
  0007e258: ldr r1,[0x0007e2d8]
  0007e25a: add r1,pc
  0007e25c: blx 0x0006e944
  0007e260: mov r5,r0
  0007e262: cbz r0,0x0007e290
  0007e264: movs r0,#0x38
  0007e266: blx 0x0006eb24
  0007e26a: mov r4,r0
  0007e26c: ldr r0,[0x0007e2dc]
  0007e26e: ldr r1,[0x0007e2e0]
  0007e270: movs r2,#0x0
  0007e272: add r0,pc
  0007e274: strd r5,r2,[r4,#0x8]
  0007e278: add r1,pc
  0007e27a: str r2,[r4,#0x10]
  0007e27c: ldr r0,[r0,#0x0]
  0007e27e: movs r2,#0x1
  0007e280: ldr r1,[r1,#0x0]
  0007e282: strb r2,[r4,#0x14]
  0007e284: adds r1,#0x8
  0007e286: str r1,[r4,#0x0]
  0007e288: ldr r1,[r0,#0x0]
  0007e28a: adds r1,#0x1
  0007e28c: str r1,[r0,#0x0]
  0007e28e: b 0x0007e292
  0007e290: movs r4,#0x0
  0007e292: add r0,sp,#0xc
  0007e294: blx 0x0006ee30
  0007e298: add r0,sp,#0x18
  0007e29a: blx 0x0006ee30
  0007e29e: ldr r0,[sp,#0x24]
  0007e2a0: ldr r1,[r6,#0x0]
  0007e2a2: subs r0,r1,r0
  0007e2a4: itttt eq
  0007e2a6: mov.eq r0,r4
  0007e2a8: add.eq sp,#0x28
  0007e2aa: pop.eq.w r11
  0007e2ae: pop.eq {r4,r5,r6,r7,pc}
  0007e2b0: blx 0x0006e824
```
