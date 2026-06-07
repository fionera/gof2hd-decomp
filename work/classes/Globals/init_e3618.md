# Globals::init
elf 0xe3618 body 632
Sig: int __stdcall init(EVP_PKEY_CTX * ctx)

## decompile
```c

/* Globals::init(AbyssEngine::ApplicationManager*, AbyssEngine::Engine*) */

int Globals::init(EVP_PKEY_CTX *ctx)

{
  char cVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  Mission *this;
  undefined4 *puVar6;
  undefined4 uVar7;
  Galaxy *this_00;
  Achievements *this_01;
  Status *this_02;
  ImageFactory *this_03;
  FileRead *this_04;
  void *pvVar8;
  AERandom *this_05;
  Generator *this_06;
  RecordHandler *this_07;
  FModSound *this_08;
  int iVar9;
  Layout *this_09;
  ApplicationManager *in_r1;
  undefined4 *puVar10;
  EVP_PKEY_CTX *ctx_00;
  char *pcVar11;
  undefined1 *puVar12;
  int *piVar13;
  undefined4 *puVar14;
  code *pcVar15;
  undefined1 *puVar16;
  undefined1 *puVar17;
  
  piVar13 = *(int **)(DAT_000f38c8 + 0xf362a);
  if (*piVar13 == 0) {
    this = operator_new(0x78);
    Mission::Mission(this);
    *piVar13 = (int)this;
  }
  uVar5 = DAT_000f38c0;
  uVar4 = DAT_000f38b8;
  puVar14 = *(undefined4 **)(DAT_000f38cc + 0xf364a);
  puVar6 = *(undefined4 **)(DAT_000f38d0 + 0xf364e);
  puVar10 = *(undefined4 **)(DAT_000f38d4 + 0xf3652);
  pcVar11 = *(char **)(DAT_000f38d8 + 0xf3656);
  puVar12 = *(undefined1 **)(DAT_000f38dc + 0xf3658);
  *(undefined1 *)((int)puVar14 + 0x11) = 1;
  *(undefined1 *)(puVar14 + 0xc) = 1;
  *puVar14 = 0x3f000000;
  puVar14[1] = 0x3f000000;
  puVar14[2] = 0x3f000000;
  puVar14[9] = 0x3f000000;
  puVar14[10] = 0x3f800000;
  *puVar12 = 0;
  cVar1 = *pcVar11;
  *(undefined8 *)(puVar14 + 5) = uVar4;
  *(undefined8 *)(puVar14 + 7) = uVar5;
  *(undefined2 *)((int)puVar14 + 0xf) = 0x101;
  *puVar6 = 0xffffffff;
  uVar7 = 0xc;
  if (cVar1 == '\0') {
    uVar7 = 6;
  }
  *puVar10 = uVar7;
  this_00 = operator_new(8);
  Galaxy::Galaxy(this_00);
  **(undefined4 **)(DAT_000f38e0 + 0xf36b4) = this_00;
  this_01 = operator_new(0x28);
  Achievements::Achievements(this_01);
  **(undefined4 **)(DAT_000f38e4 + 0xf36c8) = this_01;
  this_02 = operator_new(0x1f0);
  Status::Status(this_02);
  **(undefined4 **)(DAT_000f38e8 + 0xf36de) = this_02;
  this_03 = operator_new(0xc);
  ImageFactory::ImageFactory(this_03);
  **(undefined4 **)(DAT_000f38ec + 0xf36f6) = this_03;
  this_04 = operator_new(1);
  FileRead::FileRead(this_04);
  uVar7 = FileRead::loadItemsBinary();
  **(undefined4 **)(DAT_000f38f0 + 0xf3710) = uVar7;
  uVar7 = FileRead::loadShipsBinary();
  **(undefined4 **)(DAT_000f38f4 + 0xf371e) = uVar7;
  pvVar8 = (void *)FileRead::~FileRead(this_04);
  operator_delete(pvVar8);
  if (**(int **)(DAT_000f38f8 + 0xf3730) == 0) {
    **(int **)(DAT_000f38f8 + 0xf3730) = *(int *)in_r1;
  }
  **(undefined4 **)(DAT_000f38fc + 0xf3742) = in_r1;
  AbyssEngine::ApplicationManager::VibrateEnable(in_r1,false);
  this_05 = operator_new(8);
  AbyssEngine::AERandom::AERandom(this_05);
  puVar6 = *(undefined4 **)(DAT_000f3904 + 0xf3762);
  **(undefined4 **)(DAT_000f3900 + 0xf3760) = ctx;
  *puVar6 = this_05;
  this_06 = operator_new(1);
  Generator::Generator(this_06);
  **(undefined4 **)(DAT_000f3908 + 0xf377c) = this_06;
  this_07 = operator_new(0x2c);
  RecordHandler::RecordHandler(this_07);
  puVar6 = *(undefined4 **)(DAT_000f390c + 0xf3790);
  *puVar6 = this_07;
  Status::resetGame();
  RecordHandler::loadOptions((RecordHandler *)*puVar6);
  this_08 = operator_new(0x243c);
  FModSound::FModSound(this_08);
  puVar6 = *(undefined4 **)(DAT_000f3910 + 0xf37b4);
  *puVar6 = this_08;
  FModSound::init(this_08,ctx_00);
  pcVar15 = *(code **)(DAT_000f3914 + 0xf37c8);
  (*pcVar15)(*puVar6,1,*(undefined1 *)((int)puVar14 + 0xd));
  (*pcVar15)(*puVar6,2,*(undefined1 *)(puVar14 + 3));
  (*pcVar15)(*puVar6,3,*(undefined1 *)((int)puVar14 + 0xe));
  pcVar15 = *(code **)(DAT_000f3918 + 0xf37e8);
  (*pcVar15)(*puVar6,1,*puVar14);
  (*pcVar15)(*puVar6,2,puVar14[1]);
  (*pcVar15)(*puVar6,3,puVar14[2]);
  iVar9 = FModSound::tryToStopMusicForBGMusic();
  if (iVar9 != 0) {
    *(undefined1 *)((int)puVar14 + 0xd) = 0;
  }
  iVar9 = DAT_000f3930;
  uVar7 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  uVar2 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  uVar3 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  puVar10 = (undefined4 *)(DAT_000f3928 + 0xf3822);
  puVar12 = *(undefined1 **)(DAT_000f3920 + 0xf381a);
  puVar6 = *(undefined4 **)(DAT_000f3924 + 0xf381e);
  puVar17 = *(undefined1 **)(DAT_000f392c + 0xf3824);
  **(undefined4 **)(DAT_000f391c + 0xf3816) = 0;
  *puVar12 = 1;
  *puVar6 = 0;
  puVar6[1] = uVar7;
  puVar6[2] = uVar2;
  puVar6[3] = uVar3;
  puVar12 = (undefined1 *)*puVar10;
  puVar16 = *(undefined1 **)(iVar9 + 0xf383a);
  *(undefined4 *)((int)puVar6 + 0x2b) = 0;
  *(undefined4 *)((int)puVar6 + 0x2f) = uVar7;
  *(undefined4 *)((int)puVar6 + 0x33) = uVar2;
  *(undefined4 *)((int)puVar6 + 0x37) = uVar3;
  puVar6[8] = 0;
  puVar6[9] = uVar7;
  puVar6[10] = uVar2;
  puVar6[0xb] = uVar3;
  puVar6[4] = 0;
  puVar6[5] = uVar7;
  puVar6[6] = uVar2;
  puVar6[7] = uVar3;
  *puVar12 = 0;
  *puVar17 = 0;
  *puVar16 = 0;
  this_09 = operator_new(0x414);
  Layout::Layout(this_09);
  **(undefined4 **)(DAT_000f3934 + 0xf386e) = this_09;
  Layout::reload();
  ParticleSettingsRef::initialize();
  pvVar8 = operator_new(0xc);
  iVar9 = Array<int>::Array();
  *(void **)(ctx + 4) = pvVar8;
  return iVar9;
}

```

## target disasm
```
  000f3618: push {r4,r5,r6,r7,lr}
  000f361a: add r7,sp,#0xc
  000f361c: push {r8,r9,r10}
  000f3620: mov r8,r0
  000f3622: ldr r0,[0x000f38c8]
  000f3624: mov r10,r1
  000f3626: add r0,pc
  000f3628: ldr r4,[r0,#0x0]
  000f362a: ldr r0,[r4,#0x0]
  000f362c: cbnz r0,0x000f363c
  000f362e: movs r0,#0x78
  000f3630: blx 0x0006eb24
  000f3634: mov r6,r0
  000f3636: blx 0x0007384c
  000f363a: str r6,[r4,#0x0]
  000f363c: ldr r0,[0x000f38cc]
  000f363e: mov.w r5,#0x3f800000
  000f3642: ldr r1,[0x000f38d0]
  000f3644: ldr r2,[0x000f38d4]
  000f3646: add r0,pc
  000f3648: ldr r3,[0x000f38d8]
  000f364a: add r1,pc
  000f364c: ldr r6,[0x000f38dc]
  000f364e: add r2,pc
  000f3650: ldr r4,[r0,#0x0]
  000f3652: add r3,pc
  000f3654: add r6,pc
  000f3656: adr r0,[0xf38b8]
  000f3658: vld1.64 {d16,d17},[r0]
  000f365c: ldr r0,[r1,#0x0]
  000f365e: ldr r1,[r2,#0x0]
  000f3660: ldr r2,[r3,#0x0]
  000f3662: ldr r3,[r6,#0x0]
  000f3664: movs r6,#0x1
  000f3666: strb r6,[r4,#0x11]
  000f3668: strb.w r6,[r4,#0x30]
  000f366c: mov.w r6,#0x3f000000
  000f3670: strd r6,r6,[r4,#0x0]
  000f3674: str r6,[r4,#0x8]
  000f3676: strd r6,r5,[r4,#0x24]
  000f367a: movs r6,#0x0
  000f367c: strb r6,[r3,#0x0]
  000f367e: add.w r3,r4,#0x14
  000f3682: ldrb r2,[r2,#0x0]
  000f3684: vst1.32 {d16,d17},[r3]
  000f3688: movw r3,#0x101
  000f368c: cmp r2,#0x0
  000f368e: strh.w r3,[r4,#0xf]
  000f3692: mov.w r3,#0xffffffff
  000f3696: str r3,[r0,#0x0]
  000f3698: mov.w r0,#0xc
  000f369c: it eq
  000f369e: mov.eq r0,#0x6
  000f36a0: str r0,[r1,#0x0]
  000f36a2: movs r0,#0x8
  000f36a4: blx 0x0006eb24
  000f36a8: mov r6,r0
  000f36aa: blx 0x00075d00
  000f36ae: ldr r0,[0x000f38e0]
  000f36b0: add r0,pc
  000f36b2: ldr r0,[r0,#0x0]
  000f36b4: str r6,[r0,#0x0]
  000f36b6: movs r0,#0x28
  000f36b8: blx 0x0006eb24
  000f36bc: mov r6,r0
  000f36be: blx 0x00075d0c
  000f36c2: ldr r0,[0x000f38e4]
  000f36c4: add r0,pc
  000f36c6: ldr r0,[r0,#0x0]
  000f36c8: str r6,[r0,#0x0]
  000f36ca: mov.w r0,#0x1f0
  000f36ce: blx 0x0006eb24
  000f36d2: mov r6,r0
  000f36d4: blx 0x00075d18
  000f36d8: ldr r0,[0x000f38e8]
  000f36da: add r0,pc
  000f36dc: ldr.w r9,[r0,#0x0]
  000f36e0: movs r0,#0xc
  000f36e2: str.w r6,[r9,#0x0]
  000f36e6: blx 0x0006eb24
  000f36ea: mov r6,r0
  000f36ec: blx 0x00075d24
  000f36f0: ldr r0,[0x000f38ec]
  000f36f2: add r0,pc
  000f36f4: ldr r0,[r0,#0x0]
  000f36f6: str r6,[r0,#0x0]
  000f36f8: movs r0,#0x1
  000f36fa: blx 0x0006eb24
  000f36fe: mov r6,r0
  000f3700: blx 0x00072124
  000f3704: mov r0,r6
  000f3706: blx 0x00075d30
  000f370a: ldr r1,[0x000f38f0]
  000f370c: add r1,pc
  000f370e: ldr r1,[r1,#0x0]
  000f3710: str r0,[r1,#0x0]
  000f3712: mov r0,r6
  000f3714: blx 0x00075d3c
  000f3718: ldr r1,[0x000f38f4]
  000f371a: add r1,pc
  000f371c: ldr r1,[r1,#0x0]
  000f371e: str r0,[r1,#0x0]
  000f3720: mov r0,r6
  000f3722: blx 0x0007213c
  000f3726: blx 0x0006eb48
  000f372a: ldr r0,[0x000f38f8]
  000f372c: add r0,pc
  000f372e: ldr r0,[r0,#0x0]
  000f3730: ldr r1,[r0,#0x0]
  000f3732: cbnz r1,0x000f373a
  000f3734: ldr.w r1,[r10,#0x0]
  000f3738: str r1,[r0,#0x0]
  000f373a: ldr r0,[0x000f38fc]
  000f373c: movs r1,#0x0
  000f373e: add r0,pc
  000f3740: ldr r0,[r0,#0x0]
  000f3742: str.w r10,[r0,#0x0]
  000f3746: mov r0,r10
  000f3748: blx 0x00075d48
  000f374c: movs r0,#0x8
  000f374e: blx 0x0006eb24
  000f3752: mov r5,r0
  000f3754: blx 0x00075d54
  000f3758: ldr r0,[0x000f3900]
  000f375a: ldr r1,[0x000f3904]
  000f375c: add r0,pc
  000f375e: add r1,pc
  000f3760: ldr r0,[r0,#0x0]
  000f3762: ldr r1,[r1,#0x0]
  000f3764: str.w r8,[r0,#0x0]
  000f3768: movs r0,#0x1
  000f376a: str r5,[r1,#0x0]
  000f376c: blx 0x0006eb24
  000f3770: mov r5,r0
  000f3772: blx 0x000736cc
  000f3776: ldr r0,[0x000f3908]
  000f3778: add r0,pc
  000f377a: ldr r0,[r0,#0x0]
  000f377c: str r5,[r0,#0x0]
  000f377e: movs r0,#0x2c
  000f3780: blx 0x0006eb24
  000f3784: mov r5,r0
  000f3786: blx 0x0007177c
  000f378a: ldr r0,[0x000f390c]
  000f378c: add r0,pc
  000f378e: ldr r6,[r0,#0x0]
  000f3790: ldr.w r0,[r9,#0x0]
  000f3794: str r5,[r6,#0x0]
  000f3796: blx 0x00074d58
  000f379a: ldr r0,[r6,#0x0]
  000f379c: blx 0x00075d60
  000f37a0: movw r0,#0x243c
  000f37a4: blx 0x0006eb24
  000f37a8: mov r5,r0
  000f37aa: blx 0x00075d6c
  000f37ae: ldr r0,[0x000f3910]
  000f37b0: add r0,pc
  000f37b2: ldr r6,[r0,#0x0]
  000f37b4: mov r0,r5
  000f37b6: str r5,[r6,#0x0]
  000f37b8: blx 0x00075d78
  000f37bc: ldr r1,[0x000f3914]
  000f37be: mov.w r9,#0x1
  000f37c2: ldrb r2,[r4,#0xd]
  000f37c4: add r1,pc
  000f37c6: ldr r0,[r6,#0x0]
  000f37c8: ldr r5,[r1,#0x0]
  000f37ca: movs r1,#0x1
  000f37cc: blx r5
  000f37ce: ldrb r2,[r4,#0xc]
  000f37d0: movs r1,#0x2
  000f37d2: ldr r0,[r6,#0x0]
  000f37d4: blx r5
  000f37d6: ldrb r2,[r4,#0xe]
  000f37d8: movs r1,#0x3
  000f37da: ldr r0,[r6,#0x0]
  000f37dc: blx r5
  000f37de: ldr r0,[0x000f3918]
  000f37e0: movs r1,#0x1
  000f37e2: ldr r2,[r4,#0x0]
  000f37e4: add r0,pc
  000f37e6: ldr r5,[r0,#0x0]
  000f37e8: ldr r0,[r6,#0x0]
  000f37ea: blx r5
  000f37ec: ldr r2,[r4,#0x4]
  000f37ee: movs r1,#0x2
  000f37f0: ldr r0,[r6,#0x0]
  000f37f2: blx r5
  000f37f4: ldr r2,[r4,#0x8]
  000f37f6: movs r1,#0x3
  000f37f8: ldr r0,[r6,#0x0]
  000f37fa: blx r5
  000f37fc: ldr r0,[r6,#0x0]
  000f37fe: blx 0x000756c4
  000f3802: cbz r0,0x000f3808
  000f3804: movs r0,#0x0
  000f3806: strb r0,[r4,#0xd]
  000f3808: ldr r0,[0x000f391c]
  000f380a: vmov.i32 q8,#0x0
  000f380e: ldr r1,[0x000f3920]
  000f3810: ldr r2,[0x000f3924]
  000f3812: add r0,pc
  000f3814: ldr r3,[0x000f3928]
  000f3816: add r1,pc
  000f3818: ldr r4,[0x000f392c]
  000f381a: add r2,pc
  000f381c: ldr r0,[r0,#0x0]
  000f381e: add r3,pc
  000f3820: add r4,pc
  000f3822: ldr r1,[r1,#0x0]
  000f3824: ldr r2,[r2,#0x0]
  000f3826: ldr r5,[0x000f3930]
  000f3828: ldr r6,[r4,#0x0]
  000f382a: movs r4,#0x0
  000f382c: str r4,[r0,#0x0]
  000f382e: movs r0,#0x2b
  000f3830: strb.w r9,[r1,#0x0]
  000f3834: mov r1,r2
  000f3836: add r5,pc
  000f3838: vst1.8 {d16,d17},[r1],r0
  000f383c: add.w r0,r2,#0x20
  000f3840: ldr r3,[r3,#0x0]
  000f3842: ldr r5,[r5,#0x0]
  000f3844: vst1.8 {d16,d17},[r1]
  000f3848: vst1.8 {d16,d17},[r0]
  000f384c: add.w r0,r2,#0x10
  000f3850: vst1.8 {d16,d17},[r0]
  000f3854: movw r0,#0x414
  000f3858: strb r4,[r3,#0x0]
  000f385a: strb r4,[r6,#0x0]
  000f385c: strb r4,[r5,#0x0]
  000f385e: blx 0x0006eb24
  000f3862: mov r5,r0
  000f3864: blx 0x00075d84
  000f3868: ldr r0,[0x000f3934]
  000f386a: add r0,pc
  000f386c: ldr r0,[r0,#0x0]
  000f386e: str r5,[r0,#0x0]
  000f3870: mov r0,r5
  000f3872: blx 0x00071d10
  000f3876: blx 0x00075d90
  000f387a: movs r0,#0xc
  000f387c: blx 0x0006eb24
  000f3880: mov r5,r0
  000f3882: blx 0x000701f8
  000f3886: str.w r5,[r8,#0x4]
  000f388a: pop.w {r8,r9,r10}
  000f388e: pop {r4,r5,r6,r7,pc}
```
