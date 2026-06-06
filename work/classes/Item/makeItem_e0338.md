# Item::makeItem
elf 0xe0338 body 18
Sig: undefined __stdcall makeItem(int param_1)

## decompile
```c

/* Item::makeItem(int) */

void Item::makeItem(int param_1)

{
  int iVar1;
  __fn *in_r1;
  void *in_r2;
  int in_r3;
  void *unaff_r4;
  undefined4 uVar2;
  
  uVar2 = *(undefined4 *)(param_1 + 0x18);
  iVar1 = clone((Item *)param_1,in_r1,in_r2,in_r3,unaff_r4);
  *(undefined4 *)(iVar1 + 0x18) = uVar2;
  *(__fn **)(iVar1 + 0x34) = in_r1;
  return;
}

```
## target disasm
```
  000f0338: push {r4,r5,r7,lr}
  000f033a: add r7,sp,#0x8
  000f033c: mov r4,r1
  000f033e: ldr r5,[r0,#0x18]
  000f0340: blx 0x000734c8
  000f0344: str r5,[r0,#0x18]
  000f0346: str r4,[r0,#0x34]
  000f0348: pop {r4,r5,r7,pc}
```
