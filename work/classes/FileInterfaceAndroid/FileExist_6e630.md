# FileInterfaceAndroid::FileExist
elf 0x6e630 body 232
Sig: undefined __thiscall FileExist(FileInterfaceAndroid * this, String param_1)

## decompile
```c

/* FileInterfaceAndroid::FileExist(AbyssEngine::String) */

void __thiscall FileInterfaceAndroid::FileExist(FileInterfaceAndroid *this,String *param_2)

{
  String *this_00;
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  undefined4 uVar4;
  int *piVar5;
  String aSStack_54 [12];
  String aSStack_48 [12];
  String aSStack_3c [12];
  String aSStack_30 [12];
  int local_24;
  
  piVar5 = *(int **)(DAT_0007e74c + 0x7e646);
  local_24 = *piVar5;
  this_00 = (String *)AbyssEngine::String::String(aSStack_30,(char *)(DAT_0007e750 + 0x7e656),false)
  ;
  AbyssEngine::String::operator+=(this_00,param_2);
  AbyssEngine::String::String(aSStack_3c,(char *)(DAT_0007e754 + 0x7e664),false);
  AbyssEngine::String::operator+=(aSStack_3c,param_2);
  uVar4 = **(undefined4 **)(DAT_0007e758 + 0x7e678);
  uVar1 = AbyssEngine::String::GetAEChar();
  iVar2 = zip_fopen(uVar4,uVar1,0);
  uVar4 = **(undefined4 **)(DAT_0007e75c + 0x7e692);
  uVar1 = AbyssEngine::String::GetAEChar();
  iVar3 = zip_fopen(uVar4,uVar1,0);
  if (iVar2 == 0) {
    if (iVar3 == 0) {
      AbyssEngine::String::String(aSStack_48,*(char **)(this + 0x30),false);
      AbyssEngine::operator+(aSStack_54,aSStack_48);
      uVar1 = AbyssEngine::String::GetAEChar();
      iVar2 = fopen(uVar1,DAT_0007e760 + 0x7e6fc);
      if (iVar2 != 0) {
        fclose();
      }
      AbyssEngine::String::~String(aSStack_54);
      AbyssEngine::String::~String(aSStack_48);
    }
    else {
      zip_fclose();
    }
  }
  else {
    zip_fclose(iVar2);
  }
  AbyssEngine::String::~String(aSStack_3c);
  AbyssEngine::String::~String(aSStack_30);
  iVar2 = *piVar5 - local_24;
  if (iVar2 == 0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar2);
}

```

## target disasm
```
  0007e630: push {r4,r5,r6,r7,lr}
  0007e632: add r7,sp,#0xc
  0007e634: push {r8,r9,r11}
  0007e638: sub sp,#0x38
  0007e63a: mov r8,r0
  0007e63c: ldr r0,[0x0007e74c]
  0007e63e: mov r4,r1
  0007e640: movs r2,#0x0
  0007e642: add r0,pc
  0007e644: ldr.w r9,[r0,#0x0]
  0007e648: ldr.w r0,[r9,#0x0]
  0007e64c: ldr r1,[0x0007e750]
  0007e64e: str r0,[sp,#0x34]
  0007e650: add r0,sp,#0x28
  0007e652: add r1,pc
  0007e654: blx 0x0006ee18
  0007e658: mov r1,r4
  0007e65a: blx 0x0006ef5c
  0007e65e: ldr r1,[0x0007e754]
  0007e660: add r1,pc
  0007e662: add r0,sp,#0x1c
  0007e664: movs r2,#0x0
  0007e666: blx 0x0006ee18
  0007e66a: add r0,sp,#0x1c
  0007e66c: mov r1,r4
  0007e66e: blx 0x0006ef5c
  0007e672: ldr r0,[0x0007e758]
  0007e674: add r0,pc
  0007e676: ldr r0,[r0,#0x0]
  0007e678: ldr r6,[r0,#0x0]
  0007e67a: add r0,sp,#0x28
  0007e67c: blx 0x0006ef68
  0007e680: mov r1,r0
  0007e682: mov r0,r6
  0007e684: movs r2,#0x0
  0007e686: blx 0x0006ef80
  0007e68a: mov r6,r0
  0007e68c: ldr r0,[0x0007e75c]
  0007e68e: add r0,pc
  0007e690: ldr r0,[r0,#0x0]
  0007e692: ldr r5,[r0,#0x0]
  0007e694: add r0,sp,#0x1c
  0007e696: blx 0x0006ef68
  0007e69a: mov r1,r0
  0007e69c: mov r0,r5
  0007e69e: movs r2,#0x0
  0007e6a0: blx 0x0006ef80
  0007e6a4: cbz r6,0x0007e6ae
  0007e6a6: mov r0,r6
  0007e6a8: blx 0x0006f01c
  0007e6ac: b 0x0007e6b4
  0007e6ae: cbz r0,0x0007e6da
  0007e6b0: blx 0x0006f01c
  0007e6b4: movs r4,#0x1
  0007e6b6: add r0,sp,#0x1c
  0007e6b8: blx 0x0006ee30
  0007e6bc: add r0,sp,#0x28
  0007e6be: blx 0x0006ee30
  0007e6c2: ldr r0,[sp,#0x34]
  0007e6c4: ldr.w r1,[r9,#0x0]
  0007e6c8: subs r0,r1,r0
  0007e6ca: itttt eq
  0007e6cc: mov.eq r0,r4
  0007e6ce: add.eq sp,#0x38
  0007e6d0: pop.eq.w {r8,r9,r11}
  0007e6d4: pop.eq {r4,r5,r6,r7,pc}
  0007e6d6: blx 0x0006e824
  0007e6da: ldr.w r1,[r8,#0x30]
  0007e6de: add r0,sp,#0x10
  0007e6e0: movs r2,#0x0
  0007e6e2: blx 0x0006ee18
  0007e6e6: add r0,sp,#0x4
  0007e6e8: add r1,sp,#0x10
  0007e6ea: mov r2,r4
  0007e6ec: blx 0x0006ef98
  0007e6f0: add r0,sp,#0x4
  0007e6f2: blx 0x0006ef68
  0007e6f6: ldr r1,[0x0007e760]
  0007e6f8: add r1,pc
  0007e6fa: blx 0x0006e944
  0007e6fe: cbz r0,0x0007e708
  0007e700: blx 0x0006e980
  0007e704: movs r4,#0x1
  0007e706: b 0x0007e70a
  0007e708: movs r4,#0x0
  0007e70a: add r0,sp,#0x4
  0007e70c: blx 0x0006ee30
  0007e710: add r0,sp,#0x10
  0007e712: blx 0x0006ee30
  0007e716: b 0x0007e6b6
```
