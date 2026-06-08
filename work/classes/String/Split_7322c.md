# String::Split
elf 0x7322c body 170
Sig: undefined __thiscall Split(String * this, String param_1)

## decompile
```c

/* AbyssEngine::String::Split(AbyssEngine::String) */

Array * __thiscall AbyssEngine::String::Split(String *this,String *param_2)

{
  Array *this_00;
  uint uVar1;
  String *pSVar2;
  void *pvVar3;
  uint uVar4;
  
  if (((*(int *)(this + 4) != 0) && (*(int *)(this + 8) != 0)) && (*(int *)(param_2 + 8) != 0)) {
    this_00 = operator_new(0xc);
    Array<AbyssEngine::String*>::Array();
    uVar4 = 0;
    while (uVar1 = IndexOf(this,uVar4,param_2), uVar1 != 0xffffffff) {
      if (uVar4 < uVar1) {
        pSVar2 = operator_new(0xc);
        SubString((uint)pSVar2,(uint)this);
        ArrayAdd<AbyssEngine::String*>(pSVar2,this_00);
      }
      uVar4 = *(int *)(param_2 + 8) + uVar1;
    }
    if ((uVar4 != 0) && (uVar4 < *(uint *)(this + 8))) {
      pSVar2 = operator_new(0xc);
      SubString((uint)pSVar2,(uint)this);
      ArrayAdd<AbyssEngine::String*>(pSVar2,this_00);
    }
    if (*(int *)this_00 != 0) {
      return this_00;
    }
    ArrayRemoveAll<AbyssEngine::String*>(this_00);
    pvVar3 = (void *)Array<AbyssEngine::String*>::~Array((Array<AbyssEngine::String*> *)this_00);
    operator_delete(pvVar3);
  }
  return (Array *)0x0;
}

```

## target disasm
```
  0008322c: push {r4,r5,r6,r7,lr}
  0008322e: add r7,sp,#0xc
  00083230: push {r8,r9,r10}
  00083234: mov r9,r0
  00083236: ldr r0,[r0,#0x4]
  00083238: mov r10,r1
  0008323a: cmp r0,#0x0
  0008323c: beq 0x000832ca
  0008323e: ldr.w r0,[r9,#0x8]
  00083242: cmp r0,#0x0
  00083244: beq 0x000832ca
  00083246: ldr.w r0,[r10,#0x8]
  0008324a: cbz r0,0x000832ca
  0008324c: movs r0,#0xc
  0008324e: blx 0x0006eb24
  00083252: mov r8,r0
  00083254: blx 0x0006f628
  00083258: movs r4,#0x0
  0008325a: b 0x00083280
  0008325c: cmp r4,r5
  0008325e: bcs 0x0008327a
  00083260: movs r0,#0xc
  00083262: blx 0x0006eb24
  00083266: mov r6,r0
  00083268: mov r1,r9
  0008326a: mov r2,r4
  0008326c: mov r3,r5
  0008326e: blx 0x0006f604
  00083272: mov r0,r6
  00083274: mov r1,r8
  00083276: blx 0x0006f634
  0008327a: ldr.w r0,[r10,#0x8]
  0008327e: adds r4,r0,r5
  00083280: mov r0,r9
  00083282: mov r1,r4
  00083284: mov r2,r10
  00083286: blx 0x0006f61c
  0008328a: mov r5,r0
  0008328c: adds r0,#0x1
  0008328e: bne 0x0008325c
  00083290: cbz r4,0x000832b4
  00083292: ldr.w r6,[r9,#0x8]
  00083296: cmp r4,r6
  00083298: bcs 0x000832b4
  0008329a: movs r0,#0xc
  0008329c: blx 0x0006eb24
  000832a0: mov r5,r0
  000832a2: mov r1,r9
  000832a4: mov r2,r4
  000832a6: mov r3,r6
  000832a8: blx 0x0006f604
  000832ac: mov r0,r5
  000832ae: mov r1,r8
  000832b0: blx 0x0006f634
  000832b4: ldr.w r0,[r8,#0x0]
  000832b8: cbnz r0,0x000832ce
  000832ba: mov r0,r8
  000832bc: blx 0x0006f640
  000832c0: mov r0,r8
  000832c2: blx 0x0006f64c
  000832c6: blx 0x0006eb48
  000832ca: mov.w r8,#0x0
  000832ce: mov r0,r8
  000832d0: pop.w {r8,r9,r10}
  000832d4: pop {r4,r5,r6,r7,pc}
```
