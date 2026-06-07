# PaintCanvas::EnableClip
elf 0x7cab0 body 132
Sig: undefined __thiscall EnableClip(PaintCanvas * this, int param_1, int param_2, int param_3, int param_4)

## decompile
```c

/* AbyssEngine::PaintCanvas::EnableClip(int, int, int, int) */

void __thiscall
AbyssEngine::PaintCanvas::EnableClip
          (PaintCanvas *this,int param_1,int param_2,int param_3,int param_4)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  glEnable(0xc11);
  iVar1 = param_2;
  iVar3 = param_3;
  switch(*(undefined4 *)(this + 0x30)) {
  case 0:
    iVar1 = param_1;
    iVar3 = param_4;
    param_4 = param_3;
    param_1 = param_2;
    break;
  case 1:
    iVar4 = param_2 + param_4;
    iVar2 = GetHeight();
    iVar1 = GetWidth();
    iVar1 = iVar1 - (param_1 + param_3);
    iVar3 = param_4;
    param_4 = param_3;
    param_1 = iVar2 - iVar4;
    break;
  case 2:
    iVar1 = GetHeight();
    iVar1 = iVar1 - (param_2 + param_4);
    break;
  case 3:
    iVar2 = GetWidth();
    param_1 = iVar2 - (param_1 + param_3);
  }
  (*(code *)(DAT_001ab384 + 0x1ab388))(param_1,iVar1,iVar3,param_4);
  return;
}

```

## target disasm
```
  0008cab0: push {r4,r5,r6,r7,lr}
  0008cab2: add r7,sp,#0xc
  0008cab4: push {r8,r9,r11}
  0008cab8: mov r6,r0
  0008caba: movw r0,#0xc11
  0008cabe: mov r4,r3
  0008cac0: mov r5,r2
  0008cac2: mov r8,r1
  0008cac4: blx 0x0006edb8
  0008cac8: ldr r0,[r6,#0x30]
  0008caca: ldr.w r9,[r7,#0x8]
  0008cace: cmp r0,#0x3
  0008cad0: bhi 0x0008cae0
  0008cad2: tbb [pc,r0]
  0008cada: mov r2,r9
  0008cadc: mov r1,r8
  0008cade: b 0x0008cb26
  0008cae0: mov r2,r4
  0008cae2: mov r1,r5
  0008cae4: b 0x0008cb0c
  0008cae6: mov r0,r6
  0008cae8: add r5,r9
  0008caea: blx 0x0006fb08
  0008caee: subs r5,r0,r5
  0008caf0: mov r0,r6
  0008caf2: add r8,r4
  0008caf4: blx 0x0006fafc
  0008caf8: sub.w r1,r0,r8
  0008cafc: mov r2,r9
  0008cafe: b 0x0008cb26
  0008cb00: mov r0,r6
  0008cb02: add r5,r9
  0008cb04: blx 0x0006fb08
  0008cb08: subs r1,r0,r5
  0008cb0a: mov r2,r4
  0008cb0c: mov r4,r9
  0008cb0e: mov r5,r8
  0008cb10: b 0x0008cb26
  0008cb12: mov r0,r6
  0008cb14: add r8,r4
  0008cb16: blx 0x0006fafc
  0008cb1a: sub.w r0,r0,r8
  0008cb1e: mov r2,r4
  0008cb20: mov r1,r5
  0008cb22: mov r4,r9
  0008cb24: mov r5,r0
  0008cb26: mov r0,r5
  0008cb28: mov r3,r4
  0008cb2a: pop.w {r8,r9,r11}
  0008cb2e: pop.w {r4,r5,r6,r7,lr}
  0008cb32: b.w 0x001ab378
  001ab378: bx pc
```
