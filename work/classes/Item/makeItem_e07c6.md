# Item::makeItem
elf 0xe07c6 body 18
Sig: undefined __stdcall makeItem(int param_1, int param_2)

## decompile
```c

/* Item::makeItem(int, int) */

void Item::makeItem(int param_1,int param_2)

{
  int iVar1;
  void *in_r2;
  int in_r3;
  void *unaff_r4;
  
  iVar1 = clone((Item *)param_1,(__fn *)param_2,in_r2,in_r3,unaff_r4);
  *(void **)(iVar1 + 0x18) = in_r2;
  *(int *)(iVar1 + 0x34) = param_2;
  return;
}

```
## target disasm
```
  000f07c6: push {r4,r5,r7,lr}
  000f07c8: add r7,sp,#0x8
  000f07ca: mov r4,r2
  000f07cc: mov r5,r1
  000f07ce: blx 0x000734c8
  000f07d2: str r4,[r0,#0x18]
  000f07d4: str r5,[r0,#0x34]
  000f07d6: pop {r4,r5,r7,pc}
```
