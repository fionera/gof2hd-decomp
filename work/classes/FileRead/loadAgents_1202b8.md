# FileRead::loadAgents
elf 0x1202b8 body 404
Sig: undefined __stdcall loadAgents(void)

## decompile
```c

/* FileRead::loadAgents() */

void FileRead::loadAgents(void)

{
  int iVar1;
  uint uVar2;
  String *pSVar3;
  int iVar4;
  Array<Agent*> *this;
  Agent *pAVar5;
  int *piVar6;
  uint *extraout_r2;
  uint uVar7;
  int *piVar8;
  uint uVar9;
  char cStack_6d;
  String aSStack_6c [12];
  int local_60;
  int local_5c;
  int local_58;
  int local_54;
  int iStack_50;
  int local_4c;
  int iStack_48;
  int local_44;
  int iStack_40;
  int local_3c;
  uint local_38;
  String aSStack_34 [12];
  int local_28;
  
  piVar8 = *(int **)(DAT_00130484 + 0x1302ca);
  local_28 = *piVar8;
  pSVar3 = (String *)AbyssEngine::String::String(aSStack_34,(char *)(DAT_00130488 + 0x1302d6),false)
  ;
  iVar4 = AEFile::FileExist(pSVar3);
  AbyssEngine::String::~String(aSStack_34);
  if (iVar4 != 0) {
    AEFile::OpenRead((AEFile *)(DAT_0013048c + 0x1302f2),(char *)&local_38,extraout_r2);
    this = operator_new(0xc);
    Array<Agent*>::Array(this);
    ArraySetLength<Agent*>(0x1b,(Array *)this);
    AbyssEngine::String::String(aSStack_34);
    local_60 = -1;
    uVar2 = 0;
    while (uVar9 = uVar2, uVar9 < *(uint *)this) {
      AEFile::ReadSwitched(aSStack_34,local_38);
      AbyssEngine::String::ConvertFromUTF8(aSStack_34);
      AEFile::ReadSwitched(&local_3c,local_38);
      AEFile::ReadSwitched(&iStack_40,local_38);
      AEFile::ReadSwitched(&local_44,local_38);
      AEFile::ReadSwitched(&iStack_48,local_38);
      AEFile::ReadSwitched(&local_58,local_38);
      iVar4 = local_58;
      AEFile::ReadSwitched(&local_4c,local_38);
      AEFile::ReadSwitched(&iStack_50,local_38);
      AEFile::ReadSwitched(&local_60,local_38);
      AEFile::ReadSwitched(&local_54,local_38);
      pAVar5 = operator_new(0x98);
      iVar1 = local_3c;
      AbyssEngine::String::String(aSStack_6c,aSStack_34,false);
      Agent::Agent(pAVar5,iVar1,aSStack_6c,iStack_40,local_44,iStack_48,iVar4 == 1,local_4c,
                   iStack_50,local_60,local_54);
      *(Agent **)(*(int *)(this + 4) + uVar9 * 4) = pAVar5;
      AbyssEngine::String::~String(aSStack_6c);
      AEFile::ReadSwitched(&local_5c,local_38);
      uVar2 = uVar9 + 1;
      if (0 < local_5c) {
        piVar6 = operator_new__(0x14);
        for (uVar7 = 0; uVar7 < 5; uVar7 = uVar7 + 1) {
          AEFile::Read(&cStack_6d,local_38);
          piVar6[uVar7] = (int)cStack_6d;
        }
        Agent::setImageParts(*(Agent **)(*(int *)(this + 4) + uVar9 * 4),piVar6);
      }
    }
    AEFile::Close(local_38);
    AbyssEngine::String::~String(aSStack_34);
  }
  iVar4 = *piVar8 - local_28;
  if (iVar4 == 0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar4);
}

```

## target disasm
```
  001302b8: push {r4,r5,r6,r7,lr}
  001302ba: add r7,sp,#0xc
  001302bc: push {r8,r9,r10,r11}
  001302c0: sub sp,#0x74
  001302c2: ldr r0,[0x00130484]
  001302c4: movs r2,#0x0
  001302c6: add r0,pc
  001302c8: ldr r5,[r0,#0x0]
  001302ca: ldr r0,[r5,#0x0]
  001302cc: ldr r1,[0x00130488]
  001302ce: str r0,[sp,#0x70]
  001302d0: add r0,sp,#0x64
  001302d2: add r1,pc
  001302d4: blx 0x0006ee18
  001302d8: blx 0x0006fff4
  001302dc: mov r4,r0
  001302de: add r0,sp,#0x64
  001302e0: blx 0x0006ee30
  001302e4: cmp r4,#0x0
  001302e6: beq.w 0x00130434
  001302ea: ldr r0,[0x0013048c]
  001302ec: add r1,sp,#0x60
  001302ee: add r0,pc
  001302f0: blx 0x0006ecd4
  001302f4: movs r0,#0xc
  001302f6: blx 0x0006eb24
  001302fa: mov r6,r0
  001302fc: blx 0x00071ad0
  00130300: movs r0,#0x1b
  00130302: mov r1,r6
  00130304: str r5,[sp,#0x20]
  00130306: blx 0x00071adc
  0013030a: add r4,sp,#0x64
  0013030c: mov r0,r4
  0013030e: blx 0x0006efbc
  00130312: mov.w r0,#0xffffffff
  00130316: mov.w r9,#0x0
  0013031a: str r0,[sp,#0x38]
  0013031c: str r6,[sp,#0x24]
  0013031e: b 0x00130418
  00130320: mov r0,r4
  00130322: blx 0x000762f4
  00130326: mov r0,r4
  00130328: blx 0x00076300
  0013032c: ldr r1,[sp,#0x60]
  0013032e: add r0,sp,#0x5c
  00130330: blx 0x0007630c
  00130334: ldr r1,[sp,#0x60]
  00130336: add r0,sp,#0x58
  00130338: blx 0x0007630c
  0013033c: ldr r1,[sp,#0x60]
  0013033e: add r0,sp,#0x54
  00130340: blx 0x0007630c
  00130344: ldr r1,[sp,#0x60]
  00130346: add r0,sp,#0x50
  00130348: blx 0x0007630c
  0013034c: ldr r1,[sp,#0x60]
  0013034e: add r0,sp,#0x40
  00130350: blx 0x0007630c
  00130354: ldr.w r10,[sp,#0x40]
  00130358: ldr r1,[sp,#0x60]
  0013035a: add r0,sp,#0x4c
  0013035c: blx 0x0007630c
  00130360: ldr r1,[sp,#0x60]
  00130362: add r0,sp,#0x48
  00130364: blx 0x0007630c
  00130368: ldr r1,[sp,#0x60]
  0013036a: add r0,sp,#0x38
  0013036c: blx 0x0007630c
  00130370: ldr r1,[sp,#0x60]
  00130372: add r0,sp,#0x44
  00130374: blx 0x0007630c
  00130378: movs r0,#0x98
  0013037a: blx 0x0006eb24
  0013037e: ldr r6,[sp,#0x5c]
  00130380: mov r9,r0
  00130382: add r0,sp,#0x2c
  00130384: mov r1,r4
  00130386: movs r2,#0x0
  00130388: blx 0x0006f028
  0013038c: ldrd r0,r3,[sp,#0x54]
  00130390: mov r8,r4
  00130392: ldrd lr,r1,[sp,#0x4c]
  00130396: ldr r5,[sp,#0x38]
  00130398: ldrd r12,r4,[sp,#0x44]
  0013039c: sub.w r2,r10,#0x1
  001303a0: clz r2,r2
  001303a4: lsrs r2,r2,#0x5
  001303a6: stm sp,{r0,r1,r2,lr}
  001303aa: mov r0,r9
  001303ac: mov r1,r6
  001303ae: strd r4,r5,[sp,#0x10]
  001303b2: add r4,sp,#0x2c
  001303b4: str.w r12,[sp,#0x18]
  001303b8: mov r2,r4
  001303ba: blx 0x00071b30
  001303be: ldr r6,[sp,#0x24]
  001303c0: ldr r0,[r6,#0x4]
  001303c2: str.w r9,[r0,r11,lsl #0x2]
  001303c6: mov r0,r4
  001303c8: blx 0x0006ee30
  001303cc: ldr r1,[sp,#0x60]
  001303ce: add r0,sp,#0x3c
  001303d0: blx 0x0007630c
  001303d4: ldr r0,[sp,#0x3c]
  001303d6: add.w r9,r11,#0x1
  001303da: sub.w r5,r7,#0x65
  001303de: mov r4,r8
  001303e0: cmp r0,#0x1
  001303e2: blt 0x00130418
  001303e4: movs r0,#0x14
  001303e6: blx 0x0006ec08
  001303ea: mov r6,r0
  001303ec: movs r4,#0x0
  001303ee: b 0x00130402
  001303f0: ldr r1,[sp,#0x60]
  001303f2: mov r0,r5
  001303f4: blx 0x00076324
  001303f8: ldrsb.w r0,[r7,#-0x65]
  001303fc: str.w r0,[r6,r4,lsl #0x2]
  00130400: adds r4,#0x1
  00130402: cmp r4,#0x5
  00130404: bcc 0x001303f0
  00130406: ldr r0,[sp,#0x24]
  00130408: ldr r0,[r0,#0x4]
  0013040a: ldr.w r0,[r0,r11,lsl #0x2]
  0013040e: mov r1,r6
  00130410: blx 0x00071b54
  00130414: mov r4,r8
  00130416: ldr r6,[sp,#0x24]
  00130418: ldr r0,[r6,#0x0]
  0013041a: mov r11,r9
  0013041c: ldr r1,[sp,#0x60]
  0013041e: cmp r9,r0
  00130420: bcc.w 0x00130320
  00130424: mov r0,r1
  00130426: blx 0x0006ece0
  0013042a: add r0,sp,#0x64
  0013042c: blx 0x0006ee30
  00130430: ldr r5,[sp,#0x20]
  00130432: b 0x00130436
  00130434: movs r6,#0x0
  00130436: ldr r0,[sp,#0x70]
  00130438: ldr r1,[r5,#0x0]
  0013043a: subs r0,r1,r0
  0013043c: itttt eq
  0013043e: mov.eq r0,r6
  00130440: add.eq sp,#0x74
  00130442: pop.eq.w {r8,r9,r10,r11}
  00130446: pop.eq {r4,r5,r6,r7,pc}
  00130448: blx 0x0006e824
```
