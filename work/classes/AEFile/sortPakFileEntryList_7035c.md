# AEFile::sortPakFileEntryList
elf 0x7035c body 90
Sig: undefined __stdcall sortPakFileEntryList(void)

## decompile
```c

/* AEFile::sortPakFileEntryList() */

void AEFile::sortPakFileEntryList(void)

{
  uint *puVar1;
  int iVar2;
  int iVar3;
  uint *puVar4;
  int iVar5;
  int *piVar6;
  
  piVar6 = *(int **)(DAT_000803b8 + 0x80366);
  iVar5 = *(int *)*piVar6;
  if (iVar5 != 0) {
    while (iVar5 = iVar5 + -1, -1 < iVar5) {
      for (iVar3 = 0; iVar5 != iVar3; iVar3 = iVar3 + 1) {
        iVar2 = *(int *)(*piVar6 + 4);
        puVar4 = *(uint **)(iVar2 + iVar3 * 4);
        puVar1 = *(uint **)(iVar2 + iVar3 * 4 + 4);
        if (*puVar1 < *puVar4) {
          *(uint **)(iVar2 + iVar3 * 4) = puVar1;
          *(uint **)(*(int *)(*piVar6 + 4) + iVar3 * 4 + 4) = puVar4;
        }
      }
    }
  }
  return;
}

```
## target disasm
```
  0008035c: push {r4,r5,r7,lr}
  0008035e: add r7,sp,#0x8
  00080360: ldr r0,[0x000803b8]
  00080362: add r0,pc
  00080364: ldr.w lr,[r0,#0x0]
  00080368: ldr.w r0,[lr,#0x0]
  0008036c: ldr.w r12,[r0,#0x0]
  00080370: cmp.w r12,#0x0
  00080374: bne 0x000803aa
  00080376: b 0x000803b4
  00080378: movs r2,#0x0
  0008037a: b 0x000803a6
  0008037c: ldr.w r0,[lr,#0x0]
  00080380: ldr r1,[r0,#0x4]
  00080382: add.w r0,r1,r2, lsl #0x2
  00080386: ldr.w r3,[r1,r2,lsl #0x2]
  0008038a: ldr r0,[r0,#0x4]
  0008038c: ldr r4,[r0,#0x0]
  0008038e: ldr r5,[r3,#0x0]
  00080390: cmp r5,r4
  00080392: bls 0x000803a4
  00080394: str.w r0,[r1,r2,lsl #0x2]
  00080398: ldr.w r0,[lr,#0x0]
  0008039c: ldr r0,[r0,#0x4]
  0008039e: add.w r0,r0,r2, lsl #0x2
  000803a2: str r3,[r0,#0x4]
  000803a4: adds r2,#0x1
  000803a6: cmp r12,r2
  000803a8: bne 0x0008037c
  000803aa: sub.w r12,r12,#0x1
  000803ae: cmp.w r12,#0x0
  000803b2: bge 0x00080378
  000803b4: pop {r4,r5,r7,pc}
```
