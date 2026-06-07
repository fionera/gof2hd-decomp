# PaintCanvas::MeshCreate
elf 0x79f2c body 132
Sig: undefined __thiscall MeshCreate(PaintCanvas * this, ushort param_1, ushort param_2, signed param_3, ushort param_4, uint * param_5)

## decompile
```c

/* AbyssEngine::PaintCanvas::MeshCreate(unsigned short, unsigned short, signed char, unsigned short,
   unsigned int&) */

void __thiscall
AbyssEngine::PaintCanvas::MeshCreate
          (PaintCanvas *this,undefined4 param_2,undefined4 param_3,undefined4 param_4,ushort param_5
          ,int *param_6)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int *piVar4;
  
  iVar3 = -1;
  piVar4 = *(int **)(DAT_00089fb0 + 0x89f40);
  iVar1 = *piVar4;
  MaterialCreate(this,param_5,(uint *)&stack0xffffffd8);
  iVar2 = AbyssEngine::MeshCreate
                    (*(undefined4 *)(this + 0x34),param_2,param_3,param_4,&stack0xffffffd4);
  if (iVar2 == 1) {
    if (0xfffffffe < *(uint *)(this + 0x174)) {
      uRam00000030 = *(undefined4 *)(*(int *)(this + 0x178) + -4);
    }
    ArrayAdd<AbyssEngine::Mesh*>((Mesh *)0x0,(Array *)(this + 0x24));
    iVar3 = *(int *)(this + 0x24) + -1;
  }
  *param_6 = iVar3;
  iVar2 = *piVar4;
  iVar1 = iVar2 - iVar1;
  if (iVar1 == 0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar1,iVar2);
}

```

## target disasm
```
  00089f2c: push {r4,r5,r6,r7,lr}
  00089f2e: add r7,sp,#0xc
  00089f30: push {r4,r5,r6,r7,r8,r9,r10}
  00089f34: mov r4,r0
  00089f36: ldr r0,[0x00089fb0]
  00089f38: mov r5,r1
  00089f3a: ldr r1,[r7,#0x8]
  00089f3c: add r0,pc
  00089f3e: mov r10,r2
  00089f40: add r2,sp,#0x8
  00089f42: mov.w r6,#0xffffffff
  00089f46: ldr.w r9,[r0,#0x0]
  00089f4a: mov r8,r3
  00089f4c: ldr.w r0,[r9,#0x0]
  00089f50: str r0,[sp,#0xc]
  00089f52: mov r0,r4
  00089f54: str r6,[sp,#0x8]
  00089f56: blx 0x0006fc1c
  00089f5a: movs r0,#0x0
  00089f5c: add r1,sp,#0x4
  00089f5e: str r0,[sp,#0x4]
  00089f60: mov r2,r10
  00089f62: ldr r0,[r4,#0x34]
  00089f64: mov r3,r8
  00089f66: str r1,[sp,#0x0]
  00089f68: mov r1,r5
  00089f6a: blx 0x0006f1fc
  00089f6e: ldr r5,[r7,#0xc]
  00089f70: cmp r0,#0x1
  00089f72: bne 0x00089f9a
  00089f74: ldr r1,[sp,#0x8]
  00089f76: ldr.w r0,[r4,#0x174]
  00089f7a: cmp r1,r0
  00089f7c: bls 0x00089f82
  00089f7e: ldr r0,[sp,#0x4]
  00089f80: b 0x00089f8e
  00089f82: ldr.w r2,[r4,#0x178]
  00089f86: ldr r0,[sp,#0x4]
  00089f88: ldr.w r1,[r2,r1,lsl #0x2]
  00089f8c: str r1,[r0,#0x30]
  00089f8e: add.w r1,r4,#0x24
  00089f92: blx 0x0006ecbc
  00089f96: ldr r0,[r4,#0x24]
  00089f98: subs r6,r0,#0x1
  00089f9a: str r6,[r5,#0x0]
  00089f9c: ldr r0,[sp,#0xc]
  00089f9e: ldr.w r1,[r9,#0x0]
  00089fa2: subs r0,r1,r0
  00089fa4: itt eq
  00089fa6: pop.eq.w {r0,r1,r2,r3,r8,r9,r10}
  00089faa: pop.eq {r4,r5,r6,r7,pc}
  00089fac: blx 0x0006e824
```
