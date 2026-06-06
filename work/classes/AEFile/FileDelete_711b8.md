# AEFile::FileDelete
elf 0x711b8 body 88
Sig: undefined __stdcall FileDelete(String * param_1)

## decompile
```c

/* AEFile::FileDelete(AbyssEngine::String const&) */

void AEFile::FileDelete(String *param_1)

{
  int iVar1;
  int *piVar2;
  int *piVar3;
  String aSStack_28 [12];
  int local_1c;
  
  piVar3 = *(int **)(DAT_00081224 + 0x811ce);
  piVar2 = (int *)**(undefined4 **)(DAT_00081220 + 0x811cc);
  local_1c = *piVar3;
  if (piVar2 != (int *)0x0) {
    AbyssEngine::String::String(aSStack_28,param_1,false);
    (**(code **)(*piVar2 + 0x28))(piVar2,aSStack_28);
    AbyssEngine::String::~String(aSStack_28);
  }
  iVar1 = *piVar3 - local_1c;
  if (iVar1 == 0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar1);
}

```
## target disasm
```
  000811b8: push {r4,r5,r6,r7,lr}
  000811ba: add r7,sp,#0xc
  000811bc: push.w r11
  000811c0: sub sp,#0x10
  000811c2: mov r1,r0
  000811c4: ldr r0,[0x00081220]
  000811c6: ldr r2,[0x00081224]
  000811c8: add r0,pc
  000811ca: add r2,pc
  000811cc: ldr r0,[r0,#0x0]
  000811ce: ldr r6,[r2,#0x0]
  000811d0: ldr r4,[r0,#0x0]
  000811d2: ldr r0,[r6,#0x0]
  000811d4: cmp r4,#0x0
  000811d6: str r0,[sp,#0xc]
  000811d8: beq 0x000811f8
  000811da: mov r5,sp
  000811dc: movs r2,#0x0
  000811de: mov r0,r5
  000811e0: blx 0x0006f028
  000811e4: ldr r0,[r4,#0x0]
  000811e6: ldr r2,[r0,#0x28]
  000811e8: mov r0,r4
  000811ea: mov r1,r5
  000811ec: blx r2
  000811ee: mov r4,r0
  000811f0: mov r0,sp
  000811f2: blx 0x0006ee30
  000811f6: b 0x000811fa
  000811f8: movs r4,#0x0
  000811fa: ldr r0,[sp,#0xc]
  000811fc: ldr r1,[r6,#0x0]
  000811fe: subs r0,r1,r0
  00081200: itttt eq
  00081202: mov.eq r0,r4
  00081204: add.eq sp,#0x10
  00081206: pop.eq.w r11
  0008120a: pop.eq {r4,r5,r6,r7,pc}
  0008120c: blx 0x0006e824
```
