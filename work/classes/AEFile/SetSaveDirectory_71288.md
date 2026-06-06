# AEFile::SetSaveDirectory
elf 0x71288 body 80
Sig: undefined __stdcall SetSaveDirectory(String param_1)

## decompile
```c

/* AEFile::SetSaveDirectory(AbyssEngine::String) */

void AEFile::SetSaveDirectory(String *param_1)

{
  int *piVar1;
  int *piVar2;
  String aSStack_28 [12];
  int local_1c;
  
  piVar2 = *(int **)(DAT_000812ec + 0x8129e);
  piVar1 = (int *)**(undefined4 **)(DAT_000812e8 + 0x8129c);
  local_1c = *piVar2;
  if (piVar1 != (int *)0x0) {
    AbyssEngine::String::String(aSStack_28,param_1,false);
    (**(code **)(*piVar1 + 0x50))(piVar1,aSStack_28);
    AbyssEngine::String::~String(aSStack_28);
  }
  if (*piVar2 != local_1c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```
## target disasm
```
  00081288: push {r4,r5,r6,r7,lr}
  0008128a: add r7,sp,#0xc
  0008128c: push.w r11
  00081290: sub sp,#0x10
  00081292: mov r1,r0
  00081294: ldr r0,[0x000812e8]
  00081296: ldr r2,[0x000812ec]
  00081298: add r0,pc
  0008129a: add r2,pc
  0008129c: ldr r0,[r0,#0x0]
  0008129e: ldr r6,[r2,#0x0]
  000812a0: ldr r4,[r0,#0x0]
  000812a2: ldr r0,[r6,#0x0]
  000812a4: cmp r4,#0x0
  000812a6: str r0,[sp,#0xc]
  000812a8: beq 0x000812c4
  000812aa: mov r5,sp
  000812ac: movs r2,#0x0
  000812ae: mov r0,r5
  000812b0: blx 0x0006f028
  000812b4: ldr r0,[r4,#0x0]
  000812b6: ldr r2,[r0,#0x50]
  000812b8: mov r0,r4
  000812ba: mov r1,r5
  000812bc: blx r2
  000812be: mov r0,sp
  000812c0: blx 0x0006ee30
  000812c4: ldr r0,[sp,#0xc]
  000812c6: ldr r1,[r6,#0x0]
  000812c8: subs r0,r1,r0
  000812ca: ittt eq
  000812cc: add.eq sp,#0x10
  000812ce: pop.eq.w r11
  000812d2: pop.eq {r4,r5,r6,r7,pc}
  000812d4: blx 0x0006e824
```
