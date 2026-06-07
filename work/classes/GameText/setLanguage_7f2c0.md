# GameText::setLanguage
elf 0x7f2c0 body 710
Sig: undefined __stdcall setLanguage(short param_1, int param_2)

## decompile
```c

/* GameText::setLanguage(short, int) */

void GameText::setLanguage(short param_1,int param_2)

{
  ushort uVar1;
  GameText *this;
  int iVar2;
  uint uVar3;
  void *pvVar4;
  int iVar5;
  uint in_r2;
  int *piVar6;
  ushort *puVar7;
  GameText *pGStack_40;
  int iStack_3c;
  String aSStack_34 [4];
  
  this = (GameText *)(int)param_1;
  piVar6 = *(int **)(DAT_0008f5f8 + 0x8f2d4);
  puVar7 = *(ushort **)(DAT_0008f5fc + 0x8f2d8);
  iVar2 = *piVar6;
  pGStack_40 = this;
  iStack_3c = param_2;
  if ((uint)*puVar7 == (param_2 & 0xffffU)) goto LAB_0008f580;
  release(this);
  *(uint *)(this + 0x1c) = in_r2;
  uVar3 = (uint)((ulonglong)in_r2 * 4);
  if ((int)((ulonglong)in_r2 * 4 >> 0x20) != 0) {
    uVar3 = 0xffffffff;
  }
  pvVar4 = operator_new__(uVar3);
  *(void **)(this + 0xc) = pvVar4;
  for (iVar5 = 0; iVar5 < (int)in_r2; iVar5 = iVar5 + 1) {
    *(undefined4 *)(*(int *)(this + 0xc) + iVar5 * 4) = 0;
  }
  AbyssEngine::String::String(aSStack_34,(char *)(DAT_0008f600 + 0x8f330),false);
  switch(param_2) {
  case 0:
    AbyssEngine::String::String((String *)&pGStack_40,(char *)(DAT_0008f604 + 0x8f354),false);
    AbyssEngine::String::operator=(aSStack_34,(String *)&pGStack_40);
    goto LAB_0008f52a;
  case 1:
    AbyssEngine::String::String((String *)&pGStack_40,(char *)(DAT_0008f608 + 0x8f36a),false);
    AbyssEngine::String::operator=(aSStack_34,(String *)&pGStack_40);
    AbyssEngine::String::~String((String *)&pGStack_40);
    uVar1 = 1;
    break;
  case 2:
    AbyssEngine::String::String((String *)&pGStack_40,(char *)(DAT_0008f610 + 0x8f388),false);
    AbyssEngine::String::operator=(aSStack_34,(String *)&pGStack_40);
    AbyssEngine::String::~String((String *)&pGStack_40);
    uVar1 = 2;
    break;
  case 3:
    AbyssEngine::String::String((String *)&pGStack_40,(char *)(DAT_0008f60c + 0x8f3a6),false);
    AbyssEngine::String::operator=(aSStack_34,(String *)&pGStack_40);
    AbyssEngine::String::~String((String *)&pGStack_40);
    uVar1 = 3;
    break;
  case 4:
    AbyssEngine::String::String((String *)&pGStack_40,(char *)(DAT_0008f614 + 0x8f3c4),false);
    AbyssEngine::String::operator=(aSStack_34,(String *)&pGStack_40);
    AbyssEngine::String::~String((String *)&pGStack_40);
    uVar1 = 4;
    break;
  case 5:
    AbyssEngine::String::String((String *)&pGStack_40,(char *)(DAT_0008f618 + 0x8f3e2),false);
    AbyssEngine::String::operator=(aSStack_34,(String *)&pGStack_40);
    AbyssEngine::String::~String((String *)&pGStack_40);
    uVar1 = 5;
    break;
  case 6:
    AbyssEngine::String::String((String *)&pGStack_40,(char *)(DAT_0008f61c + 0x8f400),false);
    AbyssEngine::String::operator=(aSStack_34,(String *)&pGStack_40);
    AbyssEngine::String::~String((String *)&pGStack_40);
    uVar1 = 6;
    break;
  case 7:
    AbyssEngine::String::String((String *)&pGStack_40,(char *)(DAT_0008f620 + 0x8f41e),false);
    AbyssEngine::String::operator=(aSStack_34,(String *)&pGStack_40);
    AbyssEngine::String::~String((String *)&pGStack_40);
    uVar1 = 7;
    break;
  case 8:
    AbyssEngine::String::String((String *)&pGStack_40,(char *)(DAT_0008f624 + 0x8f43c),false);
    AbyssEngine::String::operator=(aSStack_34,(String *)&pGStack_40);
    AbyssEngine::String::~String((String *)&pGStack_40);
    uVar1 = 8;
    break;
  case 9:
    AbyssEngine::String::String((String *)&pGStack_40,(char *)(DAT_0008f628 + 0x8f45a),false);
    AbyssEngine::String::operator=(aSStack_34,(String *)&pGStack_40);
    AbyssEngine::String::~String((String *)&pGStack_40);
    uVar1 = 9;
    break;
  case 10:
    AbyssEngine::String::String((String *)&pGStack_40,(char *)(DAT_0008f62c + 0x8f478),false);
    AbyssEngine::String::operator=(aSStack_34,(String *)&pGStack_40);
    goto LAB_0008f52a;
  case 0xb:
    AbyssEngine::String::String((String *)&pGStack_40,(char *)(DAT_0008f630 + 0x8f48e),false);
    AbyssEngine::String::operator=(aSStack_34,(String *)&pGStack_40);
    goto LAB_0008f52a;
  case 0xc:
    AbyssEngine::String::String((String *)&pGStack_40,(char *)(DAT_0008f634 + 0x8f4a4),false);
    AbyssEngine::String::operator=(aSStack_34,(String *)&pGStack_40);
    AbyssEngine::String::~String((String *)&pGStack_40);
    uVar1 = 0xc;
    break;
  case 0xd:
    AbyssEngine::String::String((String *)&pGStack_40,(char *)(DAT_0008f638 + 0x8f4c2),false);
    AbyssEngine::String::operator=(aSStack_34,(String *)&pGStack_40);
    AbyssEngine::String::~String((String *)&pGStack_40);
    uVar1 = 0xd;
    break;
  case 0xe:
    AbyssEngine::String::String((String *)&pGStack_40,(char *)(DAT_0008f63c + 0x8f4e0),false);
    AbyssEngine::String::operator=(aSStack_34,(String *)&pGStack_40);
    AbyssEngine::String::~String((String *)&pGStack_40);
    uVar1 = 0xe;
    break;
  case 0xf:
    AbyssEngine::String::String((String *)&pGStack_40,(char *)(DAT_0008f640 + 0x8f4fe),false);
    AbyssEngine::String::operator=(aSStack_34,(String *)&pGStack_40);
    AbyssEngine::String::~String((String *)&pGStack_40);
    uVar1 = 0xf;
    break;
  default:
    AbyssEngine::String::String((String *)&pGStack_40,(char *)(DAT_0008f648 + 0x8f51c),false);
    AbyssEngine::String::operator=(aSStack_34,(String *)&pGStack_40);
LAB_0008f52a:
    AbyssEngine::String::~String((String *)&pGStack_40);
    uVar1 = 0;
  }
  *puVar7 = uVar1;
  iVar5 = AEFile::FileExist(aSStack_34);
  if (iVar5 == 0) {
    pvVar4 = (void *)AbyssEngine::String::GetAEChar();
    operator_delete__(pvVar4);
    AbyssEngine::String::String((String *)&pGStack_40,(char *)(DAT_0008f644 + 0x8f54e),false);
    AbyssEngine::String::operator=(aSStack_34,(String *)&pGStack_40);
    AbyssEngine::String::~String((String *)&pGStack_40);
    *puVar7 = 0;
  }
  AEFile::OpenRead(aSStack_34,(uint *)&stack0xffffffd8);
  ReadLangFile(this,0,in_r2);
  AbyssEngine::String::~String(aSStack_34);
LAB_0008f580:
  if (*piVar6 != iVar2) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  0008f2c0: push {r4,r5,r6,r7,lr}
  0008f2c2: add r7,sp,#0xc
  0008f2c4: push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r11}
  0008f2c8: mov r5,r0
  0008f2ca: ldr r0,[0x0008f5f8]
  0008f2cc: mov r6,r1
  0008f2ce: ldr r1,[0x0008f5fc]
  0008f2d0: add r0,pc
  0008f2d2: mov r4,r2
  0008f2d4: add r1,pc
  0008f2d6: ldr.w r8,[r0,#0x0]
  0008f2da: ldr.w r9,[r1,#0x0]
  0008f2de: ldr.w r0,[r8,#0x0]
  0008f2e2: ldrh.w r1,[r9,#0x0]
  0008f2e6: str r0,[sp,#0x1c]
  0008f2e8: uxth r0,r6
  0008f2ea: cmp r1,r0
  0008f2ec: beq.w 0x0008f580
  0008f2f0: mov r0,r5
  0008f2f2: blx 0x0006ffe8
  0008f2f6: movs r0,#0x4
  0008f2f8: str r4,[r5,#0x1c]
  0008f2fa: umull r0,r1,r4,r0
  0008f2fe: cmp r1,#0x0
  0008f300: it ne
  0008f302: mov.ne r1,#0x1
  0008f304: cmp r1,#0x0
  0008f306: it ne
  0008f308: mov.ne.w r0,#0xffffffff
  0008f30c: blx 0x0006ec08
  0008f310: str r0,[r5,#0xc]
  0008f312: movs r0,#0x0
  0008f314: movs r1,#0x0
  0008f316: b 0x0008f320
  0008f318: ldr r2,[r5,#0xc]
  0008f31a: str.w r0,[r2,r1,lsl #0x2]
  0008f31e: adds r1,#0x1
  0008f320: cmp r1,r4
  0008f322: blt 0x0008f318
  0008f324: ldr r1,[0x0008f600]
  0008f326: movs r0,#0x0
  0008f328: str r0,[sp,#0x18]
  0008f32a: add r0,sp,#0xc
  0008f32c: add r1,pc
  0008f32e: movs r2,#0x0
  0008f330: blx 0x0006ee18
  0008f334: cmp r6,#0xf
  0008f336: bhi.w 0x0008f516
  0008f33a: tbb [pc,r6]
  0008f34e: ldr r1,[0x0008f604]
  0008f350: add r1,pc
  0008f352: mov r0,sp
  0008f354: movs r2,#0x0
  0008f356: blx 0x0006ee18
  0008f35a: add r0,sp,#0xc
  0008f35c: mov r1,sp
  0008f35e: blx 0x0006f2b0
  0008f362: b 0x0008f52a
  0008f364: ldr r1,[0x0008f608]
  0008f366: add r1,pc
  0008f368: mov r0,sp
  0008f36a: movs r2,#0x0
  0008f36c: blx 0x0006ee18
  0008f370: add r0,sp,#0xc
  0008f372: mov r1,sp
  0008f374: blx 0x0006f2b0
  0008f378: mov r0,sp
  0008f37a: blx 0x0006ee30
  0008f37e: movs r0,#0x1
  0008f380: b 0x0008f532
  0008f382: ldr r1,[0x0008f610]
  0008f384: add r1,pc
  0008f386: mov r0,sp
  0008f388: movs r2,#0x0
  0008f38a: blx 0x0006ee18
  0008f38e: add r0,sp,#0xc
  0008f390: mov r1,sp
  0008f392: blx 0x0006f2b0
  0008f396: mov r0,sp
  0008f398: blx 0x0006ee30
  0008f39c: movs r0,#0x2
  0008f39e: b 0x0008f532
  0008f3a0: ldr r1,[0x0008f60c]
  0008f3a2: add r1,pc
  0008f3a4: mov r0,sp
  0008f3a6: movs r2,#0x0
  0008f3a8: blx 0x0006ee18
  0008f3ac: add r0,sp,#0xc
  0008f3ae: mov r1,sp
  0008f3b0: blx 0x0006f2b0
  0008f3b4: mov r0,sp
  0008f3b6: blx 0x0006ee30
  0008f3ba: movs r0,#0x3
  0008f3bc: b 0x0008f532
  0008f3be: ldr r1,[0x0008f614]
  0008f3c0: add r1,pc
  0008f3c2: mov r0,sp
  0008f3c4: movs r2,#0x0
  0008f3c6: blx 0x0006ee18
  0008f3ca: add r0,sp,#0xc
  0008f3cc: mov r1,sp
  0008f3ce: blx 0x0006f2b0
  0008f3d2: mov r0,sp
  0008f3d4: blx 0x0006ee30
  0008f3d8: movs r0,#0x4
  0008f3da: b 0x0008f532
  0008f3dc: ldr r1,[0x0008f618]
  0008f3de: add r1,pc
  0008f3e0: mov r0,sp
  0008f3e2: movs r2,#0x0
  0008f3e4: blx 0x0006ee18
  0008f3e8: add r0,sp,#0xc
  0008f3ea: mov r1,sp
  0008f3ec: blx 0x0006f2b0
  0008f3f0: mov r0,sp
  0008f3f2: blx 0x0006ee30
  0008f3f6: movs r0,#0x5
  0008f3f8: b 0x0008f532
  0008f3fa: ldr r1,[0x0008f61c]
  0008f3fc: add r1,pc
  0008f3fe: mov r0,sp
  0008f400: movs r2,#0x0
  0008f402: blx 0x0006ee18
  0008f406: add r0,sp,#0xc
  0008f408: mov r1,sp
  0008f40a: blx 0x0006f2b0
  0008f40e: mov r0,sp
  0008f410: blx 0x0006ee30
  0008f414: movs r0,#0x6
  0008f416: b 0x0008f532
  0008f418: ldr r1,[0x0008f620]
  0008f41a: add r1,pc
  0008f41c: mov r0,sp
  0008f41e: movs r2,#0x0
  0008f420: blx 0x0006ee18
  0008f424: add r0,sp,#0xc
  0008f426: mov r1,sp
  0008f428: blx 0x0006f2b0
  0008f42c: mov r0,sp
  0008f42e: blx 0x0006ee30
  0008f432: movs r0,#0x7
  0008f434: b 0x0008f532
  0008f436: ldr r1,[0x0008f624]
  0008f438: add r1,pc
  0008f43a: mov r0,sp
  0008f43c: movs r2,#0x0
  0008f43e: blx 0x0006ee18
  0008f442: add r0,sp,#0xc
  0008f444: mov r1,sp
  0008f446: blx 0x0006f2b0
  0008f44a: mov r0,sp
  0008f44c: blx 0x0006ee30
  0008f450: movs r0,#0x8
  0008f452: b 0x0008f532
  0008f454: ldr r1,[0x0008f628]
  0008f456: add r1,pc
  0008f458: mov r0,sp
  0008f45a: movs r2,#0x0
  0008f45c: blx 0x0006ee18
  0008f460: add r0,sp,#0xc
  0008f462: mov r1,sp
  0008f464: blx 0x0006f2b0
  0008f468: mov r0,sp
  0008f46a: blx 0x0006ee30
  0008f46e: movs r0,#0x9
  0008f470: b 0x0008f532
  0008f472: ldr r1,[0x0008f62c]
  0008f474: add r1,pc
  0008f476: mov r0,sp
  0008f478: movs r2,#0x0
  0008f47a: blx 0x0006ee18
  0008f47e: add r0,sp,#0xc
  0008f480: mov r1,sp
  0008f482: blx 0x0006f2b0
  0008f486: b 0x0008f52a
  0008f488: ldr r1,[0x0008f630]
  0008f48a: add r1,pc
  0008f48c: mov r0,sp
  0008f48e: movs r2,#0x0
  0008f490: blx 0x0006ee18
  0008f494: add r0,sp,#0xc
  0008f496: mov r1,sp
  0008f498: blx 0x0006f2b0
  0008f49c: b 0x0008f52a
  0008f49e: ldr r1,[0x0008f634]
  0008f4a0: add r1,pc
  0008f4a2: mov r0,sp
  0008f4a4: movs r2,#0x0
  0008f4a6: blx 0x0006ee18
  0008f4aa: add r0,sp,#0xc
  0008f4ac: mov r1,sp
  0008f4ae: blx 0x0006f2b0
  0008f4b2: mov r0,sp
  0008f4b4: blx 0x0006ee30
  0008f4b8: movs r0,#0xc
  0008f4ba: b 0x0008f532
  0008f4bc: ldr r1,[0x0008f638]
  0008f4be: add r1,pc
  0008f4c0: mov r0,sp
  0008f4c2: movs r2,#0x0
  0008f4c4: blx 0x0006ee18
  0008f4c8: add r0,sp,#0xc
  0008f4ca: mov r1,sp
  0008f4cc: blx 0x0006f2b0
  0008f4d0: mov r0,sp
  0008f4d2: blx 0x0006ee30
  0008f4d6: movs r0,#0xd
  0008f4d8: b 0x0008f532
  0008f4da: ldr r1,[0x0008f63c]
  0008f4dc: add r1,pc
  0008f4de: mov r0,sp
  0008f4e0: movs r2,#0x0
  0008f4e2: blx 0x0006ee18
  0008f4e6: add r0,sp,#0xc
  0008f4e8: mov r1,sp
  0008f4ea: blx 0x0006f2b0
  0008f4ee: mov r0,sp
  0008f4f0: blx 0x0006ee30
  0008f4f4: movs r0,#0xe
  0008f4f6: b 0x0008f532
  0008f4f8: ldr r1,[0x0008f640]
  0008f4fa: add r1,pc
  0008f4fc: mov r0,sp
  0008f4fe: movs r2,#0x0
  0008f500: blx 0x0006ee18
  0008f504: add r0,sp,#0xc
  0008f506: mov r1,sp
  0008f508: blx 0x0006f2b0
  0008f50c: mov r0,sp
  0008f50e: blx 0x0006ee30
  0008f512: movs r0,#0xf
  0008f514: b 0x0008f532
  0008f516: ldr r1,[0x0008f648]
  0008f518: add r1,pc
  0008f51a: mov r0,sp
  0008f51c: movs r2,#0x0
  0008f51e: blx 0x0006ee18
  0008f522: add r0,sp,#0xc
  0008f524: mov r1,sp
  0008f526: blx 0x0006f2b0
  0008f52a: mov r0,sp
  0008f52c: blx 0x0006ee30
  0008f530: movs r0,#0x0
  0008f532: strh.w r0,[r9,#0x0]
  0008f536: add r0,sp,#0xc
  0008f538: blx 0x0006fff4
  0008f53c: cbnz r0,0x0008f568
  0008f53e: add r0,sp,#0xc
  0008f540: blx 0x0006ef68
  0008f544: blx 0x0006ebfc
  0008f548: ldr r1,[0x0008f644]
  0008f54a: add r1,pc
  0008f54c: mov r0,sp
  0008f54e: movs r2,#0x0
  0008f550: blx 0x0006ee18
  0008f554: add r0,sp,#0xc
  0008f556: mov r1,sp
  0008f558: blx 0x0006f2b0
  0008f55c: mov r0,sp
  0008f55e: blx 0x0006ee30
  0008f562: movs r0,#0x0
  0008f564: strh.w r0,[r9,#0x0]
  0008f568: add r0,sp,#0xc
  0008f56a: add r1,sp,#0x18
  0008f56c: blx 0x0006f34c
  0008f570: ldr r1,[sp,#0x18]
  0008f572: mov r0,r5
  0008f574: mov r2,r4
  0008f576: blx 0x00070000
  0008f57a: add r0,sp,#0xc
  0008f57c: blx 0x0006ee30
  0008f580: ldr r0,[sp,#0x1c]
  0008f582: ldr.w r1,[r8,#0x0]
  0008f586: subs r0,r1,r0
  0008f588: ittt eq
  0008f58a: add.eq sp,#0x20
  0008f58c: pop.eq.w {r8,r9,r11}
  0008f590: pop.eq {r4,r5,r6,r7,pc}
  0008f592: blx 0x0006e824
```
