# ConfigReader::ParseFile
elf 0x876bc body 236
Sig: undefined __thiscall ParseFile(ConfigReader * this, String param_1)

## decompile
```c

/* AbyssEngine::ConfigReader::ParseFile(AbyssEngine::String) */

void __thiscall AbyssEngine::ConfigReader::ParseFile(ConfigReader *this,String *param_2)

{
  int iVar1;
  char *pcVar2;
  short *psVar3;
  uint uVar4;
  String *this_00;
  int *piVar5;
  String local_40 [12];
  String aSStack_34 [8];
  int local_2c;
  int local_28;
  
  piVar5 = *(int **)(DAT_000977cc + 0x976d2);
  local_28 = *piVar5;
  iVar1 = AEFile::OpenRead(param_2,(uint *)(this + 0x10));
  if (iVar1 != 0) {
    GetNewLine();
    pcVar2 = (char *)(DAT_000977d0 + 0x976fe);
    while (iVar1 = String::Compare(aSStack_34,pcVar2), iVar1 != 0) {
      psVar3 = (short *)String::operator[](aSStack_34,0);
      if ((*psVar3 == 0x5b) &&
         (psVar3 = (short *)String::operator[](aSStack_34,local_2c + -1), *psVar3 == 0x5d)) {
        for (uVar4 = 0; uVar4 < *(uint *)this; uVar4 = uVar4 + 1) {
          this_00 = *(String **)(*(int *)(this + 4) + uVar4 * 4);
          String::SubString((uint)local_40,(uint)aSStack_34);
          iVar1 = String::Compare(this_00,local_40);
          String::~String(local_40);
          if (iVar1 == 0) {
            iVar1 = *(int *)(*(int *)(this + 4) + uVar4 * 4);
            (**(code **)(iVar1 + 0xc))(this,*(undefined4 *)(iVar1 + 0x10));
            break;
          }
        }
      }
      GetNewLine();
      String::operator=(aSStack_34,local_40);
      String::~String(local_40);
    }
    AEFile::Close(*(uint *)(this + 0x10));
    String::~String(aSStack_34);
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
  000976bc: push {r4,r5,r6,r7,lr}
  000976be: add r7,sp,#0xc
  000976c0: push {r8,r9,r10,r11}
  000976c4: sub sp,#0x24
  000976c6: mov r4,r0
  000976c8: ldr r0,[0x000977cc]
  000976ca: add.w r5,r4,#0x10
  000976ce: add r0,pc
  000976d0: ldr.w r10,[r0,#0x0]
  000976d4: ldr.w r0,[r10,#0x0]
  000976d8: str r0,[sp,#0x20]
  000976da: mov r0,r1
  000976dc: mov r1,r5
  000976de: blx 0x0006f34c
  000976e2: cmp r0,#0x0
  000976e4: beq 0x00097792
  000976e6: add.w r9,sp,#0x14
  000976ea: mov r1,r4
  000976ec: str r5,[sp,#0x4]
  000976ee: mov r0,r9
  000976f0: blx 0x000708ac
  000976f4: ldr r0,[0x000977d0]
  000976f6: add.w r11,sp,#0x8
  000976fa: add r0,pc
  000976fc: mov r8,r0
  000976fe: b 0x00097778
  00097700: mov r0,r9
  00097702: movs r1,#0x0
  00097704: blx 0x0006f3b8
  00097708: ldrh r0,[r0,#0x0]
  0009770a: cmp r0,#0x5b
  0009770c: bne 0x00097762
  0009770e: ldr r0,[sp,#0x1c]
  00097710: subs r1,r0,#0x1
  00097712: mov r0,r9
  00097714: blx 0x0006f3b8
  00097718: ldrh r0,[r0,#0x0]
  0009771a: cmp r0,#0x5d
  0009771c: bne 0x00097762
  0009771e: movs r5,#0x0
  00097720: b 0x00097724
  00097722: adds r5,#0x1
  00097724: ldr r0,[r4,#0x0]
  00097726: cmp r5,r0
  00097728: bcs 0x00097762
  0009772a: ldr r0,[r4,#0x4]
  0009772c: ldr.w r6,[r0,r5,lsl #0x2]
  00097730: ldrh.w r0,[sp,#0x1c]
  00097734: subs r3,r0,#0x1
  00097736: mov r0,r11
  00097738: mov r1,r9
  0009773a: movs r2,#0x1
  0009773c: blx 0x0006f604
  00097740: mov r0,r6
  00097742: mov r1,r11
  00097744: blx 0x0006f664
  00097748: mov r6,r0
  0009774a: mov r0,r11
  0009774c: blx 0x0006ee30
  00097750: cmp r6,#0x0
  00097752: bne 0x00097722
  00097754: ldr r0,[r4,#0x4]
  00097756: ldr.w r0,[r0,r5,lsl #0x2]
  0009775a: ldrd r2,r1,[r0,#0xc]
  0009775e: mov r0,r4
  00097760: blx r2
  00097762: mov r0,r11
  00097764: mov r1,r4
  00097766: blx 0x000708ac
  0009776a: mov r0,r9
  0009776c: mov r1,r11
  0009776e: blx 0x0006f2b0
  00097772: mov r0,r11
  00097774: blx 0x0006ee30
  00097778: mov r0,r9
  0009777a: mov r1,r8
  0009777c: blx 0x0006fa3c
  00097780: cmp r0,#0x0
  00097782: bne 0x00097700
  00097784: ldr r0,[sp,#0x4]
  00097786: ldr r0,[r0,#0x0]
  00097788: blx 0x0006ece0
  0009778c: add r0,sp,#0x14
  0009778e: blx 0x0006ee30
  00097792: ldr r0,[sp,#0x20]
  00097794: ldr.w r1,[r10,#0x0]
  00097798: subs r0,r1,r0
  0009779a: ittt eq
  0009779c: add.eq sp,#0x24
  0009779e: pop.eq.w {r8,r9,r10,r11}
  000977a2: pop.eq {r4,r5,r6,r7,pc}
  000977a4: blx 0x0006e824
```
