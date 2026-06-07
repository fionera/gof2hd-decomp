# HangarWindow::showCreditsBuyWindow
elf 0x14e118 body 410
Sig: undefined __thiscall showCreditsBuyWindow(HangarWindow * this)

## decompile
```c

/* HangarWindow::showCreditsBuyWindow() */

void __thiscall HangarWindow::showCreditsBuyWindow(HangarWindow *this)

{
  int iVar1;
  String *pSVar2;
  char *pcVar3;
  code *pcVar4;
  ChoiceWindow *pCVar5;
  int *piVar6;
  uint in_fpscr;
  float fVar7;
  String aSStack_54 [12];
  String aSStack_48 [12];
  String aSStack_3c [12];
  String aSStack_30 [12];
  int local_24;
  
  piVar6 = *(int **)(DAT_0015e2ec + 0x15e12c);
  local_24 = *piVar6;
  iVar1 = AbyssEngine::ApplicationManager::GetApplicationData();
  *(undefined1 *)(iVar1 + 0x4c) = 0;
  iVar1 = AbyssEngine::ApplicationManager::GetApplicationData();
  *(undefined1 *)(iVar1 + 0x3d) = 1;
  pCVar5 = *(ChoiceWindow **)(this + 0x20);
  if (this[0x11f] == (HangarWindow)0x0) {
    AbyssEngine::String::String(aSStack_30,(char *)(DAT_0015e30c + 0x15e20e),false);
    AbyssEngine::String::String(aSStack_3c,(char *)(DAT_0015e310 + 0x15e216),false);
    AbyssEngine::String::String(aSStack_48,(char *)(DAT_0015e314 + 0x15e222),false);
    AbyssEngine::String::String(aSStack_54,(char *)(DAT_0015e318 + 0x15e230),false);
    pSVar2 = (String *)GameText::getText(**(int **)(DAT_0015e31c + 0x15e23e));
    ChoiceWindow::set(pCVar5,aSStack_30,aSStack_3c,false,aSStack_48,aSStack_54,pSVar2,-1,-1);
    pcVar4 = *(code **)(DAT_0015e320 + 0x15e266);
    (*pcVar4)(aSStack_54);
    (*pcVar4)(aSStack_48);
    (*pcVar4)(aSStack_3c);
    (*pcVar4)(aSStack_30);
  }
  else {
    pcVar3 = (char *)(DAT_0015e2f4 + 0x15e166);
    AbyssEngine::String::String(aSStack_30,pcVar3,false);
    AbyssEngine::String::String(aSStack_3c,pcVar3,false);
    AbyssEngine::String::String(aSStack_48,(char *)(DAT_0015e2f8 + 0x15e17a),false);
    AbyssEngine::String::String(aSStack_54,(char *)(DAT_0015e2fc + 0x15e188),false);
    pSVar2 = (String *)GameText::getText(**(int **)(DAT_0015e300 + 0x15e196));
    ChoiceWindow::set(pCVar5,aSStack_30,aSStack_3c,false,aSStack_48,aSStack_54,pSVar2,-1,-1);
    pcVar4 = *(code **)(DAT_0015e304 + 0x15e1be);
    (*pcVar4)(aSStack_54);
    (*pcVar4)(aSStack_48);
    (*pcVar4)(aSStack_3c);
    (*pcVar4)(aSStack_30);
    if (**(char **)(DAT_0015e308 + 0x15e1d4) == '\0') {
      ChoiceWindow::setWidth(*(ChoiceWindow **)(this + 0x20),**(int **)(DAT_0015e324 + 0x15e27e));
      iVar1 = **(int **)(DAT_0015e328 + 0x15e28a);
      pCVar5 = *(ChoiceWindow **)(this + 0x20);
    }
    else {
      ChoiceWindow::setWidth(*(ChoiceWindow **)(this + 0x20),*(int *)(this + 0x120) * 3);
      fVar7 = (float)VectorSignedToFloat(*(undefined4 *)(this + 0x124),(byte)(in_fpscr >> 0x16) & 3)
      ;
      pCVar5 = *(ChoiceWindow **)(this + 0x20);
      iVar1 = (int)(fVar7 * DAT_0015e2e8);
    }
    ChoiceWindow::setHeight(pCVar5,iVar1);
  }
  this[0x3c] = (HangarWindow)0x1;
  *(undefined2 *)(this + 0xae) = 1;
  if (*piVar6 != local_24) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  0015e118: push {r4,r5,r6,r7,lr}
  0015e11a: add r7,sp,#0xc
  0015e11c: push {r8,r9,r11}
  0015e120: sub sp,#0x48
  0015e122: mov r4,r0
  0015e124: ldr r0,[0x0015e2ec]
  0015e126: ldr r1,[0x0015e2f0]
  0015e128: add r0,pc
  0015e12a: add r1,pc
  0015e12c: ldr.w r9,[r0,#0x0]
  0015e130: ldr r5,[r1,#0x0]
  0015e132: ldr.w r1,[r9,#0x0]
  0015e136: ldr r0,[r5,#0x0]
  0015e138: str r1,[sp,#0x44]
  0015e13a: blx 0x00071704
  0015e13e: movs r1,#0x0
  0015e140: strb.w r1,[r0,#0x4c]
  0015e144: ldr r0,[r5,#0x0]
  0015e146: blx 0x00071704
  0015e14a: movs r1,#0x1
  0015e14c: strb.w r1,[r0,#0x3d]
  0015e150: ldrb.w r0,[r4,#0x11f]
  0015e154: ldr.w r8,[r4,#0x20]
  0015e158: cmp r0,#0x0
  0015e15a: beq 0x0015e204
  0015e15c: ldr r5,[0x0015e2f4]
  0015e15e: add r0,sp,#0x38
  0015e160: movs r2,#0x0
  0015e162: add r5,pc
  0015e164: mov r1,r5
  0015e166: blx 0x0006ee18
  0015e16a: add r0,sp,#0x2c
  0015e16c: mov r1,r5
  0015e16e: movs r2,#0x0
  0015e170: blx 0x0006ee18
  0015e174: ldr r1,[0x0015e2f8]
  0015e176: add r1,pc
  0015e178: add r6,sp,#0x20
  0015e17a: movs r2,#0x0
  0015e17c: mov r0,r6
  0015e17e: blx 0x0006ee18
  0015e182: ldr r1,[0x0015e2fc]
  0015e184: add r1,pc
  0015e186: add r5,sp,#0x14
  0015e188: movs r2,#0x0
  0015e18a: mov r0,r5
  0015e18c: blx 0x0006ee18
  0015e190: ldr r0,[0x0015e300]
  0015e192: add r0,pc
  0015e194: ldr r0,[r0,#0x0]
  0015e196: ldr r0,[r0,#0x0]
  0015e198: movs r1,#0xaa
  0015e19a: blx 0x00072f70
  0015e19e: mov.w r1,#0xffffffff
  0015e1a2: strd r6,r5,[sp,#0x0]
  0015e1a6: strd r0,r1,[sp,#0x8]
  0015e1aa: add r2,sp,#0x2c
  0015e1ac: str r1,[sp,#0x10]
  0015e1ae: add r1,sp,#0x38
  0015e1b0: mov r0,r8
  0015e1b2: movs r3,#0x0
  0015e1b4: blx 0x00075574
  0015e1b8: ldr r0,[0x0015e304]
  0015e1ba: add r0,pc
  0015e1bc: ldr r5,[r0,#0x0]
  0015e1be: add r0,sp,#0x14
  0015e1c0: blx r5
  0015e1c2: add r0,sp,#0x20
  0015e1c4: blx r5
  0015e1c6: add r0,sp,#0x2c
  0015e1c8: blx r5
  0015e1ca: add r0,sp,#0x38
  0015e1cc: blx r5
  0015e1ce: ldr r0,[0x0015e308]
  0015e1d0: add r0,pc
  0015e1d2: ldr r1,[r0,#0x0]
  0015e1d4: ldr r0,[r4,#0x20]
  0015e1d6: ldrb r1,[r1,#0x0]
  0015e1d8: cmp r1,#0x0
  0015e1da: beq 0x0015e278
  0015e1dc: ldr.w r1,[r4,#0x120]
  0015e1e0: add.w r1,r1,r1, lsl #0x1
  0015e1e4: blx 0x0007723c
  0015e1e8: vldr.32 s0,[r4,#0x124]
  0015e1ec: vldr.32 s2,[pc,#0xf8]
  0015e1f0: vcvt.f32.s32 s0,s0
  0015e1f4: ldr r0,[r4,#0x20]
  0015e1f6: vmul.f32 s0,s0,s2
  0015e1fa: vcvt.s32.f32 s0,s0
  0015e1fe: vmov r1,s0
  0015e202: b 0x0015e28e
  0015e204: ldr r1,[0x0015e30c]
  0015e206: add r0,sp,#0x38
  0015e208: movs r2,#0x0
  0015e20a: add r1,pc
  0015e20c: blx 0x0006ee18
  0015e210: ldr r1,[0x0015e310]
  0015e212: add r1,pc
  0015e214: add r0,sp,#0x2c
  0015e216: movs r2,#0x0
  0015e218: blx 0x0006ee18
  0015e21c: ldr r1,[0x0015e314]
  0015e21e: add r1,pc
  0015e220: add r6,sp,#0x20
  0015e222: movs r2,#0x0
  0015e224: mov r0,r6
  0015e226: blx 0x0006ee18
  0015e22a: ldr r1,[0x0015e318]
  0015e22c: add r1,pc
  0015e22e: add r5,sp,#0x14
  0015e230: movs r2,#0x0
  0015e232: mov r0,r5
  0015e234: blx 0x0006ee18
  0015e238: ldr r0,[0x0015e31c]
  0015e23a: add r0,pc
  0015e23c: ldr r0,[r0,#0x0]
  0015e23e: ldr r0,[r0,#0x0]
  0015e240: movs r1,#0xaa
  0015e242: blx 0x00072f70
  0015e246: mov.w r1,#0xffffffff
  0015e24a: strd r6,r5,[sp,#0x0]
  0015e24e: strd r0,r1,[sp,#0x8]
  0015e252: add r2,sp,#0x2c
  0015e254: str r1,[sp,#0x10]
  0015e256: add r1,sp,#0x38
  0015e258: mov r0,r8
  0015e25a: movs r3,#0x0
  0015e25c: blx 0x00075574
  0015e260: ldr r0,[0x0015e320]
  0015e262: add r0,pc
  0015e264: ldr r5,[r0,#0x0]
  0015e266: add r0,sp,#0x14
  0015e268: blx r5
  0015e26a: add r0,sp,#0x20
  0015e26c: blx r5
  0015e26e: add r0,sp,#0x2c
  0015e270: blx r5
  0015e272: add r0,sp,#0x38
  0015e274: blx r5
  0015e276: b 0x0015e292
  0015e278: ldr r1,[0x0015e324]
  0015e27a: add r1,pc
  0015e27c: ldr r1,[r1,#0x0]
  0015e27e: ldr r1,[r1,#0x0]
  0015e280: blx 0x0007723c
  0015e284: ldr r0,[0x0015e328]
  0015e286: add r0,pc
  0015e288: ldr r0,[r0,#0x0]
  0015e28a: ldr r1,[r0,#0x0]
  0015e28c: ldr r0,[r4,#0x20]
  0015e28e: blx 0x00076d98
  0015e292: movs r0,#0x1
  0015e294: strb.w r0,[r4,#0x3c]
  0015e298: strh.w r0,[r4,#0xae]
  0015e29c: ldr r0,[sp,#0x44]
  0015e29e: ldr.w r1,[r9,#0x0]
  0015e2a2: subs r0,r1,r0
  0015e2a4: ittt eq
  0015e2a6: add.eq sp,#0x48
  0015e2a8: pop.eq.w {r8,r9,r11}
  0015e2ac: pop.eq {r4,r5,r6,r7,pc}
  0015e2ae: blx 0x0006e824
```
