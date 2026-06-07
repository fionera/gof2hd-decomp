# PaintCanvas::MeshCreate
elf 0x79fb4 body 84
Sig: undefined __stdcall MeshCreate(ushort param_1, ushort param_2, signed param_3, uint * param_4)

## decompile
```c

/* AbyssEngine::PaintCanvas::MeshCreate(unsigned short, unsigned short, signed char, unsigned int&)
    */

void AbyssEngine::PaintCanvas::MeshCreate(int param_1)

{
  int iVar1;
  int iVar2;
  int *piVar3;
  int *in_stack_00000000;
  
  piVar3 = *(int **)(DAT_0008a008 + 0x89fc8);
  iVar1 = *piVar3;
  iVar2 = AbyssEngine::MeshCreate(*(undefined4 *)(param_1 + 0x34));
  if (iVar2 == 1) {
    ArrayAdd<AbyssEngine::Mesh*>((Mesh *)0x0,(Array *)(param_1 + 0x24));
    iVar2 = *(int *)(param_1 + 0x24) + -1;
  }
  else {
    iVar2 = -1;
  }
  *in_stack_00000000 = iVar2;
  if (*piVar3 != iVar1) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  00089fb4: push {r4,r5,r6,r7,lr}
  00089fb6: add r7,sp,#0xc
  00089fb8: push.w r11
  00089fbc: sub sp,#0x10
  00089fbe: mov r4,r0
  00089fc0: ldr r0,[0x0008a008]
  00089fc2: add r6,sp,#0x8
  00089fc4: add r0,pc
  00089fc6: ldr r5,[r0,#0x0]
  00089fc8: ldr r0,[r5,#0x0]
  00089fca: str r0,[sp,#0xc]
  00089fcc: movs r0,#0x0
  00089fce: str r0,[sp,#0x8]
  00089fd0: ldr r0,[r4,#0x34]
  00089fd2: str r6,[sp,#0x0]
  00089fd4: blx 0x0006f1fc
  00089fd8: ldr r6,[r7,#0x8]
  00089fda: cmp r0,#0x1
  00089fdc: bne 0x00089fee
  00089fde: ldr r0,[sp,#0x8]
  00089fe0: add.w r1,r4,#0x24
  00089fe4: blx 0x0006ecbc
  00089fe8: ldr r0,[r4,#0x24]
  00089fea: subs r0,#0x1
  00089fec: b 0x00089ff2
  00089fee: mov.w r0,#0xffffffff
  00089ff2: str r0,[r6,#0x0]
  00089ff4: ldr r0,[sp,#0xc]
  00089ff6: ldr r1,[r5,#0x0]
  00089ff8: subs r0,r1,r0
  00089ffa: ittt eq
  00089ffc: add.eq sp,#0x10
  00089ffe: pop.eq.w r11
  0008a002: pop.eq {r4,r5,r6,r7,pc}
  0008a004: blx 0x0006e824
```
