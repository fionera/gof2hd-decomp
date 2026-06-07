# RecordHandler::saveOptions
elf 0xcd8c8 body 510
Sig: undefined __thiscall saveOptions(RecordHandler * this)

## decompile
```c

/* RecordHandler::saveOptions() */

void __thiscall RecordHandler::saveOptions(RecordHandler *this)

{
  short sVar1;
  int iVar2;
  int iVar3;
  String *pSVar4;
  code *pcVar5;
  RecordHandler *pRVar6;
  code *pcVar7;
  uint unaff_r6;
  undefined4 *puVar8;
  int *piVar9;
  code *pcVar10;
  String aSStack_34 [4];
  
  pRVar6 = this + 8;
  piVar9 = *(int **)(DAT_000ddad4 + 0xdd8dc);
  iVar2 = *piVar9;
  iVar3 = AEFile::FileExist(pRVar6);
  if (iVar3 != 0) {
    AEFile::FileDelete(pRVar6);
  }
  AEFile::OpenWrite(pRVar6,(uint *)&stack0xffffffd8);
  puVar8 = *(undefined4 **)(DAT_000ddad8 + 0xdd902);
  pcVar7 = *(code **)(DAT_000ddadc + 0xdd906);
  (*pcVar7)(*(undefined1 *)(puVar8 + 4),unaff_r6);
  (*pcVar7)(*(undefined1 *)((int)puVar8 + 0x11),unaff_r6);
  pcVar5 = *(code **)(DAT_000ddae0 + 0xdd91a);
  (*pcVar5)(puVar8[5],unaff_r6);
  (*pcVar5)(puVar8[6],unaff_r6);
  (*pcVar5)(puVar8[7],unaff_r6);
  (*pcVar5)(*puVar8,unaff_r6);
  (*pcVar7)(*(undefined1 *)(puVar8 + 3),unaff_r6);
  (*pcVar7)(*(undefined1 *)((int)puVar8 + 0xd),unaff_r6);
  (*pcVar5)(puVar8[1],unaff_r6);
  (*pcVar5)(puVar8[9],unaff_r6);
  (*pcVar5)(puVar8[10],unaff_r6);
  if ((String *)**(undefined4 **)(DAT_000ddae4 + 0xdd954) == (String *)0x0) {
    pSVar4 = (String *)
             AbyssEngine::String::String(aSStack_34,(char *)(DAT_000ddae8 + 0xdd96c),false);
    AEFile::Write(pSVar4,unaff_r6,false);
    AbyssEngine::String::~String(aSStack_34);
  }
  else {
    AEFile::Write((String *)**(undefined4 **)(DAT_000ddae4 + 0xdd954),unaff_r6,false);
  }
  pcVar10 = *(code **)(DAT_000ddaf0 + 0xdd988);
  (*pcVar10)(**(undefined4 **)(DAT_000ddaec + 0xdd984),unaff_r6);
  (*pcVar7)(**(undefined1 **)(DAT_000ddaf4 + 0xdd998),unaff_r6);
  (*pcVar10)(puVar8[0x15],unaff_r6);
  (*pcVar10)(puVar8[0x16],unaff_r6);
  (*pcVar5)(puVar8[2],unaff_r6);
  (*pcVar7)(*(undefined1 *)((int)puVar8 + 0xe),unaff_r6);
  (*pcVar7)(*(undefined1 *)((int)puVar8 + 0x32),unaff_r6);
  (*pcVar7)(*(undefined1 *)((int)puVar8 + 0x33),unaff_r6);
  (*pcVar7)(*(undefined1 *)(puVar8 + 0xd),unaff_r6);
  (*pcVar5)(puVar8[8],unaff_r6);
  sVar1 = GameText::getLanguage();
  AEFile::Write(sVar1,unaff_r6);
  (*pcVar7)(**(undefined1 **)(DAT_000ddaf8 + 0xdd9e4),unaff_r6);
  (*pcVar7)(*(undefined1 *)((int)puVar8 + 0x35),unaff_r6);
  (*pcVar7)(*(undefined1 *)((int)puVar8 + 0x36),unaff_r6);
  (*pcVar7)(*(undefined1 *)((int)puVar8 + 0x3a),unaff_r6);
  (*pcVar7)(*(undefined1 *)((int)puVar8 + 0x3b),unaff_r6);
  (*pcVar7)(*(undefined1 *)(puVar8 + 0xf),unaff_r6);
  (*pcVar7)(*(undefined1 *)((int)puVar8 + 0x37),unaff_r6);
  (*pcVar7)(*(undefined1 *)((int)puVar8 + 0x3d),unaff_r6);
  (*pcVar7)(*(undefined1 *)((int)puVar8 + 0x3e),unaff_r6);
  (*pcVar7)(*(undefined1 *)((int)puVar8 + 0x3f),unaff_r6);
  (*pcVar7)(*(undefined1 *)(puVar8 + 0x10),unaff_r6);
  (*pcVar7)(*(undefined1 *)((int)puVar8 + 0x41),unaff_r6);
  (*pcVar5)(puVar8[0x11],unaff_r6);
  (*pcVar7)(*(undefined1 *)(puVar8 + 0x12),unaff_r6);
  (*pcVar7)(*(undefined1 *)(puVar8 + 0xe),unaff_r6);
  (*pcVar7)(*(undefined1 *)((int)puVar8 + 0x4e),unaff_r6);
  (*pcVar7)(*(undefined1 *)((int)puVar8 + 0x49),unaff_r6);
  (*pcVar7)(*(undefined1 *)((int)puVar8 + 0x4a),unaff_r6);
  (*pcVar7)(*(undefined1 *)((int)puVar8 + 0x4b),unaff_r6);
  (*pcVar7)(*(undefined1 *)(puVar8 + 0x13),unaff_r6);
  (*pcVar7)(*(undefined1 *)((int)puVar8 + 0x4d),unaff_r6);
  (*pcVar10)(puVar8[0x14],unaff_r6);
  (*pcVar7)(*(undefined1 *)(puVar8 + 0x18),unaff_r6);
  (*pcVar7)(*(undefined1 *)((int)puVar8 + 0x61),unaff_r6);
  (*pcVar7)(*(undefined1 *)((int)puVar8 + 0x62),unaff_r6);
  AEFile::Close(unaff_r6);
  addHashToOptions(this);
  if (*piVar9 != iVar2) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000dd8c8: push {r4,r5,r6,r7,lr}
  000dd8ca: add r7,sp,#0xc
  000dd8cc: push {r2,r3,r4,r5,r6,r7,r8,r9,r10}
  000dd8d0: mov r9,r0
  000dd8d2: ldr r0,[0x000ddad4]
  000dd8d4: add.w r5,r9,#0x8
  000dd8d8: add r0,pc
  000dd8da: ldr.w r8,[r0,#0x0]
  000dd8de: ldr.w r0,[r8,#0x0]
  000dd8e2: str r0,[sp,#0x14]
  000dd8e4: mov r0,r5
  000dd8e6: blx 0x0006fff4
  000dd8ea: cbz r0,0x000dd8f2
  000dd8ec: mov r0,r5
  000dd8ee: blx 0x0007495c
  000dd8f2: add r1,sp,#0x10
  000dd8f4: mov r0,r5
  000dd8f6: blx 0x0006f3dc
  000dd8fa: ldr r0,[0x000ddad8]
  000dd8fc: ldr r2,[0x000ddadc]
  000dd8fe: add r0,pc
  000dd900: ldr r1,[sp,#0x10]
  000dd902: add r2,pc
  000dd904: ldr r6,[r0,#0x0]
  000dd906: ldr r5,[r2,#0x0]
  000dd908: ldrb r0,[r6,#0x10]
  000dd90a: blx r5
  000dd90c: ldrb r0,[r6,#0x11]
  000dd90e: ldr r1,[sp,#0x10]
  000dd910: blx r5
  000dd912: ldr r2,[0x000ddae0]
  000dd914: ldr r0,[r6,#0x14]
  000dd916: add r2,pc
  000dd918: ldr r1,[sp,#0x10]
  000dd91a: ldr r4,[r2,#0x0]
  000dd91c: blx r4
  000dd91e: ldr r0,[r6,#0x18]
  000dd920: ldr r1,[sp,#0x10]
  000dd922: blx r4
  000dd924: ldr r0,[r6,#0x1c]
  000dd926: ldr r1,[sp,#0x10]
  000dd928: blx r4
  000dd92a: ldr r0,[r6,#0x0]
  000dd92c: ldr r1,[sp,#0x10]
  000dd92e: blx r4
  000dd930: ldrb r0,[r6,#0xc]
  000dd932: ldr r1,[sp,#0x10]
  000dd934: blx r5
  000dd936: ldrb r0,[r6,#0xd]
  000dd938: ldr r1,[sp,#0x10]
  000dd93a: blx r5
  000dd93c: ldr r0,[r6,#0x4]
  000dd93e: ldr r1,[sp,#0x10]
  000dd940: blx r4
  000dd942: ldr r0,[r6,#0x24]
  000dd944: ldr r1,[sp,#0x10]
  000dd946: blx r4
  000dd948: ldr r0,[r6,#0x28]
  000dd94a: ldr r1,[sp,#0x10]
  000dd94c: blx r4
  000dd94e: ldr r0,[0x000ddae4]
  000dd950: add r0,pc
  000dd952: ldr r0,[r0,#0x0]
  000dd954: ldr r0,[r0,#0x0]
  000dd956: cbz r0,0x000dd962
  000dd958: ldr r1,[sp,#0x10]
  000dd95a: movs r2,#0x0
  000dd95c: blx 0x00074968
  000dd960: b 0x000dd97c
  000dd962: ldr r1,[0x000ddae8]
  000dd964: add r0,sp,#0x4
  000dd966: movs r2,#0x0
  000dd968: add r1,pc
  000dd96a: blx 0x0006ee18
  000dd96e: ldr r1,[sp,#0x10]
  000dd970: movs r2,#0x0
  000dd972: blx 0x00074968
  000dd976: add r0,sp,#0x4
  000dd978: blx 0x0006ee30
  000dd97c: ldr r0,[0x000ddaec]
  000dd97e: ldr r2,[0x000ddaf0]
  000dd980: add r0,pc
  000dd982: ldr r1,[sp,#0x10]
  000dd984: add r2,pc
  000dd986: ldr r0,[r0,#0x0]
  000dd988: ldr.w r10,[r2,#0x0]
  000dd98c: ldr r0,[r0,#0x0]
  000dd98e: blx r10
  000dd990: ldr r0,[0x000ddaf4]
  000dd992: ldr r1,[sp,#0x10]
  000dd994: add r0,pc
  000dd996: ldr r0,[r0,#0x0]
  000dd998: ldrb r0,[r0,#0x0]
  000dd99a: blx r5
  000dd99c: ldr r0,[r6,#0x54]
  000dd99e: ldr r1,[sp,#0x10]
  000dd9a0: blx r10
  000dd9a2: ldr r0,[r6,#0x58]
  000dd9a4: ldr r1,[sp,#0x10]
  000dd9a6: blx r10
  000dd9a8: ldr r0,[r6,#0x8]
  000dd9aa: ldr r1,[sp,#0x10]
  000dd9ac: blx r4
  000dd9ae: ldrb r0,[r6,#0xe]
  000dd9b0: ldr r1,[sp,#0x10]
  000dd9b2: blx r5
  000dd9b4: ldrb.w r0,[r6,#0x32]
  000dd9b8: ldr r1,[sp,#0x10]
  000dd9ba: blx r5
  000dd9bc: ldrb.w r0,[r6,#0x33]
  000dd9c0: ldr r1,[sp,#0x10]
  000dd9c2: blx r5
  000dd9c4: ldrb.w r0,[r6,#0x34]
  000dd9c8: ldr r1,[sp,#0x10]
  000dd9ca: blx r5
  000dd9cc: ldr r0,[r6,#0x20]
  000dd9ce: ldr r1,[sp,#0x10]
  000dd9d0: blx r4
  000dd9d2: blx 0x0006f544
  000dd9d6: ldr r1,[sp,#0x10]
  000dd9d8: blx 0x00074974
  000dd9dc: ldr r0,[0x000ddaf8]
  000dd9de: ldr r1,[sp,#0x10]
  000dd9e0: add r0,pc
  000dd9e2: ldr r0,[r0,#0x0]
  000dd9e4: ldrb r0,[r0,#0x0]
  000dd9e6: blx r5
  000dd9e8: ldrb.w r0,[r6,#0x35]
  000dd9ec: ldr r1,[sp,#0x10]
  000dd9ee: blx r5
  000dd9f0: ldrb.w r0,[r6,#0x36]
  000dd9f4: ldr r1,[sp,#0x10]
  000dd9f6: blx r5
  000dd9f8: ldrb.w r0,[r6,#0x3a]
  000dd9fc: ldr r1,[sp,#0x10]
  000dd9fe: blx r5
  000dda00: ldrb.w r0,[r6,#0x3b]
  000dda04: ldr r1,[sp,#0x10]
  000dda06: blx r5
  000dda08: ldrb.w r0,[r6,#0x3c]
  000dda0c: ldr r1,[sp,#0x10]
  000dda0e: blx r5
  000dda10: ldrb.w r0,[r6,#0x37]
  000dda14: ldr r1,[sp,#0x10]
  000dda16: blx r5
  000dda18: ldrb.w r0,[r6,#0x3d]
  000dda1c: ldr r1,[sp,#0x10]
  000dda1e: blx r5
  000dda20: ldrb.w r0,[r6,#0x3e]
  000dda24: ldr r1,[sp,#0x10]
  000dda26: blx r5
  000dda28: ldrb.w r0,[r6,#0x3f]
  000dda2c: ldr r1,[sp,#0x10]
  000dda2e: blx r5
  000dda30: ldrb.w r0,[r6,#0x40]
  000dda34: ldr r1,[sp,#0x10]
  000dda36: blx r5
  000dda38: ldrb.w r0,[r6,#0x41]
  000dda3c: ldr r1,[sp,#0x10]
  000dda3e: blx r5
  000dda40: ldr r0,[r6,#0x44]
  000dda42: ldr r1,[sp,#0x10]
  000dda44: blx r4
  000dda46: ldrb.w r0,[r6,#0x48]
  000dda4a: ldr r1,[sp,#0x10]
  000dda4c: blx r5
  000dda4e: ldrb.w r0,[r6,#0x38]
  000dda52: ldr r1,[sp,#0x10]
  000dda54: blx r5
  000dda56: ldrb.w r0,[r6,#0x4e]
  000dda5a: ldr r1,[sp,#0x10]
  000dda5c: blx r5
  000dda5e: ldrb.w r0,[r6,#0x49]
  000dda62: ldr r1,[sp,#0x10]
  000dda64: blx r5
  000dda66: ldrb.w r0,[r6,#0x4a]
  000dda6a: ldr r1,[sp,#0x10]
  000dda6c: blx r5
  000dda6e: ldrb.w r0,[r6,#0x4b]
  000dda72: ldr r1,[sp,#0x10]
  000dda74: blx r5
  000dda76: ldrb.w r0,[r6,#0x4c]
  000dda7a: ldr r1,[sp,#0x10]
  000dda7c: blx r5
  000dda7e: ldrb.w r0,[r6,#0x4d]
  000dda82: ldr r1,[sp,#0x10]
  000dda84: blx r5
  000dda86: ldr r0,[r6,#0x50]
  000dda88: ldr r1,[sp,#0x10]
  000dda8a: blx r10
  000dda8c: ldrb.w r0,[r6,#0x60]
  000dda90: ldr r1,[sp,#0x10]
  000dda92: blx r5
  000dda94: ldrb.w r0,[r6,#0x61]
  000dda98: ldr r1,[sp,#0x10]
  000dda9a: blx r5
  000dda9c: ldrb.w r0,[r6,#0x62]
  000ddaa0: ldr r1,[sp,#0x10]
  000ddaa2: blx r5
  000ddaa4: ldr r0,[sp,#0x10]
  000ddaa6: blx 0x0006ece0
  000ddaaa: mov r0,r9
  000ddaac: blx 0x00074980
  000ddab0: ldr r0,[sp,#0x14]
  000ddab2: ldr.w r1,[r8,#0x0]
  000ddab6: subs r0,r1,r0
  000ddab8: ittt eq
  000ddaba: add.eq sp,#0x18
  000ddabc: pop.eq.w {r8,r9,r10}
  000ddac0: pop.eq {r4,r5,r6,r7,pc}
  000ddac2: blx 0x0006e824
```
