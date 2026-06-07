# PaintCanvas::GetLine
elf 0x7c354 body 276
Sig: undefined __thiscall GetLine(PaintCanvas * this, uint param_1, String param_2, int param_3, String * param_4)

## decompile
```c

/* AbyssEngine::PaintCanvas::GetLine(unsigned int, AbyssEngine::String, int, AbyssEngine::String*)
    */

void __thiscall
AbyssEngine::PaintCanvas::GetLine
          (PaintCanvas *this,uint param_1,String *param_3,int param_4,String *param_5)

{
  short sVar1;
  int *piVar2;
  short *psVar3;
  int iVar4;
  uint uVar5;
  uint uVar6;
  int iVar7;
  String aSStack_34 [12];
  int local_28;
  
  uVar6 = 0;
  iVar7 = 5;
  piVar2 = *(int **)(DAT_0008c480 + 0x8c376);
  local_28 = *piVar2;
  uVar5 = 0;
  while( true ) {
    if (*(uint *)(param_3 + 8) <= uVar5) {
      if ((int)*(uint *)(param_3 + 8) < 2) {
        String::String(aSStack_34,(char *)(DAT_0008c484 + 0x8c428),false);
        String::operator=(param_5,aSStack_34);
      }
      else {
        String::SubString((uint)aSStack_34,(uint)param_3);
        String::operator=(param_5,aSStack_34);
      }
      goto LAB_0008c44c;
    }
    psVar3 = (short *)String::operator[](param_3,uVar5);
    sVar1 = *psVar3;
    iVar4 = GetTextWidth(this,param_1,param_3,uVar5,uVar5 + 1);
    iVar7 = iVar7 + iVar4;
    if (sVar1 == 0x20) {
      uVar6 = uVar5;
    }
    if (param_4 <= iVar7) break;
    psVar3 = (short *)String::operator[](param_3,uVar5);
    if ((*psVar3 == 10) ||
       (psVar3 = (short *)String::operator[](param_3,uVar5), uVar5 = uVar5 + 1, *psVar3 == 0xd)) {
      String::SubString((uint)aSStack_34,(uint)param_3);
      String::operator=(param_5,aSStack_34);
LAB_0008c44c:
      String::~String(aSStack_34);
      if (*piVar2 != local_28) {
                    /* WARNING: Subroutine does not return */
        __stack_chk_fail();
      }
      return;
    }
  }
  if ((int)uVar6 < 1) {
    String::SubString((uint)aSStack_34,(uint)param_3);
    String::operator=(param_5,aSStack_34);
  }
  else {
    String::SubString((uint)aSStack_34,(uint)param_3);
    String::operator=(param_5,aSStack_34);
  }
  goto LAB_0008c44c;
}

```

## target disasm
```
  0008c354: push {r4,r5,r6,r7,lr}
  0008c356: add r7,sp,#0xc
  0008c358: push {r8,r9,r10,r11}
  0008c35c: sub sp,#0x24
  0008c35e: str r3,[sp,#0x10]
  0008c360: mov r11,r0
  0008c362: str r1,[sp,#0xc]
  0008c364: mov r5,r2
  0008c366: ldr r0,[0x0008c480]
  0008c368: mov.w r8,#0x0
  0008c36c: mov.w r9,#0x5
  0008c370: movs r4,#0x0
  0008c372: add r0,pc
  0008c374: ldr r0,[r0,#0x0]
  0008c376: str r0,[sp,#0x8]
  0008c378: ldr r0,[r0,#0x0]
  0008c37a: str r0,[sp,#0x20]
  0008c37c: ldr r3,[r5,#0x8]
  0008c37e: cmp r4,r3
  0008c380: bcs 0x0008c3e4
  0008c382: mov r0,r5
  0008c384: mov r1,r4
  0008c386: blx 0x0006f3b8
  0008c38a: ldr r1,[sp,#0xc]
  0008c38c: mov r2,r5
  0008c38e: ldrh.w r10,[r0,#0x0]
  0008c392: mov r0,r11
  0008c394: mov r3,r4
  0008c396: adds r6,r4,#0x1
  0008c398: str r6,[sp,#0x0]
  0008c39a: blx 0x0006fdf0
  0008c39e: cmp.w r10,#0x20
  0008c3a2: add r9,r0
  0008c3a4: it eq
  0008c3a6: mov.eq r8,r4
  0008c3a8: ldr r0,[sp,#0x10]
  0008c3aa: cmp r9,r0
  0008c3ac: bge 0x0008c3fe
  0008c3ae: mov r0,r5
  0008c3b0: mov r1,r4
  0008c3b2: blx 0x0006f3b8
  0008c3b6: ldrh r0,[r0,#0x0]
  0008c3b8: cmp r0,#0xa
  0008c3ba: beq 0x0008c3cc
  0008c3bc: mov r0,r5
  0008c3be: mov r1,r4
  0008c3c0: blx 0x0006f3b8
  0008c3c4: ldrh r0,[r0,#0x0]
  0008c3c6: mov r4,r6
  0008c3c8: cmp r0,#0xd
  0008c3ca: bne 0x0008c37c
  0008c3cc: add r4,sp,#0x14
  0008c3ce: mov r1,r5
  0008c3d0: movs r2,#0x0
  0008c3d2: mov r3,r6
  0008c3d4: mov r0,r4
  0008c3d6: blx 0x0006f604
  0008c3da: ldr r0,[r7,#0x8]
  0008c3dc: mov r1,r4
  0008c3de: blx 0x0006f2b0
  0008c3e2: b 0x0008c44c
  0008c3e4: cmp r3,#0x2
  0008c3e6: blt 0x0008c41e
  0008c3e8: add r4,sp,#0x14
  0008c3ea: mov r1,r5
  0008c3ec: movs r2,#0x0
  0008c3ee: mov r0,r4
  0008c3f0: blx 0x0006f604
  0008c3f4: ldr r0,[r7,#0x8]
  0008c3f6: mov r1,r4
  0008c3f8: blx 0x0006f2b0
  0008c3fc: b 0x0008c44c
  0008c3fe: cmp.w r8,#0x1
  0008c402: blt 0x0008c436
  0008c404: add r4,sp,#0x14
  0008c406: add.w r3,r8,#0x1
  0008c40a: mov r1,r5
  0008c40c: movs r2,#0x0
  0008c40e: mov r0,r4
  0008c410: blx 0x0006f604
  0008c414: ldr r0,[r7,#0x8]
  0008c416: mov r1,r4
  0008c418: blx 0x0006f2b0
  0008c41c: b 0x0008c44c
  0008c41e: ldr r1,[0x0008c484]
  0008c420: add r4,sp,#0x14
  0008c422: movs r2,#0x0
  0008c424: add r1,pc
  0008c426: mov r0,r4
  0008c428: blx 0x0006ee18
  0008c42c: ldr r0,[r7,#0x8]
  0008c42e: mov r1,r4
  0008c430: blx 0x0006f2b0
  0008c434: b 0x0008c44c
  0008c436: add r4,sp,#0x14
  0008c438: mov r1,r5
  0008c43a: movs r2,#0x0
  0008c43c: mov r3,r6
  0008c43e: mov r0,r4
  0008c440: blx 0x0006f604
  0008c444: ldr r0,[r7,#0x8]
  0008c446: mov r1,r4
  0008c448: blx 0x0006f2b0
  0008c44c: add r0,sp,#0x14
  0008c44e: blx 0x0006ee30
  0008c452: ldr r1,[sp,#0x8]
  0008c454: ldr r0,[sp,#0x20]
  0008c456: ldr r1,[r1,#0x0]
  0008c458: subs r0,r1,r0
  0008c45a: ittt eq
  0008c45c: add.eq sp,#0x24
  0008c45e: pop.eq.w {r8,r9,r10,r11}
  0008c462: pop.eq {r4,r5,r6,r7,pc}
  0008c464: blx 0x0006e824
```
