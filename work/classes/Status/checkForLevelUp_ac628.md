# Status::checkForLevelUp
elf 0xac628 body 94
Sig: undefined __stdcall checkForLevelUp(void)

## decompile
```c

/* Status::checkForLevelUp() */

void Status::checkForLevelUp(void)

{
  int in_r0;
  int iVar1;
  int iVar2;
  int iVar3;
  undefined4 in_r2;
  int iVar4;
  undefined4 in_r3;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  undefined4 uVar9;
  int iVar10;
  
  uVar9 = *(undefined4 *)(in_r0 + 0xa0);
  iVar7 = *(int *)(in_r0 + 0xa4);
  iVar6 = *(int *)(in_r0 + 0x1c0);
  iVar1 = __aeabi_idiv(*(undefined4 *)(in_r0 + 0xd4),3,in_r2,in_r3,&stack0xfffffff8);
  iVar8 = *(int *)(in_r0 + 0x1d0);
  iVar10 = *(int *)(in_r0 + 0x1e8);
  iVar5 = *(int *)(in_r0 + 0x1c4);
  iVar2 = __aeabi_idiv(uVar9,0x32);
  iVar4 = DAT_000bc688 + 0xbc66a;
  for (iVar3 = 0; iVar3 != 0x15; iVar3 = iVar3 + 1) {
    if (*(int *)(iVar4 + iVar3 * 4) <= iVar2 + iVar6 + iVar1 + iVar7 + iVar5 * 2 + iVar10 + iVar8) {
      *(int *)(in_r0 + 0x1c8) = iVar3;
    }
  }
  return;
}

```
## target disasm
```
  000bc628: push {r4,r5,r6,r7,lr}
  000bc62a: add r7,sp,#0xc
  000bc62c: push {r7,r8,r9,r10,r11}
  000bc630: mov r4,r0
  000bc632: ldrd r10,r8,[r0,#0xa0]
  000bc636: ldr.w r0,[r0,#0xd4]
  000bc63a: movs r1,#0x3
  000bc63c: ldr.w r6,[r4,#0x1c0]
  000bc640: blx 0x0007198c
  000bc644: add r6,r0
  000bc646: mov r0,r10
  000bc648: movs r1,#0x32
  000bc64a: ldr.w r9,[r4,#0x1d0]
  000bc64e: ldr.w r11,[r4,#0x1e8]
  000bc652: ldr.w r5,[r4,#0x1c4]
  000bc656: blx 0x0007198c
  000bc65a: add r0,r6
  000bc65c: ldr r2,[0x000bc688]
  000bc65e: add r0,r8
  000bc660: movs r1,#0x0
  000bc662: add.w r0,r0,r5, lsl #0x1
  000bc666: add r2,pc
  000bc668: add r0,r11
  000bc66a: add r0,r9
  000bc66c: b 0x000bc67c
  000bc66e: ldr.w r3,[r2,r1,lsl #0x2]
  000bc672: cmp r0,r3
  000bc674: it ge
  000bc676: str.w.ge r1,[r4,#0x1c8]
  000bc67a: adds r1,#0x1
  000bc67c: cmp r1,#0x15
  000bc67e: bne 0x000bc66e
  000bc680: pop.w {r3,r8,r9,r10,r11}
  000bc684: pop {r4,r5,r6,r7,pc}
```
