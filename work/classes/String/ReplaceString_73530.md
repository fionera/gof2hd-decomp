# String::ReplaceString
elf 0x73530 body 208
Sig: undefined __stdcall ReplaceString(String param_1, String param_2)

## decompile
```c

/* AbyssEngine::String::ReplaceString(AbyssEngine::String, AbyssEngine::String) */

void AbyssEngine::String::ReplaceString(String *param_1,String *param_2)

{
  int iVar1;
  int *piVar2;
  uint uVar3;
  String aSStack_4c [12];
  String aSStack_40 [12];
  int local_34;
  ushort *local_30;
  int local_2c;
  int local_28;
  
  piVar2 = *(int **)(DAT_0008362c + 0x83546);
  local_28 = *piVar2;
  if (((*(int *)(param_1 + 4) != 0) && (*(int *)(param_1 + 8) != 0)) && (*(int *)(param_2 + 8) != 0)
     ) {
    uVar3 = 0;
    local_30 = (ushort *)0x0;
    local_2c = 0;
    local_34 = *(int *)(DAT_00083630 + 0x8356e) + 8;
    while (iVar1 = IndexOf(param_1,uVar3,param_2), iVar1 != -1) {
      SubString((uint)aSStack_4c,(uint)param_1);
      operator+(aSStack_40,aSStack_4c);
      operator+=((String *)&local_34,aSStack_40);
      ~String(aSStack_40);
      ~String(aSStack_4c);
      uVar3 = *(int *)(param_2 + 8) + iVar1;
    }
    if ((uVar3 != 0) && (uVar3 < *(uint *)(param_1 + 8))) {
      SubString((uint)aSStack_40,(uint)param_1);
      operator+=((String *)&local_34,aSStack_40);
      ~String(aSStack_40);
    }
    if (local_2c != 0) {
      Set(param_1,local_30);
    }
    ~String((String *)&local_34);
  }
  if (*piVar2 != local_28) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  00083530: push {r4,r5,r6,r7,lr}
  00083532: add r7,sp,#0xc
  00083534: push {r8,r9,r10,r11}
  00083538: sub sp,#0x2c
  0008353a: mov r4,r0
  0008353c: ldr r0,[0x0008362c]
  0008353e: mov r6,r1
  00083540: mov r8,r2
  00083542: add r0,pc
  00083544: ldr r1,[r0,#0x0]
  00083546: ldr r0,[r1,#0x0]
  00083548: str r0,[sp,#0x28]
  0008354a: ldr r0,[r4,#0x4]
  0008354c: cmp r0,#0x0
  0008354e: beq 0x000835ec
  00083550: ldr r0,[r4,#0x8]
  00083552: cmp r0,#0x0
  00083554: beq 0x000835ec
  00083556: ldr r0,[r6,#0x8]
  00083558: cmp r0,#0x0
  0008355a: beq 0x000835ec
  0008355c: str r1,[sp,#0x0]
  0008355e: add.w r11,sp,#0x4
  00083562: ldr r0,[0x00083630]
  00083564: add.w r9,sp,#0x10
  00083568: movs r5,#0x0
  0008356a: add r0,pc
  0008356c: strd r5,r5,[sp,#0x20]
  00083570: ldr r0,[r0,#0x0]
  00083572: adds r0,#0x8
  00083574: str r0,[sp,#0x1c]
  00083576: b 0x000835a8
  00083578: mov r0,r11
  0008357a: mov r1,r4
  0008357c: mov r2,r5
  0008357e: mov r3,r10
  00083580: blx 0x0006f604
  00083584: mov r0,r9
  00083586: mov r1,r11
  00083588: mov r2,r8
  0008358a: blx 0x0006ef98
  0008358e: add r0,sp,#0x1c
  00083590: mov r1,r9
  00083592: blx 0x0006ef5c
  00083596: mov r0,r9
  00083598: blx 0x0006f580
  0008359c: mov r0,r11
  0008359e: blx 0x0006f580
  000835a2: ldr r0,[r6,#0x8]
  000835a4: add.w r5,r0,r10
  000835a8: mov r0,r4
  000835aa: mov r1,r5
  000835ac: mov r2,r6
  000835ae: blx 0x0006f61c
  000835b2: mov r10,r0
  000835b4: adds r0,#0x1
  000835b6: bne 0x00083578
  000835b8: cbz r5,0x000835d8
  000835ba: ldr r3,[r4,#0x8]
  000835bc: cmp r5,r3
  000835be: bcs 0x000835d8
  000835c0: add r0,sp,#0x10
  000835c2: mov r1,r4
  000835c4: mov r2,r5
  000835c6: blx 0x0006f604
  000835ca: add r0,sp,#0x1c
  000835cc: add r1,sp,#0x10
  000835ce: blx 0x0006ef5c
  000835d2: add r0,sp,#0x10
  000835d4: blx 0x0006f580
  000835d8: ldr r0,[sp,#0x24]
  000835da: cbz r0,0x000835e4
  000835dc: ldr r1,[sp,#0x20]
  000835de: mov r0,r4
  000835e0: blx 0x0006f3f4
  000835e4: add r0,sp,#0x1c
  000835e6: blx 0x0006f580
  000835ea: ldr r1,[sp,#0x0]
  000835ec: ldr r0,[sp,#0x28]
  000835ee: ldr r1,[r1,#0x0]
  000835f0: subs r0,r1,r0
  000835f2: ittt eq
  000835f4: add.eq sp,#0x2c
  000835f6: pop.eq.w {r8,r9,r10,r11}
  000835fa: pop.eq {r4,r5,r6,r7,pc}
  000835fc: blx 0x0006e824
```
