# SystemPathFinder::constructPath
elf 0x11ba96 body 102
Sig: undefined __thiscall constructPath(SystemPathFinder * this, Node * param_1)

## decompile
```c

/* SystemPathFinder::constructPath(Node*) */

Array<Node*> * __thiscall SystemPathFinder::constructPath(SystemPathFinder *this,Node *param_1)

{
  Array<Node*> *this_00;
  Array<Node*> *this_01;
  uint uVar1;
  int iVar2;
  
  this_00 = operator_new(0xc);
  Array<Node*>::Array(this_00);
  for (; *(int *)(param_1 + 4) != 0; param_1 = *(Node **)(param_1 + 4)) {
    ArrayAdd<Node*>(param_1,(Array *)this_00);
  }
  this_01 = operator_new(0xc);
  Array<Node*>::Array(this_01);
  ArraySetLength<Node*>(*(uint *)this_00,(Array *)this_01);
  uVar1 = *(uint *)this_00;
  iVar2 = 0;
  while (uVar1 = uVar1 - 1, uVar1 < 0x80000000) {
    *(undefined4 *)(*(int *)(this_01 + 4) + iVar2) =
         *(undefined4 *)(*(int *)(this_00 + 4) + uVar1 * 4);
    iVar2 = iVar2 + 4;
  }
  ArrayRelease<Node*>((Array *)this_00);
  return this_01;
}

```

## target disasm
```
  0012ba96: push {r4,r5,r6,r7,lr}
  0012ba98: add r7,sp,#0xc
  0012ba9a: push.w r11
  0012ba9e: movs r0,#0xc
  0012baa0: mov r5,r1
  0012baa2: blx 0x0006eb24
  0012baa6: mov r4,r0
  0012baa8: blx 0x00075ee0
  0012baac: b 0x0012bab8
  0012baae: mov r0,r5
  0012bab0: mov r1,r4
  0012bab2: blx 0x00075f10
  0012bab6: ldr r5,[r5,#0x4]
  0012bab8: ldr r0,[r5,#0x4]
  0012baba: cmp r0,#0x0
  0012babc: bne 0x0012baae
  0012babe: movs r0,#0xc
  0012bac0: blx 0x0006eb24
  0012bac4: mov r5,r0
  0012bac6: blx 0x00075ee0
  0012baca: ldr r0,[r4,#0x0]
  0012bacc: mov r1,r5
  0012bace: blx 0x00075ef8
  0012bad2: ldr r0,[r4,#0x0]
  0012bad4: movs r1,#0x0
  0012bad6: subs r0,#0x1
  0012bad8: b 0x0012bae8
  0012bada: ldr r2,[r4,#0x4]
  0012badc: ldr r3,[r5,#0x4]
  0012bade: ldr.w r2,[r2,r0,lsl #0x2]
  0012bae2: subs r0,#0x1
  0012bae4: str r2,[r3,r1]
  0012bae6: adds r1,#0x4
  0012bae8: cmp.w r0,#0xffffffff
  0012baec: bgt 0x0012bada
  0012baee: mov r0,r4
  0012baf0: blx 0x00075eec
  0012baf4: mov r0,r5
  0012baf6: pop.w r11
  0012bafa: pop {r4,r5,r6,r7,pc}
```
