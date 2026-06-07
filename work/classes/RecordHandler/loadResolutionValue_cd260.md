# RecordHandler::loadResolutionValue
elf 0xcd260 body 464
Sig: undefined __stdcall loadResolutionValue(float param_1)

## decompile
```c

/* RecordHandler::loadResolutionValue(float) */

void RecordHandler::loadResolutionValue(float param_1)

{
  int in_r0;
  int iVar1;
  String *pSVar2;
  code *pcVar3;
  code *pcVar4;
  int *piVar5;
  short local_a0;
  bool bStack_9d;
  int iStack_9c;
  String aSStack_98 [12];
  uint local_8c;
  undefined1 auStack_88 [4];
  undefined1 auStack_84 [4];
  float fStack_80;
  undefined1 uStack_7c;
  undefined1 uStack_7b;
  bool abStack_7a [2];
  undefined1 uStack_78;
  undefined1 auStack_77 [3];
  undefined1 auStack_74 [4];
  undefined1 auStack_70 [4];
  undefined1 auStack_6c [4];
  float fStack_68;
  undefined1 auStack_64 [4];
  undefined1 auStack_60 [10];
  bool bStack_56;
  bool bStack_55;
  bool bStack_54;
  bool bStack_53;
  bool bStack_52;
  bool abStack_51 [3];
  bool bStack_4e;
  bool bStack_4d;
  bool bStack_4c;
  bool bStack_4b;
  bool bStack_4a;
  bool bStack_49;
  bool bStack_48;
  bool abStack_47 [3];
  float local_44 [4];
  int iStack_34;
  int aiStack_30 [3];
  int local_24;
  
  piVar5 = *(int **)(DAT_000dd444 + 0xdd27a);
  local_24 = *piVar5;
  iVar1 = AEFile::FileExist((String *)(in_r0 + 8));
  if (iVar1 != 0) {
    AEFile::OpenRead((String *)(in_r0 + 8),&local_8c);
    pcVar4 = *(code **)(DAT_000dd448 + 0xdd2a0);
    (*pcVar4)(&uStack_78,local_8c);
    (*pcVar4)(auStack_77,local_8c);
    pcVar3 = *(code **)(DAT_000dd44c + 0xdd2b6);
    (*pcVar3)(auStack_74,local_8c);
    (*pcVar3)(auStack_70,local_8c);
    (*pcVar3)(auStack_6c,local_8c);
    (*pcVar3)(auStack_88,local_8c);
    (*pcVar4)(&uStack_7c,local_8c);
    (*pcVar4)(&uStack_7b,local_8c);
    (*pcVar3)(auStack_84,local_8c);
    (*pcVar3)(auStack_64,local_8c);
    (*pcVar3)(auStack_60,local_8c);
    pSVar2 = (String *)AbyssEngine::String::String(aSStack_98);
    AEFile::Read(pSVar2,local_8c,false);
    AEFile::Read(&iStack_9c,local_8c);
    AEFile::Read(&bStack_9d,local_8c);
    AEFile::Read(&iStack_34,local_8c);
    AEFile::Read(aiStack_30,local_8c);
    AEFile::Read(&fStack_80,local_8c);
    AEFile::Read(abStack_7a,local_8c);
    AEFile::Read(&bStack_56,local_8c);
    AEFile::Read(&bStack_55,local_8c);
    AEFile::Read(&bStack_54,local_8c);
    AEFile::Read(&fStack_68,local_8c);
    local_a0 = -1;
    AEFile::Read(&local_a0,local_8c);
    AEFile::Read(&bStack_9d,local_8c);
    AEFile::Read(&bStack_53,local_8c);
    AEFile::Read(&bStack_52,local_8c);
    AEFile::Read(&bStack_4e,local_8c);
    AEFile::Read(&bStack_4d,local_8c);
    AEFile::Read(&bStack_4c,local_8c);
    AEFile::Read(abStack_51,local_8c);
    AEFile::Read(&bStack_4b,local_8c);
    AEFile::Read(&bStack_4a,local_8c);
    AEFile::Read(&bStack_49,local_8c);
    AEFile::Read(&bStack_48,local_8c);
    AEFile::Read(abStack_47,local_8c);
    AEFile::Read(local_44,local_8c);
    AEFile::Close(local_8c);
    AbyssEngine::String::~String(aSStack_98);
  }
  iVar1 = *piVar5 - local_24;
  if (iVar1 == 0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar1);
}

```

## target disasm
```
  000dd260: push {r4,r5,r6,r7,lr}
  000dd262: add r7,sp,#0xc
  000dd264: push.w r8
  000dd268: vpush {d8}
  000dd26c: sub sp,#0x80
  000dd26e: mov r4,r1
  000dd270: ldr r1,[0x000dd444]
  000dd272: add.w r5,r0,#0x8
  000dd276: add r1,pc
  000dd278: mov r0,r5
  000dd27a: ldr.w r8,[r1,#0x0]
  000dd27e: ldr.w r1,[r8,#0x0]
  000dd282: str r1,[sp,#0x7c]
  000dd284: blx 0x0006fff4
  000dd288: cmp r0,#0x0
  000dd28a: beq.w 0x000dd40c
  000dd28e: add r1,sp,#0x14
  000dd290: mov r0,r5
  000dd292: blx 0x0006f34c
  000dd296: ldr r0,[0x000dd448]
  000dd298: add r4,sp,#0x18
  000dd29a: ldr r1,[sp,#0x14]
  000dd29c: add r0,pc
  000dd29e: ldr r6,[r0,#0x0]
  000dd2a0: add.w r0,r4,#0x10
  000dd2a4: blx r6
  000dd2a6: ldr r1,[sp,#0x14]
  000dd2a8: add.w r0,r4,#0x11
  000dd2ac: blx r6
  000dd2ae: ldr r0,[0x000dd44c]
  000dd2b0: ldr r1,[sp,#0x14]
  000dd2b2: add r0,pc
  000dd2b4: ldr r5,[r0,#0x0]
  000dd2b6: add.w r0,r4,#0x14
  000dd2ba: blx r5
  000dd2bc: ldr r1,[sp,#0x14]
  000dd2be: add.w r0,r4,#0x18
  000dd2c2: blx r5
  000dd2c4: ldr r1,[sp,#0x14]
  000dd2c6: add.w r0,r4,#0x1c
  000dd2ca: blx r5
  000dd2cc: ldr r1,[sp,#0x14]
  000dd2ce: mov r0,r4
  000dd2d0: blx r5
  000dd2d2: ldr r1,[sp,#0x14]
  000dd2d4: add.w r0,r4,#0xc
  000dd2d8: blx r6
  000dd2da: ldr r1,[sp,#0x14]
  000dd2dc: add.w r0,r4,#0xd
  000dd2e0: blx r6
  000dd2e2: ldr r1,[sp,#0x14]
  000dd2e4: adds r0,r4,#0x4
  000dd2e6: blx r5
  000dd2e8: ldr r1,[sp,#0x14]
  000dd2ea: add.w r0,r4,#0x24
  000dd2ee: blx r5
  000dd2f0: ldr r1,[sp,#0x14]
  000dd2f2: add.w r0,r4,#0x28
  000dd2f6: blx r5
  000dd2f8: add r0,sp,#0x8
  000dd2fa: blx 0x0006efbc
  000dd2fe: ldr r1,[sp,#0x14]
  000dd300: movs r2,#0x0
  000dd302: blx 0x00074824
  000dd306: ldr r1,[sp,#0x14]
  000dd308: add r0,sp,#0x4
  000dd30a: blx 0x00074818
  000dd30e: ldr r1,[sp,#0x14]
  000dd310: sub.w r0,r7,#0x95
  000dd314: blx 0x00074854
  000dd318: ldr r1,[sp,#0x14]
  000dd31a: add.w r0,r4,#0x54
  000dd31e: blx 0x00074818
  000dd322: ldr r1,[sp,#0x14]
  000dd324: add.w r0,r4,#0x58
  000dd328: blx 0x00074818
  000dd32c: ldr r1,[sp,#0x14]
  000dd32e: add.w r0,r4,#0x8
  000dd332: blx 0x00074830
  000dd336: ldr r1,[sp,#0x14]
  000dd338: add.w r0,r4,#0xe
  000dd33c: blx 0x00074854
  000dd340: ldr r1,[sp,#0x14]
  000dd342: add.w r0,r4,#0x32
  000dd346: blx 0x00074854
  000dd34a: ldr r1,[sp,#0x14]
  000dd34c: add.w r0,r4,#0x33
  000dd350: blx 0x00074854
  000dd354: ldr r1,[sp,#0x14]
  000dd356: add.w r0,r4,#0x34
  000dd35a: blx 0x00074854
  000dd35e: ldr r1,[sp,#0x14]
  000dd360: add.w r0,r4,#0x20
  000dd364: blx 0x00074830
  000dd368: ldr r1,[sp,#0x14]
  000dd36a: movw r0,#0xffff
  000dd36e: strh.w r0,[sp,#0x0]
  000dd372: mov r0,sp
  000dd374: blx 0x00074908
  000dd378: ldr r1,[sp,#0x14]
  000dd37a: sub.w r0,r7,#0x95
  000dd37e: blx 0x00074854
  000dd382: ldr r1,[sp,#0x14]
  000dd384: add.w r0,r4,#0x35
  000dd388: blx 0x00074854
  000dd38c: ldr r1,[sp,#0x14]
  000dd38e: add.w r0,r4,#0x36
  000dd392: blx 0x00074854
  000dd396: ldr r1,[sp,#0x14]
  000dd398: add.w r0,r4,#0x3a
  000dd39c: blx 0x00074854
  000dd3a0: ldr r1,[sp,#0x14]
  000dd3a2: add.w r0,r4,#0x3b
  000dd3a6: blx 0x00074854
  000dd3aa: ldr r1,[sp,#0x14]
  000dd3ac: add.w r0,r4,#0x3c
  000dd3b0: blx 0x00074854
  000dd3b4: ldr r1,[sp,#0x14]
  000dd3b6: add.w r0,r4,#0x37
  000dd3ba: blx 0x00074854
  000dd3be: ldr r1,[sp,#0x14]
  000dd3c0: add.w r0,r4,#0x3d
  000dd3c4: blx 0x00074854
  000dd3c8: ldr r1,[sp,#0x14]
  000dd3ca: add.w r0,r4,#0x3e
  000dd3ce: blx 0x00074854
  000dd3d2: ldr r1,[sp,#0x14]
  000dd3d4: add.w r0,r4,#0x3f
  000dd3d8: blx 0x00074854
  000dd3dc: ldr r1,[sp,#0x14]
  000dd3de: add.w r0,r4,#0x40
  000dd3e2: blx 0x00074854
  000dd3e6: ldr r1,[sp,#0x14]
  000dd3e8: add.w r0,r4,#0x41
  000dd3ec: blx 0x00074854
  000dd3f0: ldr r1,[sp,#0x14]
  000dd3f2: add.w r0,r4,#0x44
  000dd3f6: blx 0x00074830
  000dd3fa: ldr r0,[sp,#0x14]
  000dd3fc: vldr.32 s16,[sp,#0x5c]
  000dd400: blx 0x0006ece0
  000dd404: add r0,sp,#0x8
  000dd406: blx 0x0006ee30
  000dd40a: b 0x000dd410
  000dd40c: vmov s16,r4
  000dd410: ldr r0,[sp,#0x7c]
  000dd412: ldr.w r1,[r8,#0x0]
  000dd416: subs r0,r1,r0
  000dd418: itttt eq
  000dd41a: vmov.eq r0,s16
  000dd41e: add.eq sp,#0x80
  000dd420: vpop.eq {d8}
  000dd424: pop.eq.w r8
  000dd428: it eq
  000dd42a: pop.eq {r4,r5,r6,r7,pc}
  000dd42c: blx 0x0006e824
```
