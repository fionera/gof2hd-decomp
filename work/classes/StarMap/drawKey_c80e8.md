# StarMap::drawKey
elf 0xc80e8 body 560
Sig: undefined __thiscall drawKey(StarMap * this)

## decompile
```c

/* StarMap::drawKey() */

void __thiscall StarMap::drawKey(StarMap *this)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  undefined4 uVar5;
  undefined4 *puVar6;
  uint *puVar7;
  int *piVar8;
  code *pcVar9;
  int iVar10;
  code *pcVar11;
  code *pcVar12;
  int iVar13;
  int iVar14;
  uint uVar15;
  int iVar16;
  int iVar17;
  undefined4 *puVar18;
  int iVar19;
  undefined4 uVar20;
  int iVar21;
  String aSStack_34 [12];
  int local_28;
  
  piVar1 = *(int **)(DAT_000d8328 + 0xd80fc);
  puVar7 = *(uint **)(DAT_000d832c + 0xd80fe);
  local_28 = *piVar1;
  iVar2 = AbyssEngine::PaintCanvas::GetImage2DWidth(*puVar7);
  piVar8 = *(int **)(DAT_000d8330 + 0xd811a);
  iVar13 = *piVar8;
  iVar3 = **(int **)(DAT_000d8334 + 0xd811e);
  iVar4 = *(int *)(iVar13 + 4);
  iVar19 = *(int *)(this + 0x10c);
  iVar21 = *(int *)(this + 0x110);
  iVar14 = **(int **)(DAT_000d8338 + 0xd8124);
  iVar16 = *(int *)(iVar13 + 8);
  iVar10 = *(int *)(iVar13 + 0x10);
  iVar17 = *(int *)(iVar13 + 0x2c);
  uVar5 = AbyssEngine::String::String(aSStack_34,(char *)(DAT_000d833c + 0xd8150),false);
  iVar14 = iVar14 - iVar19;
  Layout::drawBox(iVar13,7,iVar14,((iVar3 - iVar10) - iVar21) - iVar16,iVar19,iVar16 + iVar21,uVar5)
  ;
  iVar14 = iVar14 + iVar17;
  iVar2 = iVar2 + iVar17 + iVar14;
  iVar4 = ((iVar3 - iVar17) - iVar10) - iVar4;
  AbyssEngine::String::~String(aSStack_34);
  pcVar11 = *(code **)(DAT_000d8340 + 0xd81a8);
  (*pcVar11)(*puVar7,*(undefined4 *)(this + 0x20),iVar14,iVar4);
  puVar18 = *(undefined4 **)(DAT_000d8344 + 0xd81bc);
  pcVar9 = *(code **)(DAT_000d8348 + 0xd81be);
  uVar15 = *puVar7;
  puVar6 = *(undefined4 **)(DAT_000d834c + 0xd81d2);
  uVar20 = *puVar6;
  uVar5 = (*pcVar9)(*puVar18,0x112);
  pcVar12 = *(code **)(DAT_000d8350 + 0xd81f4);
  (*pcVar12)(uVar15,uVar20,uVar5,iVar2,iVar4,0);
  iVar4 = iVar4 - *(int *)(*piVar8 + 4);
  (*pcVar11)(*puVar7,*(undefined4 *)(this + 0x30),iVar14,iVar4);
  uVar20 = *puVar6;
  uVar15 = *puVar7;
  uVar5 = (*pcVar9)(*puVar18,0x191);
  (*pcVar12)(uVar15,uVar20,uVar5,iVar2,iVar4,0);
  iVar4 = iVar4 - *(int *)(*piVar8 + 4);
  (*pcVar11)(*puVar7,*(undefined4 *)(this + 0x2c),iVar14,iVar4);
  uVar15 = *puVar7;
  uVar20 = *puVar6;
  uVar5 = (*pcVar9)(*puVar18,0x223);
  (*pcVar12)(uVar15,uVar20,uVar5,iVar2,iVar4,0);
  iVar4 = iVar4 - *(int *)(*piVar8 + 4);
  (*pcVar11)(*puVar7,*(undefined4 *)(this + 0x28),iVar14,iVar4);
  uVar5 = *puVar6;
  uVar15 = *puVar7;
  uVar20 = (*pcVar9)(*puVar18,0x22c);
  (*pcVar12)(uVar15,uVar5,uVar20,iVar2,iVar4,0);
  iVar4 = iVar4 - *(int *)(*piVar8 + 4);
  (*pcVar11)(*puVar7,*(undefined4 *)(this + 0x24),iVar14,iVar4);
  uVar15 = *puVar7;
  uVar20 = *puVar6;
  uVar5 = (*pcVar9)(*puVar18,0x22b);
  (*pcVar12)(uVar15,uVar20,uVar5,iVar2,iVar4,0);
  if (*piVar1 != local_28) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000d80e8: push {r4,r5,r6,r7,lr}
  000d80ea: add r7,sp,#0xc
  000d80ec: push {r8,r9,r10,r11}
  000d80f0: sub sp,#0x4c
  000d80f2: mov r4,r0
  000d80f4: ldr r0,[0x000d8328]
  000d80f6: ldr r1,[0x000d832c]
  000d80f8: add r0,pc
  000d80fa: add r1,pc
  000d80fc: ldr r0,[r0,#0x0]
  000d80fe: ldr r2,[r1,#0x0]
  000d8100: str r0,[sp,#0x18]
  000d8102: ldr r1,[r0,#0x0]
  000d8104: str r2,[sp,#0x14]
  000d8106: ldr r0,[r2,#0x0]
  000d8108: str r1,[sp,#0x48]
  000d810a: ldr r1,[r4,#0x30]
  000d810c: blx 0x00072d84
  000d8110: str r0,[sp,#0x2c]
  000d8112: ldr r0,[0x000d8330]
  000d8114: ldr r1,[0x000d8334]
  000d8116: add r0,pc
  000d8118: ldr r2,[0x000d8338]
  000d811a: add r1,pc
  000d811c: str r4,[sp,#0x38]
  000d811e: ldr r3,[r0,#0x0]
  000d8120: add r2,pc
  000d8122: ldr r0,[r1,#0x0]
  000d8124: ldr r1,[r2,#0x0]
  000d8126: movs r2,#0x0
  000d8128: ldr r5,[r3,#0x0]
  000d812a: ldr r0,[r0,#0x0]
  000d812c: str r0,[sp,#0x34]
  000d812e: ldr r0,[r5,#0x4]
  000d8130: str r0,[sp,#0x24]
  000d8132: add r0,sp,#0x3c
  000d8134: str r3,[sp,#0x30]
  000d8136: ldr.w r10,[r4,#0x10c]
  000d813a: ldr.w r11,[r4,#0x110]
  000d813e: ldr r6,[r1,#0x0]
  000d8140: ldr.w r8,[r5,#0x8]
  000d8144: ldr r4,[r5,#0x10]
  000d8146: ldr.w r9,[r5,#0x2c]
  000d814a: ldr r1,[0x000d833c]
  000d814c: add r1,pc
  000d814e: blx 0x0006ee18
  000d8152: sub.w r6,r6,r10
  000d8156: str.w r10,[sp,#0x0]
  000d815a: mov r10,r4
  000d815c: ldr r4,[sp,#0x34]
  000d815e: add.w r1,r8,r11
  000d8162: strd r1,r0,[sp,#0x4]
  000d8166: movs r1,#0x7
  000d8168: sub.w r0,r4,r10
  000d816c: mov r2,r6
  000d816e: sub.w r0,r0,r11
  000d8172: sub.w r3,r0,r8
  000d8176: mov r0,r5
  000d8178: blx 0x0007462c
  000d817c: ldr r0,[sp,#0x2c]
  000d817e: add r6,r9
  000d8180: ldr r1,[sp,#0x24]
  000d8182: add r0,r9
  000d8184: str r6,[sp,#0x28]
  000d8186: add.w r8,r0,r6
  000d818a: sub.w r0,r4,r9
  000d818e: sub.w r0,r0,r10
  000d8192: subs r5,r0,r1
  000d8194: add r0,sp,#0x3c
  000d8196: blx 0x0006ee30
  000d819a: ldr r2,[0x000d8340]
  000d819c: mov r3,r5
  000d819e: ldr r0,[sp,#0x38]
  000d81a0: ldr.w r11,[sp,#0x14]
  000d81a4: add r2,pc
  000d81a6: ldr r4,[r2,#0x0]
  000d81a8: mov r2,r6
  000d81aa: ldr r1,[r0,#0x20]
  000d81ac: ldr.w r0,[r11,#0x0]
  000d81b0: str r4,[sp,#0x1c]
  000d81b2: blx r4
  000d81b4: ldr r0,[0x000d8344]
  000d81b6: ldr r1,[0x000d8348]
  000d81b8: add r0,pc
  000d81ba: add r1,pc
  000d81bc: ldr.w r9,[r0,#0x0]
  000d81c0: ldr r3,[r1,#0x0]
  000d81c2: mov.w r1,#0x112
  000d81c6: str r3,[sp,#0x2c]
  000d81c8: ldr r0,[0x000d834c]
  000d81ca: ldr.w r6,[r11,#0x0]
  000d81ce: add r0,pc
  000d81d0: str.w r9,[sp,#0x24]
  000d81d4: ldr r0,[r0,#0x0]
  000d81d6: str r0,[sp,#0x34]
  000d81d8: ldr.w r10,[r0,#0x0]
  000d81dc: ldr.w r0,[r9,#0x0]
  000d81e0: blx r3
  000d81e2: mov r2,r0
  000d81e4: movs r0,#0x0
  000d81e6: str r5,[sp,#0x0]
  000d81e8: mov r1,r10
  000d81ea: str r0,[sp,#0x4]
  000d81ec: mov r3,r8
  000d81ee: ldr r0,[0x000d8350]
  000d81f0: add r0,pc
  000d81f2: ldr r4,[r0,#0x0]
  000d81f4: mov r0,r6
  000d81f6: blx r4
  000d81f8: ldr r0,[sp,#0x30]
  000d81fa: mov r6,r4
  000d81fc: ldr r2,[sp,#0x28]
  000d81fe: ldr.w r10,[sp,#0x1c]
  000d8202: ldr r0,[r0,#0x0]
  000d8204: str r4,[sp,#0x20]
  000d8206: ldr r0,[r0,#0x4]
  000d8208: subs r5,r5,r0
  000d820a: ldr r0,[sp,#0x38]
  000d820c: mov r3,r5
  000d820e: ldr r1,[r0,#0x30]
  000d8210: ldr.w r0,[r11,#0x0]
  000d8214: blx r10
  000d8216: ldr r1,[sp,#0x34]
  000d8218: ldr.w r0,[r9,#0x0]
  000d821c: ldr.w r9,[sp,#0x2c]
  000d8220: ldr r1,[r1,#0x0]
  000d8222: str r1,[sp,#0xc]
  000d8224: movw r1,#0x191
  000d8228: ldr.w r4,[r11,#0x0]
  000d822c: blx r9
  000d822e: ldr r1,[sp,#0xc]
  000d8230: mov r2,r0
  000d8232: movs r0,#0x0
  000d8234: mov r3,r8
  000d8236: str r0,[sp,#0x4]
  000d8238: mov r0,r4
  000d823a: str r5,[sp,#0x0]
  000d823c: mov r4,r8
  000d823e: blx r6
  000d8240: ldr r6,[sp,#0x30]
  000d8242: ldr r2,[sp,#0x28]
  000d8244: ldr r0,[r6,#0x0]
  000d8246: ldr r0,[r0,#0x4]
  000d8248: subs r5,r5,r0
  000d824a: ldr r0,[sp,#0x38]
  000d824c: mov r3,r5
  000d824e: ldr r1,[r0,#0x2c]
  000d8250: ldr.w r0,[r11,#0x0]
  000d8254: blx r10
  000d8256: ldr r0,[sp,#0x34]
  000d8258: movw r1,#0x223
  000d825c: ldr.w r10,[r11,#0x0]
  000d8260: ldr.w r8,[r0,#0x0]
  000d8264: ldr r0,[sp,#0x24]
  000d8266: ldr r0,[r0,#0x0]
  000d8268: blx r9
  000d826a: mov r1,r8
  000d826c: mov r8,r4
  000d826e: str r4,[sp,#0x10]
  000d8270: mov r3,r4
  000d8272: ldr r4,[sp,#0x20]
  000d8274: mov r2,r0
  000d8276: movs r0,#0x0
  000d8278: str r5,[sp,#0x0]
  000d827a: str r0,[sp,#0x4]
  000d827c: mov r0,r10
  000d827e: blx r4
  000d8280: ldr r0,[r6,#0x0]
  000d8282: ldr.w r10,[sp,#0x28]
  000d8286: ldr r6,[sp,#0x1c]
  000d8288: ldr r0,[r0,#0x4]
  000d828a: mov r2,r10
  000d828c: subs r5,r5,r0
  000d828e: ldr r0,[sp,#0x38]
  000d8290: mov r3,r5
  000d8292: ldr r1,[r0,#0x28]
  000d8294: ldr.w r0,[r11,#0x0]
  000d8298: blx r6
  000d829a: ldr r0,[sp,#0x34]
  000d829c: mov.w r1,#0x22c
  000d82a0: ldr.w r9,[sp,#0x24]
  000d82a4: ldr r2,[sp,#0x2c]
  000d82a6: ldr r0,[r0,#0x0]
  000d82a8: str r0,[sp,#0xc]
  000d82aa: ldr.w r0,[r9,#0x0]
  000d82ae: ldr.w r4,[r11,#0x0]
  000d82b2: blx r2
  000d82b4: mov r3,r8
  000d82b6: ldr r1,[sp,#0xc]
  000d82b8: ldr.w r8,[sp,#0x20]
  000d82bc: mov r2,r0
  000d82be: movs r0,#0x0
  000d82c0: str r5,[sp,#0x0]
  000d82c2: str r0,[sp,#0x4]
  000d82c4: mov r0,r4
  000d82c6: blx r8
  000d82c8: ldr r0,[sp,#0x30]
  000d82ca: mov r2,r10
  000d82cc: ldr r0,[r0,#0x0]
  000d82ce: ldr r0,[r0,#0x4]
  000d82d0: subs r4,r5,r0
  000d82d2: ldr r0,[sp,#0x38]
  000d82d4: mov r3,r4
  000d82d6: ldr r1,[r0,#0x24]
  000d82d8: ldr.w r0,[r11,#0x0]
  000d82dc: blx r6
  000d82de: ldr r0,[sp,#0x34]
  000d82e0: movw r1,#0x22b
  000d82e4: ldr r2,[sp,#0x2c]
  000d82e6: ldr.w r6,[r11,#0x0]
  000d82ea: ldr r5,[r0,#0x0]
  000d82ec: ldr.w r0,[r9,#0x0]
  000d82f0: blx r2
  000d82f2: ldr r3,[sp,#0x10]
  000d82f4: mov r2,r0
  000d82f6: movs r0,#0x0
  000d82f8: mov r1,r5
  000d82fa: str r0,[sp,#0x4]
  000d82fc: mov r0,r6
  000d82fe: str r4,[sp,#0x0]
  000d8300: blx r8
  000d8302: ldr r0,[sp,#0x48]
  000d8304: ldr r1,[sp,#0x18]
  000d8306: ldr r1,[r1,#0x0]
  000d8308: subs r0,r1,r0
  000d830a: ittt eq
  000d830c: add.eq sp,#0x4c
  000d830e: pop.eq.w {r8,r9,r10,r11}
  000d8312: pop.eq {r4,r5,r6,r7,pc}
  000d8314: blx 0x0006e824
```
