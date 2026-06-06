# AEFile::collectPakFiles
elf 0x70668 body 194
Sig: undefined __stdcall collectPakFiles(String * param_1)

## decompile
```c

/* AEFile::collectPakFiles(AbyssEngine::String const&) */

void AEFile::collectPakFiles(String *param_1)

{
  void *pvVar1;
  int iVar2;
  char *pcVar3;
  int *piVar4;
  int *piVar5;
  String aSStack_4c [12];
  String aSStack_40 [12];
  String aSStack_34 [12];
  int local_28;
  
  piVar4 = *(int **)(DAT_00080750 + 0x8067a);
  piVar5 = *(int **)(DAT_00080754 + 0x8067c);
  local_28 = *piVar5;
  if ((*piVar4 != 0) && (*(char *)(*piVar4 + 4) != '\0')) {
    pvVar1 = (void *)AbyssEngine::String::GetAEChar();
    iVar2 = (**(code **)(*(int *)*piVar4 + 0x3c))((int *)*piVar4,pvVar1,0);
    if (iVar2 != 0) {
      AbyssEngine::String::String(aSStack_34);
      pcVar3 = (char *)(DAT_00080758 + 0x806be);
      while (iVar2 = (**(code **)(*(int *)*piVar4 + 0x40))((int *)*piVar4,aSStack_34), iVar2 != 0) {
        AbyssEngine::String::String(aSStack_40,aSStack_34,false);
        AbyssEngine::String::String(aSStack_4c,pcVar3,false);
        iVar2 = AbyssEngine::String::IndexOf(aSStack_34);
        AbyssEngine::String::~String(aSStack_4c);
        if (iVar2 != -1) {
          collectFilesInPakFiles(aSStack_40);
        }
        AbyssEngine::String::~String(aSStack_40);
      }
      AbyssEngine::String::~String(aSStack_34);
    }
    operator_delete__(pvVar1);
    sortPakFileEntryList();
  }
  if (*piVar5 != local_28) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```
## target disasm
```
  00080668: push {r4,r5,r6,r7,lr}
  0008066a: add r7,sp,#0xc
  0008066c: push {r8,r9,r10,r11}
  00080670: sub sp,#0x2c
  00080672: ldr r1,[0x00080750]
  00080674: ldr r2,[0x00080754]
  00080676: add r1,pc
  00080678: add r2,pc
  0008067a: ldr r4,[r1,#0x0]
  0008067c: ldr.w r11,[r2,#0x0]
  00080680: ldr r1,[r4,#0x0]
  00080682: ldr.w r2,[r11,#0x0]
  00080686: cmp r1,#0x0
  00080688: str r2,[sp,#0x28]
  0008068a: beq 0x00080714
  0008068c: ldrb r1,[r1,#0x4]
  0008068e: cmp r1,#0x0
  00080690: beq 0x00080714
  00080692: blx 0x0006ef68
  00080696: mov r5,r0
  00080698: ldr r0,[r4,#0x0]
  0008069a: movs r2,#0x0
  0008069c: ldr r1,[r0,#0x0]
  0008069e: ldr r3,[r1,#0x3c]
  000806a0: mov r1,r5
  000806a2: blx r3
  000806a4: cbz r0,0x0008070a
  000806a6: str r5,[sp,#0x0]
  000806a8: add r5,sp,#0x1c
  000806aa: mov r0,r5
  000806ac: blx 0x0006efbc
  000806b0: ldr r0,[0x00080758]
  000806b2: add.w r9,sp,#0x10
  000806b6: add.w r10,sp,#0x4
  000806ba: add r0,pc
  000806bc: mov r8,r0
  000806be: b 0x000806f4
  000806c0: mov r0,r9
  000806c2: mov r1,r5
  000806c4: movs r2,#0x0
  000806c6: blx 0x0006f028
  000806ca: mov r0,r10
  000806cc: mov r1,r8
  000806ce: movs r2,#0x0
  000806d0: blx 0x0006ee18
  000806d4: mov r0,r5
  000806d6: mov r1,r10
  000806d8: blx 0x0006f3a0
  000806dc: mov r6,r0
  000806de: mov r0,r10
  000806e0: blx 0x0006ee30
  000806e4: adds r0,r6,#0x1
  000806e6: beq 0x000806ee
  000806e8: mov r0,r9
  000806ea: blx 0x0006f388
  000806ee: mov r0,r9
  000806f0: blx 0x0006ee30
  000806f4: ldr r0,[r4,#0x0]
  000806f6: ldr r1,[r0,#0x0]
  000806f8: ldr r2,[r1,#0x40]
  000806fa: mov r1,r5
  000806fc: blx r2
  000806fe: cmp r0,#0x0
  00080700: bne 0x000806c0
  00080702: add r0,sp,#0x1c
  00080704: blx 0x0006ee30
  00080708: ldr r5,[sp,#0x0]
  0008070a: mov r0,r5
  0008070c: blx 0x0006ebfc
  00080710: blx 0x0006f394
  00080714: ldr r0,[sp,#0x28]
  00080716: ldr.w r1,[r11,#0x0]
  0008071a: subs r0,r1,r0
  0008071c: ittt eq
  0008071e: add.eq sp,#0x2c
  00080720: pop.eq.w {r8,r9,r10,r11}
  00080724: pop.eq {r4,r5,r6,r7,pc}
  00080726: blx 0x0006e824
```
