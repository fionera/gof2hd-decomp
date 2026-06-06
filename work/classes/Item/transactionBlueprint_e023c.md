# Item::transactionBlueprint
elf 0xe023c body 50
Sig: undefined __stdcall transactionBlueprint(bool param_1, int param_2)

## decompile
```c

/* Item::transactionBlueprint(bool, int) */

int Item::transactionBlueprint(bool param_1,int param_2)

{
  uint uVar1;
  
  uVar1 = (uint)param_1;
  if (param_2 == 0) {
    if (0 < *(int *)(uVar1 + 0x34)) {
      *(int *)(uVar1 + 0x34) = *(int *)(uVar1 + 0x34) + -1;
      *(int *)(uVar1 + 0x3c) = *(int *)(uVar1 + 0x3c) + 1;
      return *(int *)(uVar1 + 0x18);
    }
  }
  else if (0 < *(int *)(uVar1 + 0x3c)) {
    *(int *)(uVar1 + 0x3c) = *(int *)(uVar1 + 0x3c) + -1;
    *(int *)(uVar1 + 0x34) = *(int *)(uVar1 + 0x34) + 1;
    return -*(int *)(uVar1 + 0x18);
  }
  return 0;
}

```
## target disasm
```
  000f023c: cbz r1,0x000f0254
  000f023e: ldr r1,[r0,#0x3c]
  000f0240: cmp r1,#0x1
  000f0242: blt 0x000f0268
  000f0244: ldr r3,[r0,#0x34]
  000f0246: subs r1,#0x1
  000f0248: ldr r2,[r0,#0x18]
  000f024a: str r1,[r0,#0x3c]
  000f024c: adds r1,r3,#0x1
  000f024e: str r1,[r0,#0x34]
  000f0250: rsbs r1,r2
  000f0252: b 0x000f026a
  000f0254: ldr r2,[r0,#0x34]
  000f0256: cmp r2,#0x1
  000f0258: blt 0x000f0268
  000f025a: ldr r1,[r0,#0x18]
  000f025c: subs r2,#0x1
  000f025e: ldr r3,[r0,#0x3c]
  000f0260: str r2,[r0,#0x34]
  000f0262: adds r2,r3,#0x1
  000f0264: str r2,[r0,#0x3c]
  000f0266: b 0x000f026a
  000f0268: movs r1,#0x0
  000f026a: mov r0,r1
  000f026c: bx lr
```
