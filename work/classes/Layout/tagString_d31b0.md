# Layout::tagString
elf 0xd31b0 body 162
Sig: undefined __stdcall tagString(String param_1)

## decompile
```c

/* Layout::tagString(AbyssEngine::String) */

void Layout::tagString(String *param_1)

{
  code *pcVar1;
  int *piVar2;
  String aSStack_70 [12];
  String aSStack_64 [12];
  String aSStack_58 [12];
  String aSStack_4c [12];
  String aSStack_40 [12];
  String aSStack_34 [12];
  String aSStack_28 [12];
  int local_1c;
  
  piVar2 = *(int **)(DAT_000e32a0 + 0xe31c6);
  local_1c = *piVar2;
  AbyssEngine::String::String(aSStack_4c,(char *)(DAT_000e32a4 + 0xe31d2),false);
  AbyssEngine::String::String(aSStack_58,DAT_000e329c);
  AbyssEngine::operator+(aSStack_40,aSStack_4c);
  AbyssEngine::String::String(aSStack_64,(char *)(DAT_000e32a8 + 0xe31ec),false);
  AbyssEngine::operator+(aSStack_34,aSStack_40);
  AbyssEngine::operator+(aSStack_28,aSStack_34);
  AbyssEngine::String::String(aSStack_70,(char *)(DAT_000e32ac + 0xe320c),false);
  AbyssEngine::operator+(param_1,aSStack_28);
  pcVar1 = *(code **)(DAT_000e32b0 + 0xe3222);
  (*pcVar1)(aSStack_70);
  (*pcVar1)(aSStack_28);
  (*pcVar1)(aSStack_34);
  (*pcVar1)(aSStack_64);
  (*pcVar1)(aSStack_40);
  (*pcVar1)(aSStack_58);
  (*pcVar1)(aSStack_4c);
  if (*piVar2 != local_1c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000e31b0: push {r4,r5,r6,r7,lr}
  000e31b2: add r7,sp,#0xc
  000e31b4: push.w r11
  000e31b8: sub sp,#0x58
  000e31ba: mov r4,r0
  000e31bc: ldr r0,[0x000e32a0]
  000e31be: mov r5,r2
  000e31c0: movs r2,#0x0
  000e31c2: add r0,pc
  000e31c4: ldr r6,[r0,#0x0]
  000e31c6: ldr r0,[r6,#0x0]
  000e31c8: ldr r1,[0x000e32a4]
  000e31ca: str r0,[sp,#0x54]
  000e31cc: add r0,sp,#0x24
  000e31ce: add r1,pc
  000e31d0: blx 0x0006ee18
  000e31d4: ldr r1,[0x000e329c]
  000e31d6: add r0,sp,#0x18
  000e31d8: blx 0x0006f658
  000e31dc: add r0,sp,#0x30
  000e31de: add r1,sp,#0x24
  000e31e0: add r2,sp,#0x18
  000e31e2: blx 0x0006ef98
  000e31e6: ldr r1,[0x000e32a8]
  000e31e8: add r1,pc
  000e31ea: add r0,sp,#0xc
  000e31ec: movs r2,#0x0
  000e31ee: blx 0x0006ee18
  000e31f2: add r0,sp,#0x3c
  000e31f4: add r1,sp,#0x30
  000e31f6: add r2,sp,#0xc
  000e31f8: blx 0x0006ef98
  000e31fc: add r0,sp,#0x48
  000e31fe: add r1,sp,#0x3c
  000e3200: mov r2,r5
  000e3202: blx 0x0006ef98
  000e3206: ldr r1,[0x000e32ac]
  000e3208: add r1,pc
  000e320a: mov r0,sp
  000e320c: movs r2,#0x0
  000e320e: blx 0x0006ee18
  000e3212: add r1,sp,#0x48
  000e3214: mov r2,sp
  000e3216: mov r0,r4
  000e3218: blx 0x0006ef98
  000e321c: ldr r0,[0x000e32b0]
  000e321e: add r0,pc
  000e3220: ldr r4,[r0,#0x0]
  000e3222: mov r0,sp
  000e3224: blx r4
  000e3226: add r0,sp,#0x48
  000e3228: blx r4
  000e322a: add r0,sp,#0x3c
  000e322c: blx r4
  000e322e: add r0,sp,#0xc
  000e3230: blx r4
  000e3232: add r0,sp,#0x30
  000e3234: blx r4
  000e3236: add r0,sp,#0x18
  000e3238: blx r4
  000e323a: add r0,sp,#0x24
  000e323c: blx r4
  000e323e: ldr r0,[sp,#0x54]
  000e3240: ldr r1,[r6,#0x0]
  000e3242: subs r0,r1,r0
  000e3244: ittt eq
  000e3246: add.eq sp,#0x58
  000e3248: pop.eq.w r11
  000e324c: pop.eq {r4,r5,r6,r7,pc}
  000e324e: blx 0x0006e824
```
