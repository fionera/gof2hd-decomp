# ConfigReader::GetNewLine
elf 0x877d4 body 228
Sig: undefined __stdcall GetNewLine(void)

## decompile
```c

/* AbyssEngine::ConfigReader::GetNewLine() */

void AbyssEngine::ConfigReader::GetNewLine(void)

{
  String *in_r0;
  int iVar1;
  char *pcVar2;
  char *pcVar3;
  int iVar4;
  int iVar5;
  int in_r1;
  int *piVar6;
  String aSStack_38 [4];
  
  piVar6 = *(int **)(DAT_000978dc + 0x977e6);
  iVar1 = *piVar6;
  String::String(in_r0);
  pcVar2 = (char *)(DAT_000978e0 + 0x97808);
  pcVar3 = (char *)(DAT_000978e4 + 0x9780e);
  while (*(int *)(in_r0 + 8) == 0) {
    while (iVar4 = AEFile::Read(&stack0xffffffd7,*(uint *)(in_r1 + 0x10)), iVar4 == 1) {
      String::operator+=(in_r0,&stack0xffffffd7);
    }
    String::Trim(in_r0);
    String::String(aSStack_38,pcVar2,false);
    iVar5 = String::IndexOf(in_r0);
    String::~String(aSStack_38);
    if (iVar5 != -1) {
      String::SubString((uint)aSStack_38,(uint)in_r0);
      String::operator=(in_r0,aSStack_38);
      String::~String(aSStack_38);
      String::Trim(in_r0);
    }
    if (*(int *)(in_r0 + 8) == 0 && iVar4 == 0) {
      String::Set(in_r0,pcVar3);
    }
  }
  if (*piVar6 == iVar1) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```

## target disasm
```
  000977d4: push {r4,r5,r6,r7,lr}
  000977d6: add r7,sp,#0xc
  000977d8: push {r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11}
  000977dc: mov r9,r0
  000977de: ldr r0,[0x000978dc]
  000977e0: mov r5,r1
  000977e2: add r0,pc
  000977e4: ldr.w r11,[r0,#0x0]
  000977e8: ldr.w r0,[r11,#0x0]
  000977ec: str r0,[sp,#0x18]
  000977ee: mov r0,r9
  000977f0: blx 0x0006efbc
  000977f4: movs r0,#0x0
  000977f6: sub.w r6,r7,#0x21
  000977fa: strb.w r0,[r7,#-0x21]
  000977fe: add.w r8,sp,#0x8
  00097802: ldr r0,[0x000978e0]
  00097804: add r0,pc
  00097806: str r0,[sp,#0x4]
  00097808: ldr r0,[0x000978e4]
  0009780a: add r0,pc
  0009780c: str r0,[sp,#0x0]
  0009780e: ldr.w r0,[r9,#0x8]
  00097812: cmp r0,#0x0
  00097814: bne 0x000978a2
  00097816: ldr r1,[r5,#0x10]
  00097818: mov r0,r6
  0009781a: blx 0x0007090c
  0009781e: mov r4,r0
  00097820: ldrb.w r0,[r7,#-0x21]
  00097824: eor r2,r4,#0x1
  00097828: sub.w r1,r0,#0xa
  0009782c: clz r1,r1
  00097830: lsrs r1,r1,#0x5
  00097832: orrs r1,r2
  00097834: bne 0x00097844
  00097836: cmp r0,#0xd
  00097838: beq 0x00097816
  0009783a: mov r0,r9
  0009783c: mov r1,r6
  0009783e: blx 0x00070918
  00097842: b 0x00097816
  00097844: mov r0,r9
  00097846: blx 0x00070924
  0009784a: ldr r1,[sp,#0x4]
  0009784c: mov r0,r8
  0009784e: movs r2,#0x0
  00097850: blx 0x0006ee18
  00097854: mov r0,r9
  00097856: mov r1,r8
  00097858: blx 0x0006f3a0
  0009785c: mov r10,r0
  0009785e: mov r0,r8
  00097860: blx 0x0006ee30
  00097864: adds.w r0,r10,#0x1
  00097868: beq 0x0009788a
  0009786a: mov r0,r8
  0009786c: mov r1,r9
  0009786e: movs r2,#0x0
  00097870: mov r3,r10
  00097872: blx 0x0006f604
  00097876: mov r0,r9
  00097878: mov r1,r8
  0009787a: blx 0x0006f2b0
  0009787e: mov r0,r8
  00097880: blx 0x0006ee30
  00097884: mov r0,r9
  00097886: blx 0x00070924
  0009788a: ldr.w r0,[r9,#0x8]
  0009788e: cmp r0,#0x0
  00097890: it ne
  00097892: mov.ne r0,#0x1
  00097894: orrs r0,r4
  00097896: bne 0x0009780e
  00097898: ldr r1,[sp,#0x0]
  0009789a: mov r0,r9
  0009789c: blx 0x0006f2bc
  000978a0: b 0x0009780e
  000978a2: ldr r0,[sp,#0x18]
  000978a4: ldr.w r1,[r11,#0x0]
  000978a8: subs r0,r1,r0
  000978aa: ittt eq
  000978ac: add.eq sp,#0x1c
  000978ae: pop.eq.w {r8,r9,r10,r11}
  000978b2: pop.eq {r4,r5,r6,r7,pc}
  000978b4: blx 0x0006e824
```
