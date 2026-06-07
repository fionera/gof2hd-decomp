# SystemPathFinder::search
elf 0x11b968 body 182
Sig: undefined __thiscall search(SystemPathFinder * this, Node * param_1, Node * param_2)

## decompile
```c

/* SystemPathFinder::search(Node*, Node*) */

undefined4 __thiscall SystemPathFinder::search(SystemPathFinder *this,Node *param_1,Node *param_2)

{
  int iVar1;
  Array<Node*> *this_00;
  Array<Node*> *this_01;
  Node *pNVar2;
  undefined4 uVar3;
  uint uVar4;
  uint uVar5;
  Node *pNVar6;
  
  this_00 = operator_new(0xc);
  Array<Node*>::Array(this_00);
  this_01 = operator_new(0xc);
  Array<Node*>::Array(this_01);
  ArrayAdd<Node*>(param_1,(Array *)this_01);
  *(undefined4 *)(param_1 + 4) = 0;
LAB_0012ba00:
  if (*(int *)this_01 == 0) {
    return 0;
  }
  pNVar6 = (Node *)**(undefined4 **)(this_01 + 4);
  ArrayRemove<Node*>(pNVar6,(Array *)this_01);
  if (pNVar6 == param_2) {
    uVar3 = (*(code *)(DAT_001ac314 + 0x1ac318))();
    return uVar3;
  }
  ArrayAdd<Node*>(pNVar6,(Array *)this_00);
  uVar5 = 0;
LAB_0012b9f6:
  if (uVar5 < **(uint **)pNVar6) {
    uVar4 = 0;
    pNVar2 = *(Node **)((*(uint **)pNVar6)[1] + uVar5 * 4);
    do {
      if (*(uint *)this_00 <= uVar4) {
        uVar4 = 0;
        goto LAB_0012b9d8;
      }
      iVar1 = uVar4 * 4;
      uVar4 = uVar4 + 1;
    } while (*(Node **)(*(int *)(this_00 + 4) + iVar1) != pNVar2);
    goto LAB_0012b9f4;
  }
  goto LAB_0012ba00;
  while (iVar1 = uVar4 * 4, uVar4 = uVar4 + 1, *(Node **)(*(int *)(this_01 + 4) + iVar1) != pNVar2)
  {
LAB_0012b9d8:
    if (*(uint *)this_01 <= uVar4) {
      *(Node **)(pNVar2 + 4) = pNVar6;
      ArrayAdd<Node*>(pNVar2,(Array *)this_01);
      break;
    }
  }
LAB_0012b9f4:
  uVar5 = uVar5 + 1;
  goto LAB_0012b9f6;
}

```

## target disasm
```
  0012b968: push {r4,r5,r6,r7,lr}
  0012b96a: add r7,sp,#0xc
  0012b96c: push {r8,r9,r11}
  0012b970: movs r0,#0xc
  0012b972: mov r8,r2
  0012b974: mov r4,r1
  0012b976: blx 0x0006eb24
  0012b97a: mov r5,r0
  0012b97c: blx 0x00075ee0
  0012b980: movs r0,#0xc
  0012b982: blx 0x0006eb24
  0012b986: mov r6,r0
  0012b988: blx 0x00075ee0
  0012b98c: mov r0,r4
  0012b98e: mov r1,r6
  0012b990: blx 0x00075f10
  0012b994: movs r0,#0x0
  0012b996: str r0,[r4,#0x4]
  0012b998: b 0x0012ba00
  0012b99a: ldr r0,[r6,#0x4]
  0012b99c: mov r1,r6
  0012b99e: ldr.w r9,[r0,#0x0]
  0012b9a2: mov r0,r9
  0012b9a4: blx 0x00075f40
  0012b9a8: cmp r9,r8
  0012b9aa: beq 0x0012ba0e
  0012b9ac: mov r0,r9
  0012b9ae: mov r1,r5
  0012b9b0: blx 0x00075f10
  0012b9b4: movs r4,#0x0
  0012b9b6: b 0x0012b9f6
  0012b9b8: ldr r0,[r0,#0x4]
  0012b9ba: movs r2,#0x0
  0012b9bc: ldr r1,[r5,#0x0]
  0012b9be: ldr.w r0,[r0,r4,lsl #0x2]
  0012b9c2: cmp r2,r1
  0012b9c4: bcs 0x0012b9d4
  0012b9c6: ldr r3,[r5,#0x4]
  0012b9c8: ldr.w r3,[r3,r2,lsl #0x2]
  0012b9cc: adds r2,#0x1
  0012b9ce: cmp r3,r0
  0012b9d0: bne 0x0012b9c2
  0012b9d2: b 0x0012b9f4
  0012b9d4: ldr r1,[r6,#0x0]
  0012b9d6: movs r2,#0x0
  0012b9d8: cmp r2,r1
  0012b9da: bcs 0x0012b9ea
  0012b9dc: ldr r3,[r6,#0x4]
  0012b9de: ldr.w r3,[r3,r2,lsl #0x2]
  0012b9e2: adds r2,#0x1
  0012b9e4: cmp r3,r0
  0012b9e6: bne 0x0012b9d8
  0012b9e8: b 0x0012b9f4
  0012b9ea: mov r1,r6
  0012b9ec: str.w r9,[r0,#0x4]
  0012b9f0: blx 0x00075f10
  0012b9f4: adds r4,#0x1
  0012b9f6: ldr.w r0,[r9,#0x0]
  0012b9fa: ldr r1,[r0,#0x0]
  0012b9fc: cmp r4,r1
  0012b9fe: bcc 0x0012b9b8
  0012ba00: ldr r0,[r6,#0x0]
  0012ba02: cmp r0,#0x0
  0012ba04: bne 0x0012b99a
  0012ba06: movs r0,#0x0
  0012ba08: pop.w {r8,r9,r11}
  0012ba0c: pop {r4,r5,r6,r7,pc}
  0012ba0e: mov r1,r8
  0012ba10: pop.w {r8,r9,r11}
  0012ba14: pop.w {r4,r5,r6,r7,lr}
  0012ba18: b.w 0x001ac308
  001ac308: bx pc
```
