# ApplicationManager::OnTouchEnd
elf 0x83c18 body 110
Sig: undefined __thiscall OnTouchEnd(ApplicationManager * this, int param_1, int param_2, void * param_3)

## decompile
```c

/* AbyssEngine::ApplicationManager::OnTouchEnd(int, int, void*) */

void __thiscall
AbyssEngine::ApplicationManager::OnTouchEnd
          (ApplicationManager *this,int param_1,int param_2,void *param_3)

{
  int iVar1;
  int iVar2;
  int *piVar3;
  
  piVar3 = *(int **)(DAT_00093c8c + 0x93c30);
  **(undefined4 **)(DAT_00093c88 + 0x93c2c) = 0;
  iVar1 = *piVar3;
  if ((*(int *)(this + 0x18) != 0) && (*(int *)(this + 0x3c) == 5)) {
    ConvertTouchCoords(this,(int *)&stack0xffffffd8,(int *)&stack0xffffffd4);
    (**(code **)(**(int **)(this + 0x18) + 0x2c))(*(int **)(this + 0x18),param_1,param_2,param_3);
    (**(code **)(**(int **)(this + 0x18) + 0x20))(*(int **)(this + 0x18),param_1,param_2);
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
  00093c18: push {r4,r5,r6,r7,lr}
  00093c1a: add r7,sp,#0xc
  00093c1c: push {r4,r5,r6,r7,r8,r9,r11}
  00093c20: mov r4,r0
  00093c22: ldr r0,[0x00093c88]
  00093c24: mov r8,r3
  00093c26: ldr r3,[0x00093c8c]
  00093c28: add r0,pc
  00093c2a: str r2,[sp,#0x4]
  00093c2c: add r3,pc
  00093c2e: ldr r0,[r0,#0x0]
  00093c30: ldr r6,[r3,#0x0]
  00093c32: movs r3,#0x0
  00093c34: str r3,[r0,#0x0]
  00093c36: ldr r0,[r6,#0x0]
  00093c38: str r1,[sp,#0x8]
  00093c3a: str r0,[sp,#0xc]
  00093c3c: ldr r0,[r4,#0x18]
  00093c3e: cbz r0,0x00093c74
  00093c40: ldr r0,[r4,#0x3c]
  00093c42: cmp r0,#0x5
  00093c44: bne 0x00093c74
  00093c46: add r1,sp,#0x8
  00093c48: add r2,sp,#0x4
  00093c4a: mov r0,r4
  00093c4c: blx 0x000704b0
  00093c50: ldr r0,[r4,#0x18]
  00093c52: mov r3,r8
  00093c54: ldr r1,[r0,#0x0]
  00093c56: ldrd r9,r5,[sp,#0x4]
  00093c5a: ldr.w r12,[r1,#0x2c]
  00093c5e: mov r1,r5
  00093c60: mov r2,r9
  00093c62: blx r12
  00093c64: ldr r0,[r4,#0x18]
  00093c66: mov r2,r9
  00093c68: ldr r1,[r0,#0x0]
  00093c6a: ldr r3,[r1,#0x20]
  00093c6c: mov r1,r5
  00093c6e: blx r3
  00093c70: strd r5,r9,[r4,#0xb4]
  00093c74: ldr r0,[sp,#0xc]
  00093c76: ldr r1,[r6,#0x0]
  00093c78: subs r0,r1,r0
  00093c7a: itt eq
  00093c7c: pop.eq.w {r0,r1,r2,r3,r8,r9,r11}
  00093c80: pop.eq {r4,r5,r6,r7,pc}
  00093c82: blx 0x0006e824
```
