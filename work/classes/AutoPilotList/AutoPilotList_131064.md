# AutoPilotList::AutoPilotList
elf 0x131064 body 604
Sig: undefined __thiscall AutoPilotList(AutoPilotList * this, Level * param_1)

## decompile
```c

/* AutoPilotList::AutoPilotList(Level*) */

void __thiscall AutoPilotList::AutoPilotList(AutoPilotList *this,Level *param_1)

{
  Array *pAVar1;
  String *pSVar2;
  String *pSVar3;
  int iVar4;
  String *pSVar5;
  PlayerEgo *pPVar6;
  Route *this_00;
  uint *puVar7;
  int iVar8;
  code *pcVar9;
  undefined4 *puVar10;
  uint uVar11;
  int *piVar12;
  uint *puVar13;
  int *piVar14;
  String aSStack_48 [12];
  String aSStack_3c [12];
  String aSStack_30 [12];
  int local_24;
  
  piVar14 = *(int **)(DAT_00141320 + 0x141078);
  local_24 = *piVar14;
  pAVar1 = operator_new(0xc);
  Array<AbyssEngine::String*>::Array();
  *(Array **)(this + 0x10) = pAVar1;
  ArraySetLength<AbyssEngine::String*>(5,pAVar1);
  piVar12 = *(int **)(DAT_00141324 + 0x14109c);
  *(undefined4 *)(this + 0x14) = 0;
  if (*piVar12 != 0) {
    pSVar2 = operator_new(0xc);
    pSVar3 = (String *)GameText::getText(**(int **)(DAT_00141328 + 0x1410b2));
    AbyssEngine::String::String(aSStack_3c,(char *)(DAT_0014132c + 0x1410c4),false);
    AbyssEngine::operator+(aSStack_30,pSVar3);
    Station::getName();
    AbyssEngine::operator+(pSVar2,aSStack_30);
    pcVar9 = *(code **)(DAT_00141330 + 0x1410ee);
    **(undefined4 **)(*(int *)(this + 0x10) + 4) = pSVar2;
    (*pcVar9)(aSStack_48);
    (*pcVar9)(aSStack_30);
    (*pcVar9)(aSStack_3c);
    *(int *)(this + 0x14) = *(int *)(this + 0x14) + 1;
  }
  puVar10 = *(undefined4 **)(DAT_00141334 + 0x14110c);
  Status::getSystem();
  iVar4 = SolarSystem::currentOrbitHasWarpGate();
  if (iVar4 != 0) {
    pSVar5 = operator_new(0xc);
    pSVar2 = (String *)GameText::getText(**(int **)(DAT_00141338 + 0x141126));
    AbyssEngine::String::String(pSVar5,pSVar2,false);
    *(String **)(*(int *)(*(int *)(this + 0x10) + 4) + 4) = pSVar5;
    *(int *)(this + 0x14) = *(int *)(this + 0x14) + 1;
  }
  iVar4 = Status::inEmptyOrbit((Status *)*puVar10);
  if (iVar4 == 0) {
    pSVar2 = operator_new(0xc);
    Status::getStation();
    Station::getName();
    AbyssEngine::String::String(aSStack_48,(char *)(DAT_0014133c + 0x14116a),false);
    AbyssEngine::operator+(aSStack_30,aSStack_3c);
    GameText::getText(**(int **)(DAT_00141340 + 0x141180));
    AbyssEngine::operator+(pSVar2,aSStack_30);
    pcVar9 = *(code **)(DAT_00141344 + 0x14119c);
    *(String **)(*(int *)(*(int *)(this + 0x10) + 4) + 8) = pSVar2;
    (*pcVar9)(aSStack_30);
    (*pcVar9)(aSStack_48);
    (*pcVar9)(aSStack_3c);
    *(int *)(this + 0x14) = *(int *)(this + 0x14) + 1;
  }
  pSVar5 = operator_new(0xc);
  piVar12 = *(int **)(DAT_00141348 + 0x1411be);
  pSVar2 = (String *)GameText::getText(*piVar12);
  AbyssEngine::String::String(pSVar5,pSVar2,false);
  *(String **)(*(int *)(*(int *)(this + 0x10) + 4) + 0xc) = pSVar5;
  *(int *)(this + 0x14) = *(int *)(this + 0x14) + 1;
  pPVar6 = (PlayerEgo *)Level::getPlayer();
  iVar4 = PlayerEgo::getRoute(pPVar6);
  if (iVar4 != 0) {
    pPVar6 = (PlayerEgo *)Level::getPlayer();
    this_00 = (Route *)PlayerEgo::getRoute(pPVar6);
    iVar4 = Route::getLastWaypoint(this_00);
    if (*(char *)(iVar4 + 0x130) == '\0') {
      pSVar5 = operator_new(0xc);
      pSVar2 = (String *)GameText::getText(*piVar12);
      AbyssEngine::String::String(pSVar5,pSVar2,false);
      *(String **)(*(int *)(*(int *)(this + 0x10) + 4) + 0x10) = pSVar5;
      *(int *)(this + 0x14) = *(int *)(this + 0x14) + 1;
    }
  }
  iVar4 = 0;
  *(undefined4 *)(this + 0xc) = 0;
  *(undefined4 *)this = 0;
  puVar10 = *(undefined4 **)(DAT_0014134c + 0x141236);
  puVar13 = *(uint **)(DAT_00141350 + 0x141238);
  for (uVar11 = 0; puVar7 = *(uint **)(this + 0x10), uVar11 < *puVar7; uVar11 = uVar11 + 1) {
    if (*(int *)(puVar7[1] + uVar11 * 4) != 0) {
      iVar8 = AbyssEngine::PaintCanvas::GetTextWidth(*puVar13,(String *)*puVar10);
      iVar8 = iVar8 + 0x13;
      iVar4 = *(int *)(this + 0xc);
      if (*(int *)(this + 0xc) < iVar8) {
        *(int *)(this + 0xc) = iVar8;
        iVar4 = iVar8;
      }
    }
  }
  iVar8 = **(int **)(DAT_00141358 + 0x141272);
  *(int *)(this + 4) = (**(int **)(DAT_00141354 + 0x14126e) - iVar4) / 2;
  *(int *)(this + 8) = (iVar8 + *(int *)(this + 0x14) * -0xf + -0xc) / 2;
  while (*(int *)(puVar7[1] + *(int *)this * 4) == 0) {
    down(this);
    puVar7 = *(uint **)(this + 0x10);
  }
  iVar4 = *piVar14 - local_24;
  if (iVar4 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar4);
  }
  return;
}

```

## target disasm
```
  00141064: push {r4,r5,r6,r7,lr}
  00141066: add r7,sp,#0xc
  00141068: push {r8,r9,r10}
  0014106c: sub sp,#0x28
  0014106e: mov r4,r0
  00141070: ldr r0,[0x00141320]
  00141072: mov r8,r1
  00141074: add r0,pc
  00141076: ldr.w r9,[r0,#0x0]
  0014107a: ldr.w r0,[r9,#0x0]
  0014107e: str r0,[sp,#0x24]
  00141080: movs r0,#0xc
  00141082: blx 0x0006eb24
  00141086: mov r5,r0
  00141088: blx 0x0006f628
  0014108c: movs r0,#0x5
  0014108e: mov r1,r5
  00141090: str r5,[r4,#0x10]
  00141092: blx 0x0006fe08
  00141096: ldr r0,[0x00141324]
  00141098: add r0,pc
  0014109a: ldr r6,[r0,#0x0]
  0014109c: movs r0,#0x0
  0014109e: str r0,[r4,#0x14]
  001410a0: ldr r0,[r6,#0x0]
  001410a2: cbz r0,0x00141106
  001410a4: movs r0,#0xc
  001410a6: blx 0x0006eb24
  001410aa: mov r10,r0
  001410ac: ldr r0,[0x00141328]
  001410ae: add r0,pc
  001410b0: ldr r0,[r0,#0x0]
  001410b2: ldr r0,[r0,#0x0]
  001410b4: movw r1,#0x222
  001410b8: blx 0x00072f70
  001410bc: ldr r1,[0x0014132c]
  001410be: mov r5,r0
  001410c0: add r1,pc
  001410c2: add r0,sp,#0xc
  001410c4: movs r2,#0x0
  001410c6: blx 0x0006ee18
  001410ca: add r0,sp,#0x18
  001410cc: add r2,sp,#0xc
  001410ce: mov r1,r5
  001410d0: blx 0x0006ef98
  001410d4: ldr r1,[r6,#0x0]
  001410d6: mov r0,sp
  001410d8: blx 0x000736a8
  001410dc: add r1,sp,#0x18
  001410de: mov r2,sp
  001410e0: mov r0,r10
  001410e2: blx 0x0006ef98
  001410e6: ldr r0,[r4,#0x10]
  001410e8: ldr r1,[0x00141330]
  001410ea: add r1,pc
  001410ec: ldr r0,[r0,#0x4]
  001410ee: ldr r5,[r1,#0x0]
  001410f0: str.w r10,[r0,#0x0]
  001410f4: mov r0,sp
  001410f6: blx r5
  001410f8: add r0,sp,#0x18
  001410fa: blx r5
  001410fc: add r0,sp,#0xc
  001410fe: blx r5
  00141100: ldr r0,[r4,#0x14]
  00141102: adds r0,#0x1
  00141104: str r0,[r4,#0x14]
  00141106: ldr r0,[0x00141334]
  00141108: add r0,pc
  0014110a: ldr r5,[r0,#0x0]
  0014110c: ldr r0,[r5,#0x0]
  0014110e: blx 0x00071a10
  00141112: blx 0x00073c18
  00141116: cbz r0,0x00141146
  00141118: movs r0,#0xc
  0014111a: blx 0x0006eb24
  0014111e: mov r6,r0
  00141120: ldr r0,[0x00141338]
  00141122: add r0,pc
  00141124: ldr r0,[r0,#0x0]
  00141126: ldr r0,[r0,#0x0]
  00141128: movw r1,#0x223
  0014112c: blx 0x00072f70
  00141130: mov r1,r0
  00141132: mov r0,r6
  00141134: movs r2,#0x0
  00141136: blx 0x0006f028
  0014113a: ldr r0,[r4,#0x10]
  0014113c: ldr r0,[r0,#0x4]
  0014113e: str r6,[r0,#0x4]
  00141140: ldr r0,[r4,#0x14]
  00141142: adds r0,#0x1
  00141144: str r0,[r4,#0x14]
  00141146: ldr r0,[r5,#0x0]
  00141148: blx 0x00073d20
  0014114c: cbnz r0,0x001411b0
  0014114e: movs r0,#0xc
  00141150: blx 0x0006eb24
  00141154: mov r6,r0
  00141156: ldr r0,[r5,#0x0]
  00141158: blx 0x00071c14
  0014115c: mov r1,r0
  0014115e: add r0,sp,#0xc
  00141160: blx 0x000736a8
  00141164: ldr r1,[0x0014133c]
  00141166: add r1,pc
  00141168: mov r0,sp
  0014116a: movs r2,#0x0
  0014116c: blx 0x0006ee18
  00141170: add r0,sp,#0x18
  00141172: add r1,sp,#0xc
  00141174: mov r2,sp
  00141176: blx 0x0006ef98
  0014117a: ldr r0,[0x00141340]
  0014117c: add r0,pc
  0014117e: ldr r0,[r0,#0x0]
  00141180: ldr r0,[r0,#0x0]
  00141182: movs r1,#0x88
  00141184: blx 0x00072f70
  00141188: mov r2,r0
  0014118a: add r1,sp,#0x18
  0014118c: mov r0,r6
  0014118e: blx 0x0006ef98
  00141192: ldr r0,[r4,#0x10]
  00141194: ldr r1,[0x00141344]
  00141196: ldr r0,[r0,#0x4]
  00141198: add r1,pc
  0014119a: ldr r5,[r1,#0x0]
  0014119c: str r6,[r0,#0x8]
  0014119e: add r0,sp,#0x18
  001411a0: blx r5
  001411a2: mov r0,sp
  001411a4: blx r5
  001411a6: add r0,sp,#0xc
  001411a8: blx r5
  001411aa: ldr r0,[r4,#0x14]
  001411ac: adds r0,#0x1
  001411ae: str r0,[r4,#0x14]
  001411b0: movs r0,#0xc
  001411b2: blx 0x0006eb24
  001411b6: mov r6,r0
  001411b8: ldr r0,[0x00141348]
  001411ba: add r0,pc
  001411bc: ldr r5,[r0,#0x0]
  001411be: ldr r0,[r5,#0x0]
  001411c0: movw r1,#0x225
  001411c4: blx 0x00072f70
  001411c8: mov r1,r0
  001411ca: mov r0,r6
  001411cc: movs r2,#0x0
  001411ce: blx 0x0006f028
  001411d2: ldr r0,[r4,#0x10]
  001411d4: ldr r0,[r0,#0x4]
  001411d6: str r6,[r0,#0xc]
  001411d8: ldr r0,[r4,#0x14]
  001411da: adds r0,#0x1
  001411dc: str r0,[r4,#0x14]
  001411de: mov r0,r8
  001411e0: blx 0x00072034
  001411e4: blx 0x000768b8
  001411e8: cbz r0,0x00141226
  001411ea: mov r0,r8
  001411ec: blx 0x00072034
  001411f0: blx 0x000768b8
  001411f4: blx 0x00071ee4
  001411f8: ldrb.w r0,[r0,#0x130]
  001411fc: cbnz r0,0x00141226
  001411fe: movs r0,#0xc
  00141200: blx 0x0006eb24
  00141204: mov r6,r0
  00141206: ldr r0,[r5,#0x0]
  00141208: movw r1,#0x23d
  0014120c: blx 0x00072f70
  00141210: mov r1,r0
  00141212: mov r0,r6
  00141214: movs r2,#0x0
  00141216: blx 0x0006f028
  0014121a: ldr r0,[r4,#0x10]
  0014121c: ldr r0,[r0,#0x4]
  0014121e: str r6,[r0,#0x10]
  00141220: ldr r0,[r4,#0x14]
  00141222: adds r0,#0x1
  00141224: str r0,[r4,#0x14]
  00141226: movs r1,#0x0
  00141228: movs r5,#0x0
  0014122a: str r1,[r4,#0xc]
  0014122c: str r1,[r4,#0x0]
  0014122e: ldr r0,[0x0014134c]
  00141230: ldr r2,[0x00141350]
  00141232: add r0,pc
  00141234: add r2,pc
  00141236: ldr.w r8,[r0,#0x0]
  0014123a: ldr r6,[r2,#0x0]
  0014123c: b 0x0014125e
  0014123e: ldr r0,[r0,#0x4]
  00141240: ldr.w r2,[r0,r5,lsl #0x2]
  00141244: cbz r2,0x0014125c
  00141246: ldr.w r1,[r8,#0x0]
  0014124a: ldr r0,[r6,#0x0]
  0014124c: blx 0x0006faa8
  00141250: ldr r1,[r4,#0xc]
  00141252: adds r0,#0x13
  00141254: cmp r0,r1
  00141256: itt gt
  00141258: str.gt r0,[r4,#0xc]
  0014125a: mov.gt r1,r0
  0014125c: adds r5,#0x1
  0014125e: ldr r0,[r4,#0x10]
  00141260: ldr r2,[r0,#0x0]
  00141262: cmp r5,r2
  00141264: bcc 0x0014123e
  00141266: ldr r2,[0x00141354]
  00141268: ldr r3,[0x00141358]
  0014126a: add r2,pc
  0014126c: ldr r6,[r4,#0x14]
  0014126e: add r3,pc
  00141270: ldr r2,[r2,#0x0]
  00141272: ldr r3,[r3,#0x0]
  00141274: ldr r2,[r2,#0x0]
  00141276: subs r1,r2,r1
  00141278: ldr r2,[r3,#0x0]
  0014127a: sub.w r3,r6,r6, lsl #0x4
  0014127e: add r2,r3
  00141280: add.w r1,r1,r1, lsr #0x1f
  00141284: subs r2,#0xc
  00141286: asrs r1,r1,#0x1
  00141288: add.w r2,r2,r2, lsr #0x1f
  0014128c: asrs r2,r2,#0x1
  0014128e: strd r1,r2,[r4,#0x4]
  00141292: b 0x0014129c
  00141294: mov r0,r4
  00141296: blx 0x000768c4
  0014129a: ldr r0,[r4,#0x10]
  0014129c: ldr r1,[r4,#0x0]
  0014129e: ldr r0,[r0,#0x4]
  001412a0: ldr.w r0,[r0,r1,lsl #0x2]
  001412a4: cmp r0,#0x0
  001412a6: beq 0x00141294
  001412a8: ldr r0,[sp,#0x24]
  001412aa: ldr.w r1,[r9,#0x0]
  001412ae: subs r0,r1,r0
  001412b0: itttt eq
  001412b2: mov.eq r0,r4
  001412b4: add.eq sp,#0x28
  001412b6: pop.eq.w {r8,r9,r10}
  001412ba: pop.eq {r4,r5,r6,r7,pc}
  001412bc: blx 0x0006e824
```
