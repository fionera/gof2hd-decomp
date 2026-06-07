# Layout::formatNumber
elf 0xd32b4 body 390
Sig: undefined __stdcall formatNumber(int param_1)

## decompile
```c

/* Layout::formatNumber(int) */

void Layout::formatNumber(int param_1)

{
  ushort uVar1;
  uint uVar2;
  uint in_r2;
  int *piVar3;
  int iVar4;
  code *pcVar5;
  String aSStack_70 [12];
  String aSStack_64 [12];
  String aSStack_58 [12];
  String aSStack_4c [8];
  int local_44;
  String aSStack_40 [12];
  String aSStack_34 [8];
  int local_2c;
  int local_28;
  
  piVar3 = *(int **)(DAT_000e34c0 + 0xe32cc);
  local_28 = *piVar3;
  uVar2 = in_r2;
  if ((int)in_r2 < 0) {
    uVar2 = -in_r2;
  }
  AbyssEngine::String::String(aSStack_34,uVar2);
  AbyssEngine::String::String((String *)param_1,(char *)(DAT_000e34c4 + 0xe32e4),false);
  uVar1 = GameText::getLanguage();
  if ((uVar1 < 0xc) && ((1 << (uVar1 & 0xff) & 0xc01U) != 0)) {
    AbyssEngine::String::String(aSStack_40,(char *)(DAT_000e34c8 + 0xe3308),false);
  }
  else {
    AbyssEngine::String::String(aSStack_40,(char *)(DAT_000e34cc + 0xe3316),false);
  }
  if (local_2c < 4) {
    AbyssEngine::String::operator=((String *)param_1,aSStack_34);
  }
  else {
    pcVar5 = *(code **)(DAT_000e34d0 + 0xe3336);
    iVar4 = local_2c;
    while (2 < iVar4) {
      iVar4 = iVar4 + -3;
      AbyssEngine::String::SubString((uint)aSStack_64,(uint)aSStack_34);
      if (*(uint *)(param_1 + 8) < 2) {
        AbyssEngine::String::String(aSStack_70);
      }
      else {
        AbyssEngine::String::String(aSStack_70,aSStack_40,false);
      }
      AbyssEngine::operator+(aSStack_58,aSStack_64);
      AbyssEngine::operator+(aSStack_4c,aSStack_58);
      AbyssEngine::String::operator=((String *)param_1,aSStack_4c);
      (*pcVar5)(aSStack_4c);
      (*pcVar5)(aSStack_58);
      (*pcVar5)(aSStack_70);
      (*pcVar5)(aSStack_64);
    }
    AbyssEngine::String::SubString((uint)aSStack_4c,(uint)aSStack_34);
    if (local_44 != 0) {
      AbyssEngine::operator+(aSStack_64,aSStack_4c);
      AbyssEngine::operator+(aSStack_58,aSStack_64);
      AbyssEngine::String::operator=((String *)param_1,aSStack_58);
      AbyssEngine::String::~String(aSStack_58);
      AbyssEngine::String::~String(aSStack_64);
    }
    AbyssEngine::String::~String(aSStack_4c);
  }
  if (0x7fffffff < in_r2) {
    AbyssEngine::String::String(aSStack_58,(char *)(DAT_000e34d4 + 0xe33f6),false);
    AbyssEngine::operator+(aSStack_4c,aSStack_58);
    AbyssEngine::String::operator=((String *)param_1,aSStack_4c);
    AbyssEngine::String::~String(aSStack_4c);
    AbyssEngine::String::~String(aSStack_58);
  }
  AbyssEngine::String::~String(aSStack_40);
  AbyssEngine::String::~String(aSStack_34);
  if (*piVar3 != local_28) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000e32b4: push {r4,r5,r6,r7,lr}
  000e32b6: add r7,sp,#0xc
  000e32b8: push {r8,r9,r10,r11}
  000e32bc: sub sp,#0x5c
  000e32be: str r0,[sp,#0xc]
  000e32c0: mov r1,r2
  000e32c2: ldr r0,[0x000e34c0]
  000e32c4: mov r5,r2
  000e32c6: cmp r2,#0x0
  000e32c8: add r0,pc
  000e32ca: ldr r4,[r0,#0x0]
  000e32cc: ldr r0,[r4,#0x0]
  000e32ce: str r0,[sp,#0x58]
  000e32d0: add r0,sp,#0x4c
  000e32d2: it mi
  000e32d4: rsb.mi r1,r5
  000e32d6: blx 0x0006f658
  000e32da: ldr.w r9,[sp,#0x54]
  000e32de: ldr r1,[0x000e34c4]
  000e32e0: add r1,pc
  000e32e2: ldr r0,[sp,#0xc]
  000e32e4: movs r2,#0x0
  000e32e6: blx 0x0006ee18
  000e32ea: blx 0x0006f544
  000e32ee: uxth r0,r0
  000e32f0: cmp r0,#0xb
  000e32f2: bhi 0x000e3310
  000e32f4: movs r1,#0x1
  000e32f6: lsl.w r0,r1,r0
  000e32fa: movw r1,#0xc01
  000e32fe: tst r0,r1
  000e3300: beq 0x000e3310
  000e3302: ldr r1,[0x000e34c8]
  000e3304: add r1,pc
  000e3306: add r0,sp,#0x40
  000e3308: movs r2,#0x0
  000e330a: blx 0x0006ee18
  000e330e: b 0x000e331c
  000e3310: ldr r1,[0x000e34cc]
  000e3312: add r1,pc
  000e3314: add r0,sp,#0x40
  000e3316: movs r2,#0x0
  000e3318: blx 0x0006ee18
  000e331c: cmp.w r9,#0x4
  000e3320: blt 0x000e33e0
  000e3322: strd r5,r4,[sp,#0x4]
  000e3326: add.w r8,sp,#0x10
  000e332a: ldr r0,[0x000e34d0]
  000e332c: add.w r10,sp,#0x28
  000e3330: ldr r6,[sp,#0xc]
  000e3332: add r0,pc
  000e3334: ldr.w r11,[r0,#0x0]
  000e3338: b 0x000e3394
  000e333a: sub.w r4,r9,#0x3
  000e333e: add r0,sp,#0x1c
  000e3340: add r1,sp,#0x4c
  000e3342: mov r2,r4
  000e3344: mov r3,r9
  000e3346: blx 0x0006f604
  000e334a: ldr r0,[r6,#0x8]
  000e334c: cmp r0,#0x2
  000e334e: bcc 0x000e335e
  000e3350: add r1,sp,#0x40
  000e3352: mov r0,r8
  000e3354: movs r2,#0x0
  000e3356: blx 0x0006f028
  000e335a: add r5,sp,#0x34
  000e335c: b 0x000e3366
  000e335e: mov r0,r8
  000e3360: blx 0x0006efbc
  000e3364: add r5,sp,#0x34
  000e3366: add r1,sp,#0x1c
  000e3368: mov r0,r10
  000e336a: mov r2,r8
  000e336c: blx 0x0006ef98
  000e3370: mov r0,r5
  000e3372: mov r1,r10
  000e3374: mov r2,r6
  000e3376: blx 0x0006ef98
  000e337a: mov r0,r6
  000e337c: mov r1,r5
  000e337e: blx 0x0006f2b0
  000e3382: mov r0,r5
  000e3384: blx r11
  000e3386: mov r0,r10
  000e3388: blx r11
  000e338a: mov r0,r8
  000e338c: blx r11
  000e338e: add r0,sp,#0x1c
  000e3390: blx r11
  000e3392: mov r9,r4
  000e3394: cmp.w r9,#0x3
  000e3398: bge 0x000e333a
  000e339a: add r0,sp,#0x34
  000e339c: add r1,sp,#0x4c
  000e339e: movs r2,#0x0
  000e33a0: mov r3,r9
  000e33a2: blx 0x0006f604
  000e33a6: ldr r0,[sp,#0x3c]
  000e33a8: ldr r6,[sp,#0xc]
  000e33aa: ldrd r5,r4,[sp,#0x4]
  000e33ae: cbz r0,0x000e33d8
  000e33b0: add r0,sp,#0x1c
  000e33b2: add r1,sp,#0x34
  000e33b4: add r2,sp,#0x40
  000e33b6: blx 0x0006ef98
  000e33ba: add r0,sp,#0x28
  000e33bc: add r1,sp,#0x1c
  000e33be: mov r2,r6
  000e33c0: blx 0x0006ef98
  000e33c4: add r1,sp,#0x28
  000e33c6: mov r0,r6
  000e33c8: blx 0x0006f2b0
  000e33cc: add r0,sp,#0x28
  000e33ce: blx 0x0006ee30
  000e33d2: add r0,sp,#0x1c
  000e33d4: blx 0x0006ee30
  000e33d8: add r0,sp,#0x34
  000e33da: blx 0x0006ee30
  000e33de: b 0x000e33ea
  000e33e0: ldr r6,[sp,#0xc]
  000e33e2: add r1,sp,#0x4c
  000e33e4: mov r0,r6
  000e33e6: blx 0x0006f2b0
  000e33ea: cmp.w r5,#0xffffffff
  000e33ee: bgt 0x000e341a
  000e33f0: ldr r1,[0x000e34d4]
  000e33f2: add r1,pc
  000e33f4: add r0,sp,#0x28
  000e33f6: movs r2,#0x0
  000e33f8: blx 0x0006ee18
  000e33fc: add r0,sp,#0x34
  000e33fe: add r1,sp,#0x28
  000e3400: mov r2,r6
  000e3402: blx 0x0006ef98
  000e3406: add r1,sp,#0x34
  000e3408: mov r0,r6
  000e340a: blx 0x0006f2b0
  000e340e: add r0,sp,#0x34
  000e3410: blx 0x0006ee30
  000e3414: add r0,sp,#0x28
  000e3416: blx 0x0006ee30
  000e341a: add r0,sp,#0x40
  000e341c: blx 0x0006ee30
  000e3420: add r0,sp,#0x4c
  000e3422: blx 0x0006ee30
  000e3426: ldr r0,[sp,#0x58]
  000e3428: ldr r1,[r4,#0x0]
  000e342a: subs r0,r1,r0
  000e342c: ittt eq
  000e342e: add.eq sp,#0x5c
  000e3430: pop.eq.w {r8,r9,r10,r11}
  000e3434: pop.eq {r4,r5,r6,r7,pc}
  000e3436: blx 0x0006e824
```
