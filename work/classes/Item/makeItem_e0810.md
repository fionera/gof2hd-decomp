# Item::makeItem
elf 0xe0810 body 18
Sig: undefined __stdcall makeItem(void)

## decompile
```c

/* Item::makeItem() */

void Item::makeItem(void)

{
  Item *in_r0;
  int iVar1;
  __fn *in_r1;
  void *in_r2;
  int in_r3;
  void *unaff_r4;
  undefined4 uVar2;
  
  uVar2 = *(undefined4 *)(in_r0 + 0x18);
  iVar1 = clone(in_r0,in_r1,in_r2,in_r3,unaff_r4);
  *(undefined4 *)(iVar1 + 0x18) = uVar2;
  *(undefined4 *)(iVar1 + 0x34) = 1;
  return;
}

```
## target disasm
```
  000f0810: push {r4,r6,r7,lr}
  000f0812: add r7,sp,#0x8
  000f0814: ldr r4,[r0,#0x18]
  000f0816: blx 0x000734c8
  000f081a: movs r1,#0x1
  000f081c: str r4,[r0,#0x18]
  000f081e: str r1,[r0,#0x34]
  000f0820: pop {r4,r6,r7,pc}
```
