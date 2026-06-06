# Level::getDockingTarget
elf 0xc6726 body 54
Sig: undefined __stdcall getDockingTarget(int param_1)

## decompile
```c

/* Level::getDockingTarget(int) */

int Level::getDockingTarget(int param_1)

{
  int iVar1;
  int in_r1;
  uint uVar2;
  int iVar3;
  uint *puVar4;
  
  puVar4 = *(uint **)(param_1 + 0xf8);
  if (puVar4 != (uint *)0x0) {
    iVar3 = 0;
    for (uVar2 = 0; uVar2 < *puVar4; uVar2 = uVar2 + 1) {
      iVar1 = *(int *)(puVar4[1] + uVar2 * 4);
      if (*(char *)(iVar1 + 0x70) != '\0') {
        if (iVar3 == in_r1) {
          return iVar1;
        }
        iVar3 = iVar3 + 1;
      }
    }
  }
  return 0;
}

```
## target disasm
```
  000d6726: push {r4,r6,r7,lr}
  000d6728: add r7,sp,#0x8
  000d672a: ldr.w lr,[r0,#0xf8]
  000d672e: cmp.w lr,#0x0
  000d6732: beq 0x000d6758
  000d6734: ldr.w r12,[lr,#0x0]
  000d6738: movs r3,#0x0
  000d673a: movs r2,#0x0
  000d673c: b 0x000d6754
  000d673e: ldr.w r0,[lr,#0x4]
  000d6742: ldr.w r0,[r0,r2,lsl #0x2]
  000d6746: ldrb.w r4,[r0,#0x70]
  000d674a: cbz r4,0x000d6752
  000d674c: cmp r3,r1
  000d674e: beq 0x000d675a
  000d6750: adds r3,#0x1
  000d6752: adds r2,#0x1
  000d6754: cmp r2,r12
  000d6756: bcc 0x000d673e
  000d6758: movs r0,#0x0
  000d675a: pop {r4,r6,r7,pc}
```
