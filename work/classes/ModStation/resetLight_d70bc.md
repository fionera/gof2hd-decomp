# ModStation::resetLight
elf 0xd70bc body 270
Sig: undefined __stdcall resetLight(void)

## decompile
```c

/* ModStation::resetLight() */

void ModStation::resetLight(void)

{
  uint uVar1;
  undefined4 uVar2;
  undefined4 *puVar3;
  code *pcVar4;
  float extraout_s0;
  float extraout_s0_00;
  float extraout_s0_01;
  float extraout_s0_02;
  float extraout_s0_03;
  float extraout_s0_04;
  float extraout_s0_05;
  float extraout_s0_06;
  float fVar5;
  float extraout_s0_07;
  float extraout_s1;
  float extraout_s1_00;
  float extraout_s1_01;
  float extraout_s1_02;
  float extraout_s1_03;
  float extraout_s1_04;
  float extraout_s1_05;
  float extraout_s1_06;
  float fVar6;
  float extraout_s1_07;
  float extraout_s2;
  float extraout_s2_00;
  float extraout_s2_01;
  float extraout_s2_02;
  float extraout_s2_03;
  float extraout_s2_04;
  float extraout_s2_05;
  float extraout_s2_06;
  float fVar7;
  float extraout_s2_07;
  
  puVar3 = *(undefined4 **)(DAT_000e71f4 + 0xe70d0);
  pcVar4 = *(code **)(DAT_000e71f8 + 0xe70d2);
  (*pcVar4)(*puVar3);
  AbyssEngine::Engine::LightSetMaterialColorAmbient(extraout_s0,extraout_s1,extraout_s2);
  uVar1 = (*pcVar4)(*puVar3);
  AbyssEngine::Engine::LightSetLightDirection(extraout_s0_00,extraout_s1_00,extraout_s2_00,uVar1);
  uVar1 = (*pcVar4)(*puVar3);
  AbyssEngine::Engine::LightSetLightColorDiffuse(extraout_s0_01,extraout_s1_01,extraout_s2_01,uVar1)
  ;
  Status::getSystem();
  uVar2 = SolarSystem::getRace();
  switch(uVar2) {
  case 1:
    uVar1 = AbyssEngine::ApplicationManager::GetEngine();
    fVar5 = extraout_s0_02;
    fVar6 = extraout_s1_02;
    fVar7 = extraout_s2_02;
    break;
  case 2:
    uVar1 = AbyssEngine::ApplicationManager::GetEngine();
    fVar5 = extraout_s0_03;
    fVar6 = extraout_s1_03;
    fVar7 = extraout_s2_03;
    break;
  case 3:
    uVar1 = AbyssEngine::ApplicationManager::GetEngine();
    fVar5 = extraout_s0_04;
    fVar6 = extraout_s1_04;
    fVar7 = extraout_s2_04;
    break;
  default:
    uVar1 = AbyssEngine::ApplicationManager::GetEngine();
    fVar5 = extraout_s0_05;
    fVar6 = extraout_s1_05;
    fVar7 = extraout_s2_05;
    break;
  case 8:
    uVar1 = AbyssEngine::ApplicationManager::GetEngine();
    fVar5 = extraout_s0_06;
    fVar6 = extraout_s1_06;
    fVar7 = extraout_s2_06;
  }
  AbyssEngine::Engine::LightSetLightColorAmbient(fVar5,fVar6,fVar7,uVar1);
  uVar1 = AbyssEngine::ApplicationManager::GetEngine();
  AbyssEngine::Engine::LightSetLightColorSpecular
            (extraout_s0_07,extraout_s1_07,extraout_s2_07,uVar1);
  uVar2 = AbyssEngine::ApplicationManager::GetEngine();
  (*(code *)(DAT_001ac114 + 0x1ac118))(uVar2,DAT_000e71f0);
  return;
}

```

## target disasm
```
  000e70bc: push {r4,r5,r6,r7,lr}
  000e70be: add r7,sp,#0xc
  000e70c0: push.w r11
  000e70c4: sub sp,#0x8
  000e70c6: mov r4,r0
  000e70c8: ldr r0,[0x000e71f4]
  000e70ca: ldr r1,[0x000e71f8]
  000e70cc: add r0,pc
  000e70ce: add r1,pc
  000e70d0: ldr r5,[r0,#0x0]
  000e70d2: ldr r6,[r1,#0x0]
  000e70d4: ldr r0,[r5,#0x0]
  000e70d6: blx r6
  000e70d8: ldr r1,[0x000e71d0]
  000e70da: mov r2,r1
  000e70dc: mov r3,r1
  000e70de: blx 0x000745cc
  000e70e2: ldr r0,[r5,#0x0]
  000e70e4: blx r6
  000e70e6: add.w r3,r4,#0x11c
  000e70ea: mov.w r4,#0x4000
  000e70ee: ldmia r3,{r1,r2,r3}
  000e70f0: eor r1,r1,#0x80000000
  000e70f4: str r4,[sp,#0x0]
  000e70f6: eor r2,r2,#0x80000000
  000e70fa: eor r3,r3,#0x80000000
  000e70fe: blx 0x000745a8
  000e7102: ldr r0,[r5,#0x0]
  000e7104: blx r6
  000e7106: mov.w r1,#0x3f800000
  000e710a: mov.w r2,#0x3f800000
  000e710e: mov.w r3,#0x3f800000
  000e7112: str r4,[sp,#0x0]
  000e7114: blx 0x000745c0
  000e7118: ldr r0,[0x000e71fc]
  000e711a: add r0,pc
  000e711c: ldr r0,[r0,#0x0]
  000e711e: ldr r0,[r0,#0x0]
  000e7120: blx 0x00071a10
  000e7124: blx 0x00071950
  000e7128: subs r0,#0x1
  000e712a: cmp r0,#0x7
  000e712c: bhi 0x000e7172
  000e712e: tbb [pc,r0]
  000e713a: ldr r0,[r5,#0x0]
  000e713c: blx 0x0007258c
  000e7140: mov.w r1,#0x4000
  000e7144: ldr r2,[0x000e71ec]
  000e7146: str r1,[sp,#0x0]
  000e7148: mov.w r1,#0x3e800000
  000e714c: b 0x000e7182
  000e714e: ldr r0,[r5,#0x0]
  000e7150: blx 0x0007258c
  000e7154: ldr r1,[0x000e71e0]
  000e7156: mov.w r2,#0x4000
  000e715a: str r2,[sp,#0x0]
  000e715c: ldr r3,[0x000e71e4]
  000e715e: mov r2,r1
  000e7160: b 0x000e719a
  000e7162: ldr r0,[r5,#0x0]
  000e7164: blx 0x0007258c
  000e7168: mov.w r2,#0x4000
  000e716c: ldr r1,[0x000e71e8]
  000e716e: str r2,[sp,#0x0]
  000e7170: b 0x000e717e
  000e7172: ldr r0,[r5,#0x0]
  000e7174: blx 0x0007258c
  000e7178: mov.w r1,#0x3e800000
  000e717c: str r4,[sp,#0x0]
  000e717e: mov.w r2,#0x3e800000
  000e7182: mov.w r3,#0x3e800000
  000e7186: b 0x000e719a
  000e7188: ldr r0,[r5,#0x0]
  000e718a: blx 0x0007258c
  000e718e: mov.w r6,#0x4000
  000e7192: ldr r1,[0x000e71d4]
  000e7194: ldr r2,[0x000e71d8]
  000e7196: ldr r3,[0x000e71dc]
  000e7198: str r6,[sp,#0x0]
  000e719a: blx 0x0007087c
  000e719e: ldr r0,[r5,#0x0]
  000e71a0: blx 0x0007258c
  000e71a4: mov.w r1,#0x4000
  000e71a8: mov.w r2,#0x3f000000
  000e71ac: str r1,[sp,#0x0]
  000e71ae: mov.w r1,#0x3f000000
  000e71b2: mov.w r3,#0x3f000000
  000e71b6: blx 0x00075040
  000e71ba: ldr r0,[r5,#0x0]
  000e71bc: blx 0x0007258c
  000e71c0: ldr r1,[0x000e71f0]
  000e71c2: add sp,#0x8
  000e71c4: pop.w r11
  000e71c8: pop.w {r4,r5,r6,r7,lr}
  000e71cc: b.w 0x001ac108
  001ac108: bx pc
```
