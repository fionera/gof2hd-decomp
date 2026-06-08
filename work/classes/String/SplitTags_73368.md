# String::SplitTags
elf 0x73368 body 366
Sig: undefined __thiscall SplitTags(String * this, String param_1)

## decompile
```c

/* AbyssEngine::String::SplitTags(AbyssEngine::String) */

void __thiscall AbyssEngine::String::SplitTags(String *this,String *param_2)

{
  Array *this_00;
  char *pcVar1;
  uint uVar2;
  String *pSVar3;
  void *pvVar4;
  code *pcVar5;
  uint uVar6;
  int *piVar7;
  int iVar8;
  String aSStack_58 [12];
  String aSStack_4c [12];
  String aSStack_40 [12];
  String aSStack_34 [12];
  int local_28;
  
  piVar7 = *(int **)(DAT_0008351c + 0x8337c);
  local_28 = *piVar7;
  if (((*(int *)(this + 4) != 0) && (*(int *)(this + 8) != 0)) && (*(int *)(param_2 + 8) != 0)) {
    String(aSStack_4c,(char *)(DAT_00083520 + 0x833a8),false);
    operator+(aSStack_40,aSStack_4c);
    String(aSStack_58,(char *)(DAT_00083524 + 0x833bc),false);
    operator+(aSStack_34,aSStack_40);
    operator=(param_2,aSStack_34);
    pcVar5 = *(code **)(DAT_00083528 + 0x833dc);
    (*pcVar5)(aSStack_34);
    (*pcVar5)(aSStack_58);
    (*pcVar5)(aSStack_40);
    (*pcVar5)(aSStack_4c);
    this_00 = operator_new(0xc);
    Array<AbyssEngine::String*>::Array();
    iVar8 = 0;
    uVar6 = 0;
    pcVar1 = (char *)(DAT_0008352c + 0x83406);
    while (uVar2 = IndexOf(this,uVar6,param_2), uVar2 != 0xffffffff) {
      if (uVar6 <= uVar2) {
        pSVar3 = operator_new(0xc);
        SubString((uint)pSVar3,(uint)this);
        ArrayAdd<AbyssEngine::String*>(pSVar3,this_00);
        iVar8 = *(int *)(param_2 + 8);
        String(aSStack_34,pcVar1,false);
        iVar8 = IndexOf(this,iVar8 + uVar2,aSStack_34);
        ~String(aSStack_34);
        if (iVar8 == -1) goto LAB_000834bc;
        pSVar3 = operator_new(0xc);
        SubString((uint)pSVar3,(uint)this);
        ArrayAdd<AbyssEngine::String*>(pSVar3,this_00);
      }
      uVar6 = iVar8 + 1;
    }
    if ((uVar6 != 0) && (uVar6 < *(uint *)(this + 8))) {
      pSVar3 = operator_new(0xc);
      SubString((uint)pSVar3,(uint)this);
      ArrayAdd<AbyssEngine::String*>(pSVar3,this_00);
    }
    if (*(int *)this_00 == 0) {
      ArrayRemoveAll<AbyssEngine::String*>(this_00);
      pvVar4 = (void *)Array<AbyssEngine::String*>::~Array((Array<AbyssEngine::String*> *)this_00);
      operator_delete(pvVar4);
    }
  }
LAB_000834bc:
  iVar8 = *piVar7 - local_28;
  if (iVar8 == 0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar8);
}

```

## target disasm
```
  00083368: push {r4,r5,r6,r7,lr}
  0008336a: add r7,sp,#0xc
  0008336c: push {r8,r9,r10,r11}
  00083370: sub sp,#0x3c
  00083372: mov r11,r0
  00083374: ldr r0,[0x0008351c]
  00083376: mov r10,r1
  00083378: add r0,pc
  0008337a: ldr r5,[r0,#0x0]
  0008337c: ldr r0,[r5,#0x0]
  0008337e: str r0,[sp,#0x38]
  00083380: ldr.w r0,[r11,#0x4]
  00083384: cmp r0,#0x0
  00083386: beq.w 0x000834b8
  0008338a: ldr.w r0,[r11,#0x8]
  0008338e: cmp r0,#0x0
  00083390: beq.w 0x000834b8
  00083394: ldr.w r0,[r10,#0x8]
  00083398: cmp r0,#0x0
  0008339a: beq.w 0x000834b8
  0008339e: ldr r1,[0x00083520]
  000833a0: add r4,sp,#0x14
  000833a2: movs r2,#0x0
  000833a4: add r1,pc
  000833a6: mov r0,r4
  000833a8: blx 0x0006f610
  000833ac: add r0,sp,#0x20
  000833ae: mov r1,r4
  000833b0: mov r2,r10
  000833b2: blx 0x0006ef98
  000833b6: ldr r1,[0x00083524]
  000833b8: add r1,pc
  000833ba: add r0,sp,#0x8
  000833bc: movs r2,#0x0
  000833be: blx 0x0006f610
  000833c2: add r0,sp,#0x2c
  000833c4: add r1,sp,#0x20
  000833c6: add r2,sp,#0x8
  000833c8: blx 0x0006ef98
  000833cc: add r1,sp,#0x2c
  000833ce: mov r0,r10
  000833d0: blx 0x0006f2b0
  000833d4: str r5,[sp,#0x0]
  000833d6: ldr r0,[0x00083528]
  000833d8: add r0,pc
  000833da: ldr r4,[r0,#0x0]
  000833dc: add r0,sp,#0x2c
  000833de: blx r4
  000833e0: add r0,sp,#0x8
  000833e2: blx r4
  000833e4: add r0,sp,#0x20
  000833e6: blx r4
  000833e8: add r0,sp,#0x14
  000833ea: blx r4
  000833ec: movs r0,#0xc
  000833ee: blx 0x0006eb24
  000833f2: mov r9,r0
  000833f4: blx 0x0006f628
  000833f8: ldr r0,[0x0008352c]
  000833fa: add.w r8,sp,#0x2c
  000833fe: movs r6,#0x0
  00083400: movs r4,#0x0
  00083402: add r0,pc
  00083404: str r0,[sp,#0x4]
  00083406: b 0x0008346c
  00083408: cmp r4,r5
  0008340a: bhi 0x0008346a
  0008340c: movs r0,#0xc
  0008340e: blx 0x0006eb24
  00083412: mov r6,r0
  00083414: mov r1,r11
  00083416: mov r2,r4
  00083418: mov r3,r5
  0008341a: blx 0x0006f604
  0008341e: mov r0,r6
  00083420: mov r1,r9
  00083422: blx 0x0006f634
  00083426: ldr r1,[sp,#0x4]
  00083428: mov r0,r8
  0008342a: movs r2,#0x0
  0008342c: ldr.w r4,[r10,#0x8]
  00083430: blx 0x0006f610
  00083434: adds r1,r4,r5
  00083436: mov r0,r11
  00083438: mov r2,r8
  0008343a: blx 0x0006f61c
  0008343e: mov r6,r0
  00083440: mov r0,r8
  00083442: blx 0x0006f580
  00083446: adds r0,r6,#0x1
  00083448: beq 0x000834d2
  0008344a: movs r0,#0xc
  0008344c: blx 0x0006eb24
  00083450: mov r4,r0
  00083452: ldr.w r0,[r10,#0x8]
  00083456: adds r2,r0,r5
  00083458: mov r0,r4
  0008345a: mov r1,r11
  0008345c: mov r3,r6
  0008345e: blx 0x0006f604
  00083462: mov r0,r4
  00083464: mov r1,r9
  00083466: blx 0x0006f634
  0008346a: adds r4,r6,#0x1
  0008346c: mov r0,r11
  0008346e: mov r1,r4
  00083470: mov r2,r10
  00083472: blx 0x0006f61c
  00083476: mov r5,r0
  00083478: adds r0,#0x1
  0008347a: bne 0x00083408
  0008347c: cbz r4,0x000834a0
  0008347e: ldr.w r5,[r11,#0x8]
  00083482: cmp r4,r5
  00083484: bcs 0x000834a0
  00083486: movs r0,#0xc
  00083488: blx 0x0006eb24
  0008348c: mov r6,r0
  0008348e: mov r1,r11
  00083490: mov r2,r4
  00083492: mov r3,r5
  00083494: blx 0x0006f604
  00083498: mov r0,r6
  0008349a: mov r1,r9
  0008349c: blx 0x0006f634
  000834a0: ldr.w r0,[r9,#0x0]
  000834a4: ldr r5,[sp,#0x0]
  000834a6: cbnz r0,0x000834bc
  000834a8: mov r0,r9
  000834aa: blx 0x0006f640
  000834ae: mov r0,r9
  000834b0: blx 0x0006f64c
  000834b4: blx 0x0006eb48
  000834b8: mov.w r9,#0x0
  000834bc: ldr r0,[sp,#0x38]
  000834be: ldr r1,[r5,#0x0]
  000834c0: subs r0,r1,r0
  000834c2: itttt eq
  000834c4: mov.eq r0,r9
  000834c6: add.eq sp,#0x3c
  000834c8: pop.eq.w {r8,r9,r10,r11}
  000834cc: pop.eq {r4,r5,r6,r7,pc}
  000834ce: blx 0x0006e824
  000834d2: ldr r5,[sp,#0x0]
  000834d4: b 0x000834bc
```
