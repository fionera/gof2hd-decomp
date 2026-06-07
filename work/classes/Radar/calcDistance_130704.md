# Radar::calcDistance
elf 0x130704 body 484
Sig: undefined __stdcall calcDistance(float param_1, float param_2, float param_3, float param_4, float param_5, float param_6)

## decompile
```c

/* Radar::calcDistance(float, float, float, float, float, float) */

void Radar::calcDistance(float param_1,float param_2,float param_3,float param_4,float param_5,
                        float param_6)

{
  String *in_r0;
  float fVar1;
  float extraout_r0;
  int extraout_r1;
  float in_r2;
  float in_r3;
  code *pcVar2;
  int *piVar3;
  int iVar4;
  undefined4 extraout_s1;
  longlong lVar5;
  longlong lVar6;
  longlong lVar7;
  float in_stack_00000000;
  float in_stack_00000004;
  float in_stack_00000008;
  float in_stack_0000000c;
  String aSStack_84 [12];
  String aSStack_78 [12];
  String aSStack_6c [12];
  String aSStack_60 [12];
  String aSStack_54 [12];
  String aSStack_48 [12];
  int local_3c;
  
  lVar5 = __aeabi_f2lz(in_r2 * 0.5 - in_stack_00000004 * 0.5);
  lVar6 = __aeabi_f2lz(in_stack_00000000 * 0.5 - in_stack_0000000c * 0.5);
  lVar7 = __aeabi_f2lz(in_r3 * 0.5 - in_stack_00000008 * 0.5);
  lVar5 = lVar6 * lVar6 + lVar5 * lVar5 + lVar7 * lVar7;
  fVar1 = (float)__aeabi_l2f((int)lVar5,(int)((ulonglong)lVar5 >> 0x20));
  piVar3 = *(int **)(DAT_00140958 + 0x1407b8);
  local_3c = *piVar3;
  Globals::sqrt((Globals *)**(undefined4 **)(DAT_0014095c + 0x1407ba),
                (double)CONCAT44(extraout_s1,fVar1 * DAT_00140954));
  iVar4 = (int)extraout_r0;
  AbyssEngine::String::String(aSStack_48,iVar4 << 3);
  AbyssEngine::String::String(aSStack_54,(char *)(DAT_00140960 + 0x1407ea),false);
  AbyssEngine::operator+(in_r0,aSStack_48);
  AbyssEngine::String::~String(aSStack_54);
  AbyssEngine::String::~String(aSStack_48);
  if (0x7c < iVar4) {
    __aeabi_idivmod(iVar4 << 3,1000);
    if (extraout_r1 < 100) {
      AbyssEngine::String::String(aSStack_48,(char *)(DAT_00140970 + 0x140830),false);
      AbyssEngine::String::operator=(in_r0,aSStack_48);
    }
    else {
      AbyssEngine::String::String(aSStack_48,extraout_r1);
      AbyssEngine::String::operator=(in_r0,aSStack_48);
    }
    AbyssEngine::String::~String(aSStack_48);
    AbyssEngine::String::SubString((uint)aSStack_48,(uint)in_r0);
    AbyssEngine::String::operator=(in_r0,aSStack_48);
    AbyssEngine::String::~String(aSStack_48);
    iVar4 = __aeabi_uidiv(iVar4,0x7d);
    AbyssEngine::String::String(aSStack_6c,iVar4);
    AbyssEngine::String::String(aSStack_78,(char *)(DAT_00140964 + 0x140874),false);
    AbyssEngine::operator+(aSStack_60,aSStack_6c);
    AbyssEngine::operator+(aSStack_54,aSStack_60);
    AbyssEngine::String::String(aSStack_84,(char *)(DAT_00140968 + 0x140894),false);
    AbyssEngine::operator+(aSStack_48,aSStack_54);
    AbyssEngine::String::operator=(in_r0,aSStack_48);
    pcVar2 = *(code **)(DAT_0014096c + 0x1408b2);
    (*pcVar2)(aSStack_48);
    (*pcVar2)(aSStack_84);
    (*pcVar2)(aSStack_54);
    (*pcVar2)(aSStack_60);
    (*pcVar2)(aSStack_78);
    (*pcVar2)(aSStack_6c);
  }
  if (*piVar3 != local_3c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  00140704: push {r4,r5,r6,r7,lr}
  00140706: add r7,sp,#0xc
  00140708: push {r7,r8,r9,r10,r11}
  0014070c: vpush {d8,d9}
  00140710: sub sp,#0x58
  00140712: vmov.f32 s16,0x3f000000
  00140716: vldr.32 s2,[r7,#0xc]
  0014071a: vmov s0,r2
  0014071e: str r0,[sp,#0x4]
  00140720: mov r5,r3
  00140722: vmul.f32 s0,s0,s16
  00140726: vmls.f32 s0,s2,s16
  0014072a: vmov r0,s0
  0014072e: vldr.32 s0,[r7,#0x8]
  00140732: vmul.f32 s18,s0,s16
  00140736: blx 0x0006ebb4
  0014073a: vldr.32 s0,[r7,#0x14]
  0014073e: mov r6,r0
  00140740: vldr.32 s2,[r7,#0x10]
  00140744: mov r10,r1
  00140746: vmls.f32 s18,s0,s16
  0014074a: umull r1,r8,r6,r6
  0014074e: vmov s0,r5
  00140752: vmul.f32 s0,s0,s16
  00140756: str r1,[sp,#0x8]
  00140758: vmov r0,s18
  0014075c: vmls.f32 s0,s2,s16
  00140760: vmov r9,s0
  00140764: blx 0x0006ebb4
  00140768: mov r5,r0
  0014076a: umull r11,r0,r0,r0
  0014076e: mov r4,r1
  00140770: mla r1,r6,r10,r8
  00140774: mla r8,r5,r4,r0
  00140778: mov r0,r9
  0014077a: mla r6,r6,r10,r1
  0014077e: blx 0x0006ebb4
  00140782: umull r2,r3,r0,r0
  00140786: mla r3,r0,r1,r3
  0014078a: mla r0,r0,r1,r3
  0014078e: ldr r3,[sp,#0x8]
  00140790: mla r1,r5,r4,r8
  00140794: adds r2,r2,r3
  00140796: adc.w r3,r0,r6
  0014079a: adds.w r0,r2,r11
  0014079e: adcs r1,r3
  001407a0: blx 0x0006f7d8
  001407a4: vmov s2,r0
  001407a8: vldr.32 s0,[pc,#0x1a8]
  001407ac: ldr r0,[0x00140958]
  001407ae: vmul.f32 s0,s2,s0
  001407b2: ldr r2,[0x0014095c]
  001407b4: add r0,pc
  001407b6: add r2,pc
  001407b8: ldr.w r8,[r0,#0x0]
  001407bc: ldr r0,[r2,#0x0]
  001407be: ldr.w r2,[r8,#0x0]
  001407c2: vmov r1,s0
  001407c6: ldr r0,[r0,#0x0]
  001407c8: str r2,[sp,#0x54]
  001407ca: blx 0x000760e4
  001407ce: vmov s0,r0
  001407d2: add r0,sp,#0x48
  001407d4: vcvt.s32.f32 s0,s0
  001407d8: vmov r5,s0
  001407dc: lsls r6,r5,#0x3
  001407de: mov r1,r6
  001407e0: blx 0x0006f658
  001407e4: ldr r1,[0x00140960]
  001407e6: add r1,pc
  001407e8: add r0,sp,#0x3c
  001407ea: movs r2,#0x0
  001407ec: blx 0x0006ee18
  001407f0: ldr r4,[sp,#0x4]
  001407f2: add r1,sp,#0x48
  001407f4: add r2,sp,#0x3c
  001407f6: mov r0,r4
  001407f8: blx 0x0006ef98
  001407fc: add r0,sp,#0x3c
  001407fe: blx 0x0006ee30
  00140802: add r0,sp,#0x48
  00140804: blx 0x0006ee30
  00140808: cmp r5,#0x7d
  0014080a: blt 0x001408ca
  0014080c: mov r0,r6
  0014080e: mov.w r1,#0x3e8
  00140812: blx 0x0006f514
  00140816: cmp r1,#0x64
  00140818: blt 0x0014082a
  0014081a: add r0,sp,#0x48
  0014081c: blx 0x0006f658
  00140820: add r1,sp,#0x48
  00140822: mov r0,r4
  00140824: blx 0x0006f2b0
  00140828: b 0x0014083e
  0014082a: ldr r1,[0x00140970]
  0014082c: add r1,pc
  0014082e: add r0,sp,#0x48
  00140830: movs r2,#0x0
  00140832: blx 0x0006ee18
  00140836: add r1,sp,#0x48
  00140838: mov r0,r4
  0014083a: blx 0x0006f2b0
  0014083e: add r0,sp,#0x48
  00140840: blx 0x0006ee30
  00140844: add r0,sp,#0x48
  00140846: mov r1,r4
  00140848: movs r2,#0x0
  0014084a: movs r3,#0x1
  0014084c: blx 0x0006f604
  00140850: add r1,sp,#0x48
  00140852: mov r0,r4
  00140854: blx 0x0006f2b0
  00140858: add r0,sp,#0x48
  0014085a: blx 0x0006ee30
  0014085e: mov r0,r5
  00140860: movs r1,#0x7d
  00140862: blx 0x0006ec2c
  00140866: mov r1,r0
  00140868: add r0,sp,#0x24
  0014086a: blx 0x0006f658
  0014086e: ldr r1,[0x00140964]
  00140870: add r1,pc
  00140872: add r0,sp,#0x18
  00140874: movs r2,#0x0
  00140876: blx 0x0006ee18
  0014087a: add r0,sp,#0x30
  0014087c: add r1,sp,#0x24
  0014087e: add r2,sp,#0x18
  00140880: blx 0x0006ef98
  00140884: add r0,sp,#0x3c
  00140886: add r1,sp,#0x30
  00140888: mov r2,r4
  0014088a: blx 0x0006ef98
  0014088e: ldr r1,[0x00140968]
  00140890: add r1,pc
  00140892: add r0,sp,#0xc
  00140894: movs r2,#0x0
  00140896: blx 0x0006ee18
  0014089a: add r0,sp,#0x48
  0014089c: add r1,sp,#0x3c
  0014089e: add r2,sp,#0xc
  001408a0: blx 0x0006ef98
  001408a4: add r1,sp,#0x48
  001408a6: mov r0,r4
  001408a8: blx 0x0006f2b0
  001408ac: ldr r0,[0x0014096c]
  001408ae: add r0,pc
  001408b0: ldr r5,[r0,#0x0]
  001408b2: add r0,sp,#0x48
  001408b4: blx r5
  001408b6: add r0,sp,#0xc
  001408b8: blx r5
  001408ba: add r0,sp,#0x3c
  001408bc: blx r5
  001408be: add r0,sp,#0x30
  001408c0: blx r5
  001408c2: add r0,sp,#0x18
  001408c4: blx r5
  001408c6: add r0,sp,#0x24
  001408c8: blx r5
  001408ca: ldr r0,[sp,#0x54]
  001408cc: ldr.w r1,[r8,#0x0]
  001408d0: subs r0,r1,r0
  001408d2: itttt eq
  001408d4: add.eq sp,#0x58
  001408d6: vpop.eq {d8,d9}
  001408da: add.eq sp,#0x4
  001408dc: pop.eq.w {r8,r9,r10,r11}
  001408e0: it eq
  001408e2: pop.eq {r4,r5,r6,r7,pc}
  001408e4: blx 0x0006e824
```
