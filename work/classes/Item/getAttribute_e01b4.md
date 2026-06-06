# Item::getAttribute
elf 0xe01b4 body 46
Sig: undefined __stdcall getAttribute(int param_1)

## decompile
```c

/* Item::getAttribute(int) */

undefined4 Item::getAttribute(int param_1)

{
  int in_r1;
  uint uVar1;
  uint uVar2;
  
  uVar2 = 0;
  while( true ) {
    if (**(uint **)(param_1 + 0x30) <= uVar2) {
      return DAT_000f01e4;
    }
    uVar1 = (*(uint **)(param_1 + 0x30))[1];
    if (*(int *)(uVar1 + uVar2 * 4) == in_r1) break;
    uVar2 = uVar2 + 2;
  }
  return *(undefined4 *)(uVar1 + uVar2 * 4 + 4);
}

```
## target disasm
```
  000f01b4: push {r4,r6,r7,lr}
  000f01b6: add r7,sp,#0x8
  000f01b8: ldr.w lr,[r0,#0x30]
  000f01bc: movs r3,#0x0
  000f01be: ldr r0,[0x000f01e4]
  000f01c0: ldr.w r12,[lr,#0x0]
  000f01c4: b 0x000f01d4
  000f01c6: ldr.w r2,[lr,#0x4]
  000f01ca: ldr.w r4,[r2,r3,lsl #0x2]
  000f01ce: cmp r4,r1
  000f01d0: beq 0x000f01da
  000f01d2: adds r3,#0x2
  000f01d4: cmp r3,r12
  000f01d6: bcc 0x000f01c6
  000f01d8: pop {r4,r6,r7,pc}
  000f01da: add.w r0,r2,r3, lsl #0x2
  000f01de: ldr r0,[r0,#0x4]
  000f01e0: pop {r4,r6,r7,pc}
```
