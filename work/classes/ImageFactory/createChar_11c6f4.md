# ImageFactory::createChar
elf 0x11c6f4 body 114
Sig: undefined __stdcall createChar(bool param_1, int param_2)

## decompile
```c

/* ImageFactory::createChar(bool, int) */

int * ImageFactory::createChar(bool param_1,int param_2)

{
  int iVar1;
  int *piVar2;
  int iVar3;
  int in_r2;
  int *piVar4;
  
  if ((in_r2 == 3) &&
     (iVar1 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_0012c768 + 0x12c70a)), in_r2 = 0,
     iVar1 != 0)) {
    in_r2 = 2;
  }
  iVar1 = in_r2;
  if (in_r2 == 0) {
    iVar1 = 10;
  }
  if (param_2 != 0) {
    iVar1 = in_r2;
  }
  iVar3 = 0;
  if (iVar1 != 5) {
    iVar3 = iVar1;
  }
  piVar2 = operator_new__(0x14);
  *piVar2 = iVar3;
  piVar4 = *(int **)(DAT_0012c770 + 0x12c748);
  for (iVar1 = 0; iVar1 != 4; iVar1 = iVar1 + 1) {
    iVar3 = AbyssEngine::AERandom::nextInt(*piVar4);
    piVar2[iVar1 + 1] = iVar3;
  }
  return piVar2;
}

```

## target disasm
```
  0012c6f4: push {r4,r5,r6,r7,lr}
  0012c6f6: add r7,sp,#0xc
  0012c6f8: push {r8,r9,r11}
  0012c6fc: mov r4,r1
  0012c6fe: cmp r2,#0x3
  0012c700: bne 0x0012c718
  0012c702: ldr r0,[0x0012c768]
  0012c704: movs r1,#0x4
  0012c706: add r0,pc
  0012c708: ldr r0,[r0,#0x0]
  0012c70a: ldr r0,[r0,#0x0]
  0012c70c: blx 0x00071848
  0012c710: mov r2,r0
  0012c712: cmp r0,#0x0
  0012c714: it ne
  0012c716: mov.ne r2,#0x2
  0012c718: mov r0,r2
  0012c71a: ldr r6,[0x0012c76c]
  0012c71c: cmp r2,#0x0
  0012c71e: it eq
  0012c720: mov.eq r0,#0xa
  0012c722: cmp r4,#0x0
  0012c724: it ne
  0012c726: mov.ne r0,r2
  0012c728: subs r4,r0,#0x5
  0012c72a: add r6,pc
  0012c72c: it ne
  0012c72e: mov.ne r4,r0
  0012c730: movs r0,#0x14
  0012c732: blx 0x0006ec08
  0012c736: mov r5,r0
  0012c738: mov r8,r0
  0012c73a: stmia r5!,{r4}
  0012c73c: add.w r9,r6,r4, lsl #0x4
  0012c740: ldr r0,[0x0012c770]
  0012c742: movs r4,#0x0
  0012c744: add r0,pc
  0012c746: ldr r6,[r0,#0x0]
  0012c748: b 0x0012c75a
  0012c74a: ldr.w r1,[r9,r4,lsl #0x2]
  0012c74e: ldr r0,[r6,#0x0]
  0012c750: blx 0x00071848
  0012c754: str.w r0,[r5,r4,lsl #0x2]
  0012c758: adds r4,#0x1
  0012c75a: cmp r4,#0x4
  0012c75c: bne 0x0012c74a
  0012c75e: mov r0,r8
  0012c760: pop.w {r8,r9,r11}
  0012c764: pop {r4,r5,r6,r7,pc}
```
