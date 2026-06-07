# ApplicationManager::OnTouchMove
elf 0x83bb0 body 100
Sig: undefined __thiscall OnTouchMove(ApplicationManager * this, int param_1, int param_2, void * param_3)

## decompile
```c

/* AbyssEngine::ApplicationManager::OnTouchMove(int, int, void*) */

void __thiscall
AbyssEngine::ApplicationManager::OnTouchMove
          (ApplicationManager *this,int param_1,int param_2,void *param_3)

{
  int iVar1;
  int iVar2;
  int *piVar3;
  
  piVar3 = *(int **)(DAT_00093c14 + 0x93bc4);
  iVar1 = *piVar3;
  if ((*(int *)(this + 0x18) != 0) && (*(int *)(this + 0x3c) == 5)) {
    ConvertTouchCoords(this,(int *)&stack0xffffffd8,(int *)&stack0xffffffd4);
    (**(code **)(**(int **)(this + 0x18) + 0x28))(*(int **)(this + 0x18),param_1,param_2,param_3);
    (**(code **)(**(int **)(this + 0x18) + 0x1c))(*(int **)(this + 0x18),param_1,param_2);
    *(int *)(this + 0xb4) = param_1;
    *(int *)(this + 0xb8) = param_2;
  }
  iVar2 = *piVar3;
  iVar1 = iVar2 - iVar1;
  if (iVar1 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar1,iVar2);
  }
  return;
}

```

## target disasm
```
  00093bb0: push {r4,r5,r6,r7,lr}
  00093bb2: add r7,sp,#0xc
  00093bb4: push {r4,r5,r6,r7,r8,r9,r11}
  00093bb8: mov r4,r0
  00093bba: ldr r0,[0x00093c14]
  00093bbc: str r1,[sp,#0x8]
  00093bbe: mov r8,r3
  00093bc0: add r0,pc
  00093bc2: ldr r6,[r0,#0x0]
  00093bc4: ldr r0,[r6,#0x0]
  00093bc6: str r2,[sp,#0x4]
  00093bc8: str r0,[sp,#0xc]
  00093bca: ldr r0,[r4,#0x18]
  00093bcc: cbz r0,0x00093c02
  00093bce: ldr r0,[r4,#0x3c]
  00093bd0: cmp r0,#0x5
  00093bd2: bne 0x00093c02
  00093bd4: add r1,sp,#0x8
  00093bd6: add r2,sp,#0x4
  00093bd8: mov r0,r4
  00093bda: blx 0x000704b0
  00093bde: ldr r0,[r4,#0x18]
  00093be0: mov r3,r8
  00093be2: ldr r1,[r0,#0x0]
  00093be4: ldrd r9,r5,[sp,#0x4]
  00093be8: ldr.w r12,[r1,#0x28]
  00093bec: mov r1,r5
  00093bee: mov r2,r9
  00093bf0: blx r12
  00093bf2: ldr r0,[r4,#0x18]
  00093bf4: mov r2,r9
  00093bf6: ldr r1,[r0,#0x0]
  00093bf8: ldr r3,[r1,#0x1c]
  00093bfa: mov r1,r5
  00093bfc: blx r3
  00093bfe: strd r5,r9,[r4,#0xb4]
  00093c02: ldr r0,[sp,#0xc]
  00093c04: ldr r1,[r6,#0x0]
  00093c06: subs r0,r1,r0
  00093c08: itt eq
  00093c0a: pop.eq.w {r0,r1,r2,r3,r8,r9,r11}
  00093c0e: pop.eq {r4,r5,r6,r7,pc}
  00093c10: blx 0x0006e824
```
