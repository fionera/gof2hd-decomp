# PlayerEgo::killLiberator
elf 0xa1a18 body 154
Sig: undefined __thiscall killLiberator(PlayerEgo * this)

## decompile
```c

/* PlayerEgo::killLiberator() */

void __thiscall PlayerEgo::killLiberator(PlayerEgo *this)

{
  int iVar1;
  int iVar2;
  uint *puVar3;
  uint uVar4;
  int *piVar5;
  uint uVar6;
  int *piVar7;
  
  piVar7 = *(int **)(DAT_000b1ab8 + 0xb1a26);
  iVar2 = *piVar7;
  piVar5 = *(int **)this->m_pPlayer;
  if ((((piVar5 != (int *)0x0) && (puVar3 = *(uint **)(piVar5[1] + 4), puVar3 != (uint *)0x0)) &&
      (*piVar5 != 0)) && ((uVar4 = *puVar3, uVar4 != 0 && (this[7].field_10 == (int *)0xb3)))) {
    for (uVar6 = 0; uVar6 < uVar4; uVar6 = uVar6 + 1) {
      iVar1 = *(int *)(puVar3[1] + uVar6 * 4);
      if (*(int *)(iVar1 + 0x58) == 0xb3) {
        **(undefined4 **)(iVar1 + 0x3c) = 0xffffffff;
        AbyssEngine::AEMath::Vector::operator=(*(Vector **)(iVar1 + 0xc),(Vector *)&stack0xffffffcc)
        ;
        puVar3 = *(uint **)(piVar5[1] + 4);
        *(undefined1 *)(*(int *)(puVar3[1] + uVar6 * 4) + 0x4c) = 0;
        uVar4 = *puVar3;
      }
    }
  }
  iVar2 = *piVar7 - iVar2;
  if (iVar2 == 0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar2);
}

```

## target disasm
```
  000b1a18: push {r4,r5,r6,r7,lr}
  000b1a1a: add r7,sp,#0xc
  000b1a1c: push {r3,r4,r5,r6,r7,r8,r9,r10,r11}
  000b1a20: ldr r1,[0x000b1ab8]
  000b1a22: add r1,pc
  000b1a24: ldr.w r9,[r1,#0x0]
  000b1a28: ldr.w r1,[r9,#0x0]
  000b1a2c: str r1,[sp,#0x10]
  000b1a2e: ldr r1,[r0,#0x0]
  000b1a30: ldr r5,[r1,#0x0]
  000b1a32: cbz r5,0x000b1a98
  000b1a34: ldr r1,[r5,#0x4]
  000b1a36: ldr r1,[r1,#0x4]
  000b1a38: cbz r1,0x000b1a98
  000b1a3a: ldr r2,[r5,#0x0]
  000b1a3c: cbz r2,0x000b1a98
  000b1a3e: ldr r2,[r1,#0x0]
  000b1a40: cbz r2,0x000b1a98
  000b1a42: ldr.w r0,[r0,#0x10c]
  000b1a46: movs r4,#0x0
  000b1a48: cmp r0,#0xb3
  000b1a4a: bne 0x000b1a9a
  000b1a4c: ldr r0,[0x000b1ab4]
  000b1a4e: add.w r8,sp,#0x4
  000b1a52: mov.w r10,#0xffffffff
  000b1a56: movs r6,#0x0
  000b1a58: mov r11,r0
  000b1a5a: b 0x000b1a90
  000b1a5c: ldr r0,[r1,#0x4]
  000b1a5e: ldr.w r0,[r0,r6,lsl #0x2]
  000b1a62: ldr r3,[r0,#0x58]
  000b1a64: cmp r3,#0xb3
  000b1a66: bne 0x000b1a8e
  000b1a68: ldr r1,[r0,#0x3c]
  000b1a6a: str.w r10,[r1,#0x0]
  000b1a6e: mov r1,r8
  000b1a70: strd r11,r11,[sp,#0x4]
  000b1a74: str.w r11,[sp,#0xc]
  000b1a78: ldr r0,[r0,#0xc]
  000b1a7a: blx 0x0006eb3c
  000b1a7e: ldr r0,[r5,#0x4]
  000b1a80: ldr r1,[r0,#0x4]
  000b1a82: ldr r0,[r1,#0x4]
  000b1a84: ldr.w r0,[r0,r6,lsl #0x2]
  000b1a88: strb.w r4,[r0,#0x4c]
  000b1a8c: ldr r2,[r1,#0x0]
  000b1a8e: adds r6,#0x1
  000b1a90: cmp r6,r2
  000b1a92: bcc 0x000b1a5c
  000b1a94: movs r4,#0x1
  000b1a96: b 0x000b1a9a
  000b1a98: movs r4,#0x0
  000b1a9a: ldr r0,[sp,#0x10]
  000b1a9c: ldr.w r1,[r9,#0x0]
  000b1aa0: subs r0,r1,r0
  000b1aa2: itttt eq
  000b1aa4: mov.eq r0,r4
  000b1aa6: add.eq sp,#0x14
  000b1aa8: pop.eq.w {r8,r9,r10,r11}
  000b1aac: pop.eq {r4,r5,r6,r7,pc}
  000b1aae: blx 0x0006e824
```
