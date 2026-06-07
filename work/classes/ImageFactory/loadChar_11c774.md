# ImageFactory::loadChar
elf 0x11c774 body 98
Sig: undefined __thiscall loadChar(ImageFactory * this, int * param_1)

## decompile
```c

/* ImageFactory::loadChar(int*) */

Array<ImagePart*> * __thiscall ImageFactory::loadChar(ImageFactory *this,int *param_1)

{
  Array<ImagePart*> *this_00;
  ImageFactory *this_01;
  undefined4 *puVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  
  if (param_1 == (int *)0x0) {
    this_00 = (Array<ImagePart*> *)0x0;
  }
  else {
    this_00 = operator_new(0xc);
    Array<ImagePart*>::Array(this_00);
    ArraySetLength<ImagePart*>(4,(Array *)this_00);
    iVar4 = *param_1;
    for (iVar3 = 0; iVar3 != 4; iVar3 = iVar3 + 1) {
      this_01 = (ImageFactory *)(param_1[iVar3 + 1] + 1);
      if (this_01 != (ImageFactory *)0x0) {
        uVar2 = loadImage(this_01,iVar4,iVar3,param_1[iVar3 + 1]);
        *(undefined4 *)(*(int *)(this_00 + 4) + iVar3 * 4) = uVar2;
      }
    }
    puVar1 = *(undefined4 **)(this_00 + 4);
    uVar2 = *puVar1;
    *puVar1 = puVar1[2];
    *(undefined4 *)(*(int *)(this_00 + 4) + 8) = uVar2;
  }
  return this_00;
}

```

## target disasm
```
  0012c774: push {r4,r5,r6,r7,lr}
  0012c776: add r7,sp,#0xc
  0012c778: push.w r8
  0012c77c: mov r5,r1
  0012c77e: cbz r1,0x0012c7ca
  0012c780: movs r0,#0xc
  0012c782: blx 0x0006eb24
  0012c786: mov r8,r0
  0012c788: blx 0x00076024
  0012c78c: movs r0,#0x4
  0012c78e: mov r1,r8
  0012c790: blx 0x00076030
  0012c794: ldmia r5!,{r6}
  0012c796: movs r4,#0x0
  0012c798: b 0x0012c7b4
  0012c79a: ldr.w r3,[r5,r4,lsl #0x2]
  0012c79e: adds r0,r3,#0x1
  0012c7a0: beq 0x0012c7b2
  0012c7a2: mov r1,r6
  0012c7a4: mov r2,r4
  0012c7a6: blx 0x0007603c
  0012c7aa: ldr.w r1,[r8,#0x4]
  0012c7ae: str.w r0,[r1,r4,lsl #0x2]
  0012c7b2: adds r4,#0x1
  0012c7b4: cmp r4,#0x4
  0012c7b6: bne 0x0012c79a
  0012c7b8: ldr.w r0,[r8,#0x4]
  0012c7bc: ldr r2,[r0,#0x8]
  0012c7be: ldr r1,[r0,#0x0]
  0012c7c0: str r2,[r0,#0x0]
  0012c7c2: ldr.w r0,[r8,#0x4]
  0012c7c6: str r1,[r0,#0x8]
  0012c7c8: b 0x0012c7ce
  0012c7ca: mov.w r8,#0x0
  0012c7ce: mov r0,r8
  0012c7d0: pop.w r8
  0012c7d4: pop {r4,r5,r6,r7,pc}
```
