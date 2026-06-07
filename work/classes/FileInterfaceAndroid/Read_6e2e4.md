# FileInterfaceAndroid::Read
elf 0x6e2e4 body 316
Sig: undefined __thiscall Read(FileInterfaceAndroid * this, uint param_1, void * param_2)

## decompile
```c

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* FileInterfaceAndroid::Read(unsigned int, void*) */

bool __thiscall FileInterfaceAndroid::Read(FileInterfaceAndroid *this,uint param_1,void *param_2)

{
  uint uVar1;
  undefined4 uVar2;
  int iVar3;
  char *pcVar4;
  String *this_00;
  bool bVar5;
  int *piVar6;
  undefined4 *puVar7;
  String aSStack_40 [12];
  String aSStack_34 [12];
  int local_28;
  
  piVar6 = *(int **)(_LAB_0007e43c + 0x7e2fa);
  local_28 = *piVar6;
  if (*(int *)(this + 0xc) != 0) {
    uVar1 = zip_fread(*(int *)(this + 0xc),param_2,param_1);
    if (*piVar6 == local_28) {
      return uVar1 == param_1;
    }
    goto LAB_0007e41c;
  }
  if (*(int *)(this + 8) == 0) {
    if (*(int *)(this + 0x10) == 0) {
      bVar5 = false;
    }
    else {
      puVar7 = *(undefined4 **)(_LAB_0007e440 + 0x7e340);
      uVar2 = (**(code **)(*(int *)*puVar7 + 0x2c0))((int *)*puVar7,param_1);
      uVar1 = _JNIEnv::CallIntMethod
                        ((_jobject *)*puVar7,*(_jmethodID **)(this + 0x10),
                         **(undefined4 **)(_LAB_0007e444 + 0x7e35c),uVar2);
      iVar3 = (**(code **)(*(int *)*puVar7 + 0x3c))();
      if (iVar3 == 0) {
        if (uVar1 != param_1) goto LAB_0007e3fc;
        (**(code **)(*(int *)*puVar7 + 800))((int *)*puVar7,uVar2,0,param_1,param_2);
        AbyssEngine::String::String(aSStack_34);
        pcVar4 = (char *)(_LAB_0007e448 + 0x7e3ba);
        for (uVar1 = 0; uVar1 < param_1; uVar1 = uVar1 + 1) {
          AbyssEngine::String::String(aSStack_40,pcVar4,false);
          AbyssEngine::String::operator+=(aSStack_34,aSStack_40);
          this_00 = (String *)AbyssEngine::String::~String(aSStack_40);
          AbyssEngine::String::String(this_00,*(char *)((int)param_2 + uVar1));
          AbyssEngine::String::operator+=(aSStack_34,aSStack_40);
          AbyssEngine::String::~String(aSStack_40);
        }
        AbyssEngine::String::~String(aSStack_34);
        bVar5 = true;
      }
      else {
        (**(code **)(*(int *)*puVar7 + 0x40))();
        (**(code **)(*(int *)*puVar7 + 0x44))();
LAB_0007e3fc:
        bVar5 = false;
      }
      (**(code **)(*(int *)*puVar7 + 0x5c))((int *)*puVar7,uVar2);
    }
  }
  else {
    uVar1 = fread(param_2,1,param_1);
    bVar5 = uVar1 == param_1;
  }
  if (*piVar6 == local_28) {
    return bVar5;
  }
LAB_0007e41c:
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```

## target disasm
```
  0007e2e4: push {r4,r5,r6,r7,lr}
  0007e2e6: add r7,sp,#0xc
  0007e2e8: push {r8,r9,r10,r11}
  0007e2ec: sub sp,#0x24
  0007e2ee: mov r5,r0
  0007e2f0: ldr r0,[0x0007e43c]
  0007e2f2: mov r10,r2
  0007e2f4: mov r4,r1
  0007e2f6: add r0,pc
  0007e2f8: ldr r6,[r0,#0x0]
  0007e2fa: ldr r0,[r6,#0x0]
  0007e2fc: str r0,[sp,#0x20]
  0007e2fe: ldr r0,[r5,#0xc]
  0007e300: cbz r0,0x0007e31e
  0007e302: mov r1,r10
  0007e304: mov r2,r4
  0007e306: blx 0x0006efa4
  0007e30a: ldr r1,[sp,#0x20]
  0007e30c: ldr r2,[r6,#0x0]
  0007e30e: subs r1,r2,r1
  0007e310: bne.w 0x0007e41c
  0007e314: subs r0,r0,r4
  0007e316: clz r0,r0
  0007e31a: lsrs r0,r0,#0x5
  0007e31c: b 0x0007e414
  0007e31e: ldr r3,[r5,#0x8]
  0007e320: cbz r3,0x0007e336
  0007e322: mov r0,r10
  0007e324: movs r1,#0x1
  0007e326: mov r2,r4
  0007e328: blx 0x0006e974
  0007e32c: subs r0,r0,r4
  0007e32e: clz r0,r0
  0007e332: lsrs r4,r0,#0x5
  0007e334: b 0x0007e40a
  0007e336: ldr r0,[r5,#0x10]
  0007e338: cbz r0,0x0007e38a
  0007e33a: ldr r0,[0x0007e440]
  0007e33c: add r0,pc
  0007e33e: ldr.w r11,[r0,#0x0]
  0007e342: ldr.w r0,[r11,#0x0]
  0007e346: ldr r1,[r0,#0x0]
  0007e348: ldr.w r2,[r1,#0x2c0]
  0007e34c: mov r1,r4
  0007e34e: blx r2
  0007e350: mov r9,r0
  0007e352: ldr r0,[0x0007e444]
  0007e354: ldr r1,[r5,#0x10]
  0007e356: mov r3,r9
  0007e358: add r0,pc
  0007e35a: ldr r2,[r0,#0x0]
  0007e35c: ldr.w r0,[r11,#0x0]
  0007e360: ldr r2,[r2,#0x0]
  0007e362: blx 0x0006efb0
  0007e366: mov r5,r0
  0007e368: ldr.w r0,[r11,#0x0]
  0007e36c: ldr r1,[r0,#0x0]
  0007e36e: ldr r1,[r1,#0x3c]
  0007e370: blx r1
  0007e372: cbz r0,0x0007e38e
  0007e374: ldr.w r0,[r11,#0x0]
  0007e378: ldr r1,[r0,#0x0]
  0007e37a: ldr r1,[r1,#0x40]
  0007e37c: blx r1
  0007e37e: ldr.w r0,[r11,#0x0]
  0007e382: ldr r1,[r0,#0x0]
  0007e384: ldr r1,[r1,#0x44]
  0007e386: blx r1
  0007e388: b 0x0007e3fc
  0007e38a: movs r4,#0x0
  0007e38c: b 0x0007e40a
  0007e38e: cmp r5,r4
  0007e390: bne 0x0007e3fc
  0007e392: ldr.w r0,[r11,#0x0]
  0007e396: movs r2,#0x0
  0007e398: mov r3,r4
  0007e39a: mov.w r8,#0x0
  0007e39e: ldr r1,[r0,#0x0]
  0007e3a0: ldr.w r5,[r1,#0x320]
  0007e3a4: mov r1,r9
  0007e3a6: str.w r10,[sp,#0x0]
  0007e3aa: blx r5
  0007e3ac: add r0,sp,#0x14
  0007e3ae: blx 0x0006efbc
  0007e3b2: ldr r0,[0x0007e448]
  0007e3b4: add r5,sp,#0x8
  0007e3b6: add r0,pc
  0007e3b8: str r0,[sp,#0x4]
  0007e3ba: b 0x0007e3ee
  0007e3bc: ldr r1,[sp,#0x4]
  0007e3be: mov r0,r5
  0007e3c0: movs r2,#0x0
  0007e3c2: blx 0x0006ee18
  0007e3c6: add r0,sp,#0x14
  0007e3c8: mov r1,r5
  0007e3ca: blx 0x0006ef5c
  0007e3ce: mov r0,r5
  0007e3d0: blx 0x0006ee30
  0007e3d4: ldrb.w r1,[r10,r8]
  0007e3d8: blx 0x0006efc8
  0007e3dc: add r0,sp,#0x14
  0007e3de: mov r1,r5
  0007e3e0: blx 0x0006ef5c
  0007e3e4: mov r0,r5
  0007e3e6: blx 0x0006ee30
  0007e3ea: add.w r8,r8,#0x1
  0007e3ee: cmp r8,r4
  0007e3f0: bcc 0x0007e3bc
  0007e3f2: add r0,sp,#0x14
  0007e3f4: blx 0x0006ee30
  0007e3f8: movs r4,#0x1
  0007e3fa: b 0x0007e3fe
  0007e3fc: movs r4,#0x0
  0007e3fe: ldr.w r0,[r11,#0x0]
  0007e402: ldr r1,[r0,#0x0]
  0007e404: ldr r2,[r1,#0x5c]
  0007e406: mov r1,r9
  0007e408: blx r2
  0007e40a: ldr r0,[sp,#0x20]
  0007e40c: ldr r1,[r6,#0x0]
  0007e40e: subs r0,r1,r0
  0007e410: bne 0x0007e41c
  0007e412: mov r0,r4
  0007e414: add sp,#0x24
  0007e416: pop.w {r8,r9,r10,r11}
  0007e41a: pop {r4,r5,r6,r7,pc}
  0007e41c: blx 0x0006e824
```
