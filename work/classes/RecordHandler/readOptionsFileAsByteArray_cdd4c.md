# RecordHandler::readOptionsFileAsByteArray
elf 0xcdd4c body 116
Sig: undefined __thiscall readOptionsFileAsByteArray(RecordHandler * this, signed * * param_1)

## decompile
```c

/* RecordHandler::readOptionsFileAsByteArray(signed char**) */

void __thiscall RecordHandler::readOptionsFileAsByteArray(RecordHandler *this,signed **param_1)

{
  String *pSVar1;
  int iVar2;
  uint uVar3;
  signed *psVar4;
  int *piVar5;
  uint local_2c;
  String aSStack_28 [12];
  int local_1c;
  
  piVar5 = *(int **)(DAT_000dddd4 + 0xddd60);
  local_1c = *piVar5;
  pSVar1 = (String *)AbyssEngine::String::String(aSStack_28,this + 8,false);
  iVar2 = AEFile::FileExist(pSVar1);
  if (iVar2 != 0) {
    AEFile::OpenRead(aSStack_28,&local_2c);
    uVar3 = AEFile::GetFileSize(local_2c);
    psVar4 = operator_new__(uVar3 | (int)uVar3 >> 0x1f);
    *param_1 = psVar4;
    AEFile::Read(uVar3,psVar4,local_2c);
    AEFile::Close(local_2c);
  }
  AbyssEngine::String::~String(aSStack_28);
  iVar2 = *piVar5 - local_1c;
  if (iVar2 == 0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar2);
}

```

## target disasm
```
  000ddd4c: push {r4,r5,r6,r7,lr}
  000ddd4e: add r7,sp,#0xc
  000ddd50: push.w r11
  000ddd54: sub sp,#0x18
  000ddd56: mov r4,r1
  000ddd58: ldr r1,[0x000dddd4]
  000ddd5a: movs r2,#0x0
  000ddd5c: add r1,pc
  000ddd5e: ldr r6,[r1,#0x0]
  000ddd60: ldr r1,[r6,#0x0]
  000ddd62: str r1,[sp,#0x14]
  000ddd64: add.w r1,r0,#0x8
  000ddd68: add r0,sp,#0x8
  000ddd6a: blx 0x0006f028
  000ddd6e: blx 0x0006fff4
  000ddd72: cbz r0,0x000ddda0
  000ddd74: add r0,sp,#0x8
  000ddd76: add r1,sp,#0x4
  000ddd78: blx 0x0006f34c
  000ddd7c: ldr r0,[sp,#0x4]
  000ddd7e: blx 0x00071044
  000ddd82: mov r5,r0
  000ddd84: orr.w r0,r0,r0, asr #0x1f
  000ddd88: blx 0x0006ec08
  000ddd8c: mov r1,r0
  000ddd8e: str r1,[r4,#0x0]
  000ddd90: ldr r2,[sp,#0x4]
  000ddd92: mov r0,r5
  000ddd94: blx 0x0006eb90
  000ddd98: ldr r0,[sp,#0x4]
  000ddd9a: blx 0x0006ece0
  000ddd9e: b 0x000ddda4
  000ddda0: mov.w r5,#0xffffffff
  000ddda4: add r0,sp,#0x8
  000ddda6: blx 0x0006ee30
  000dddaa: ldr r0,[sp,#0x14]
  000dddac: ldr r1,[r6,#0x0]
  000dddae: subs r0,r1,r0
  000dddb0: itttt eq
  000dddb2: mov.eq r0,r5
  000dddb4: add.eq sp,#0x18
  000dddb6: pop.eq.w r11
  000dddba: pop.eq {r4,r5,r6,r7,pc}
  000dddbc: blx 0x0006e824
```
