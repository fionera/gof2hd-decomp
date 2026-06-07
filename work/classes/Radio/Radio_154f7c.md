# Radio::Radio
elf 0x154f7c body 78
Sig: undefined __thiscall Radio(Radio * this)

## decompile
```c

/* Radio::Radio() */

void __thiscall Radio::Radio(Radio *this)

{
  undefined4 *puVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  
  iVar3 = DAT_00164fd0;
  puVar1 = (undefined4 *)(DAT_00164fcc + 0x164f8e);
  *(undefined4 *)(this + 0x18) = 0;
  *(undefined4 *)(this + 0x1c) = 0;
  *(undefined4 *)(this + 0x28) = 0;
  *(undefined2 *)(this + 0x2c) = 0;
  *(undefined4 *)(this + 0x10) = 0;
  piVar2 = (int *)*puVar1;
  *(undefined4 *)(this + 0x30) = 0xffffffff;
  *(undefined4 *)this = 0;
  *(undefined4 *)(this + 4) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  *(undefined4 *)(this + 8) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  *(undefined4 *)(this + 0xc) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  iVar5 = *piVar2;
  iVar4 = *(int *)(iVar5 + 0x98);
  iVar3 = **(int **)(iVar3 + 0x164f96);
  *(int *)(this + 0x38) = iVar4;
  *(int *)(this + 0x3c) = iVar3 - iVar4 >> 1;
  *(undefined4 *)(this + 0x40) = *(undefined4 *)(iVar5 + 0x9c);
  return;
}

```

## target disasm
```
  00164f7c: push {r7,lr}
  00164f7e: mov r7,sp
  00164f80: ldr r1,[0x00164fcc]
  00164f82: movs r3,#0x0
  00164f84: ldr r2,[0x00164fd0]
  00164f86: vmov.i32 q8,#0x0
  00164f8a: add r1,pc
  00164f8c: strd r3,r3,[r0,#0x18]
  00164f90: str r3,[r0,#0x28]
  00164f92: add r2,pc
  00164f94: strh r3,[r0,#0x2c]
  00164f96: mov.w r12,#0x40
  00164f9a: str r3,[r0,#0x10]
  00164f9c: mov.w r3,#0xffffffff
  00164fa0: ldr r1,[r1,#0x0]
  00164fa2: str r3,[r0,#0x30]
  00164fa4: mov r3,r0
  00164fa6: vst1.64 {d16,d17},[r3],r12
  00164faa: ldr.w lr,[r1,#0x0]
  00164fae: ldr.w r12,[r2,#0x0]
  00164fb2: ldr.w r2,[lr,#0x98]
  00164fb6: ldr.w r1,[r12,#0x0]
  00164fba: subs r1,r1,r2
  00164fbc: asrs r1,r1,#0x1
  00164fbe: strd r2,r1,[r0,#0x38]
  00164fc2: ldr.w r1,[lr,#0x9c]
  00164fc6: str r1,[r3,#0x0]
  00164fc8: pop {r7,pc}
```
