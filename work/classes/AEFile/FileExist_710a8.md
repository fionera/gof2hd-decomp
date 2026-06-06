# AEFile::FileExist
elf 0x710a8 body 202
Sig: undefined __stdcall FileExist(String * param_1)

## decompile
```c

/* AEFile::FileExist(AbyssEngine::String const&) */

void AEFile::FileExist(String *param_1)

{
  int iVar1;
  String *this;
  short *psVar2;
  code *pcVar3;
  int *piVar4;
  int *piVar5;
  String aSStack_58 [12];
  String aSStack_4c [12];
  String aSStack_40 [12];
  String aSStack_34 [12];
  String aSStack_28 [12];
  int local_1c;
  
  piVar5 = *(int **)(DAT_000811ac + 0x810be);
  piVar4 = (int *)**(undefined4 **)(DAT_000811a8 + 0x810bc);
  local_1c = *piVar5;
  if (piVar4 != (int *)0x0) {
    AbyssEngine::String::String(aSStack_28,param_1,false);
    iVar1 = (**(code **)(*piVar4 + 0x24))(piVar4,aSStack_28);
    AbyssEngine::String::~String(aSStack_28);
    if (iVar1 == 0) {
      this = (String *)AbyssEngine::String::String(aSStack_34,param_1,false);
      psVar2 = (short *)AbyssEngine::String::operator[](this,0);
      if (*psVar2 != 0x2f) {
        AbyssEngine::String::String(aSStack_4c,(char *)(DAT_000811b0 + 0x81112),false);
        AbyssEngine::String::String(aSStack_58,param_1,false);
        AbyssEngine::operator+(aSStack_40,aSStack_4c);
        AbyssEngine::String::operator=(aSStack_34,aSStack_40);
        pcVar3 = *(code **)(DAT_000811b4 + 0x8113a);
        (*pcVar3)(aSStack_40);
        (*pcVar3)(aSStack_58);
        (*pcVar3)(aSStack_4c);
      }
      findPakFile(aSStack_34);
      AbyssEngine::String::~String(aSStack_34);
    }
  }
  iVar1 = *piVar5 - local_1c;
  if (iVar1 == 0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar1);
}

```
## target disasm
```
  000810a8: push {r4,r5,r6,r7,lr}
  000810aa: add r7,sp,#0xc
  000810ac: push.w r8
  000810b0: sub sp,#0x40
  000810b2: mov r4,r0
  000810b4: ldr r0,[0x000811a8]
  000810b6: ldr r1,[0x000811ac]
  000810b8: add r0,pc
  000810ba: add r1,pc
  000810bc: ldr r0,[r0,#0x0]
  000810be: ldr.w r8,[r1,#0x0]
  000810c2: ldr r5,[r0,#0x0]
  000810c4: ldr.w r0,[r8,#0x0]
  000810c8: cmp r5,#0x0
  000810ca: str r0,[sp,#0x3c]
  000810cc: beq 0x000810f2
  000810ce: add r6,sp,#0x30
  000810d0: mov r1,r4
  000810d2: movs r2,#0x0
  000810d4: mov r0,r6
  000810d6: blx 0x0006f028
  000810da: ldr r0,[r5,#0x0]
  000810dc: ldr r2,[r0,#0x24]
  000810de: mov r0,r5
  000810e0: mov r1,r6
  000810e2: blx r2
  000810e4: mov r5,r0
  000810e6: add r0,sp,#0x30
  000810e8: blx 0x0006ee30
  000810ec: cbz r5,0x000810f6
  000810ee: movs r4,#0x1
  000810f0: b 0x0008115a
  000810f2: movs r4,#0x0
  000810f4: b 0x0008115a
  000810f6: add r0,sp,#0x24
  000810f8: mov r1,r4
  000810fa: movs r2,#0x0
  000810fc: blx 0x0006f028
  00081100: movs r1,#0x0
  00081102: blx 0x0006f3b8
  00081106: ldrh r0,[r0,#0x0]
  00081108: cmp r0,#0x2f
  0008110a: beq 0x00081146
  0008110c: ldr r1,[0x000811b0]
  0008110e: add r1,pc
  00081110: add r0,sp,#0xc
  00081112: movs r2,#0x0
  00081114: blx 0x0006ee18
  00081118: mov r0,sp
  0008111a: mov r1,r4
  0008111c: movs r2,#0x0
  0008111e: blx 0x0006f028
  00081122: add r0,sp,#0x18
  00081124: add r1,sp,#0xc
  00081126: mov r2,sp
  00081128: blx 0x0006ef98
  0008112c: add r0,sp,#0x24
  0008112e: add r1,sp,#0x18
  00081130: blx 0x0006f2b0
  00081134: ldr r0,[0x000811b4]
  00081136: add r0,pc
  00081138: ldr r4,[r0,#0x0]
  0008113a: add r0,sp,#0x18
  0008113c: blx r4
  0008113e: mov r0,sp
  00081140: blx r4
  00081142: add r0,sp,#0xc
  00081144: blx r4
  00081146: add r0,sp,#0x24
  00081148: blx 0x0006f3c4
  0008114c: mov r4,r0
  0008114e: add r0,sp,#0x24
  00081150: blx 0x0006ee30
  00081154: cmp r4,#0x0
  00081156: it ne
  00081158: mov.ne r4,#0x1
  0008115a: ldr r0,[sp,#0x3c]
  0008115c: ldr.w r1,[r8,#0x0]
  00081160: subs r0,r1,r0
  00081162: itttt eq
  00081164: mov.eq r0,r4
  00081166: add.eq sp,#0x40
  00081168: pop.eq.w r8
  0008116c: pop.eq {r4,r5,r6,r7,pc}
  0008116e: blx 0x0006e824
```
