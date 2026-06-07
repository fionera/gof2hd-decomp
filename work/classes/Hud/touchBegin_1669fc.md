# Hud::touchBegin
elf 0x1669fc body 186
Sig: undefined __stdcall touchBegin(uint param_1, uint param_2, void * param_3)

## decompile
```c

/* Hud::touchBegin(unsigned int, unsigned int, void*) */

undefined4 Hud::touchBegin(uint param_1,uint param_2,void *param_3)

{
  uint uVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  int in_r3;
  uint uVar5;
  
  uVar1 = touchedElement((Hud *)param_1,param_2,(uint)param_3);
  if (uVar1 == 0) {
    for (iVar3 = 0; iVar3 != 0x19; iVar3 = iVar3 + 1) {
      iVar4 = *(int *)(*(int *)(param_1 + 0x28c) + 4);
      if (*(int *)(iVar4 + iVar3 * 4) == in_r3) {
        *(uint *)(param_1 + 0x284) =
             *(uint *)(param_1 + 0x284) & ~*(uint *)(*(int *)(param_1 + 0x290) + iVar3 * 4);
        *(undefined4 *)(*(int *)(param_1 + 0x290) + iVar3 * 4) = 0;
        *(undefined4 *)(iVar4 + iVar3 * 4) = 0;
      }
    }
  }
  else {
    iVar3 = *(int *)(*(int *)(param_1 + 0x28c) + 4);
    for (uVar2 = 0; uVar2 < 0x19; uVar2 = uVar2 + 1) {
      if (*(int *)(iVar3 + uVar2 * 4) == in_r3) {
        uVar5 = *(uint *)(*(int *)(param_1 + 0x290) + uVar2 * 4);
        if (uVar1 == uVar5) {
          uVar5 = *(uint *)(param_1 + 0x284);
        }
        else {
          uVar5 = *(uint *)(param_1 + 0x284) & ~uVar5;
        }
        *(uint *)(param_1 + 0x284) = uVar5 | uVar1;
        *(uint *)(*(int *)(param_1 + 0x290) + uVar2 * 4) = uVar1;
        goto LAB_00176aac;
      }
    }
    for (uVar2 = 0; uVar2 < 0x19; uVar2 = uVar2 + 1) {
      if (*(int *)(iVar3 + uVar2 * 4) == 0) {
        *(int *)(iVar3 + uVar2 * 4) = in_r3;
        *(uint *)(*(int *)(param_1 + 0x290) + uVar2 * 4) = uVar1;
        *(uint *)(param_1 + 0x284) = uVar1 | *(uint *)(param_1 + 0x284);
        break;
      }
    }
  }
LAB_00176aac:
  return *(undefined4 *)(param_1 + 0x284);
}

```

## target disasm
```
  001769fc: push {r4,r5,r6,r7,lr}
  001769fe: add r7,sp,#0xc
  00176a00: push.w r11
  00176a04: mov r6,r3
  00176a06: mov r4,r0
  00176a08: blx 0x00077bcc
  00176a0c: cbz r0,0x00176a38
  00176a0e: ldr.w r1,[r4,#0x28c]
  00176a12: ldr r2,[r1,#0x4]
  00176a14: movs r1,#0x0
  00176a16: b 0x00176a22
  00176a18: ldr.w r3,[r2,r1,lsl #0x2]
  00176a1c: cmp r3,r6
  00176a1e: beq 0x00176a70
  00176a20: adds r1,#0x1
  00176a22: cmp r1,#0x19
  00176a24: bcc 0x00176a18
  00176a26: movs r1,#0x0
  00176a28: b 0x00176a32
  00176a2a: ldr.w r3,[r2,r1,lsl #0x2]
  00176a2e: cbz r3,0x00176a96
  00176a30: adds r1,#0x1
  00176a32: cmp r1,#0x18
  00176a34: bls 0x00176a2a
  00176a36: b 0x00176aac
  00176a38: mov.w r12,#0x0
  00176a3c: movs r1,#0x0
  00176a3e: b 0x00176a6a
  00176a40: ldr.w r2,[r4,#0x28c]
  00176a44: ldr r2,[r2,#0x4]
  00176a46: ldr.w r3,[r2,r1,lsl #0x2]
  00176a4a: cmp r3,r6
  00176a4c: bne 0x00176a68
  00176a4e: ldr.w r0,[r4,#0x290]
  00176a52: ldr.w r3,[r4,#0x284]
  00176a56: ldr.w r5,[r0,r1,lsl #0x2]
  00176a5a: bics r3,r5
  00176a5c: str.w r3,[r4,#0x284]
  00176a60: str.w r12,[r0,r1,lsl #0x2]
  00176a64: str.w r12,[r2,r1,lsl #0x2]
  00176a68: adds r1,#0x1
  00176a6a: cmp r1,#0x19
  00176a6c: bne 0x00176a40
  00176a6e: b 0x00176aac
  00176a70: ldr.w r2,[r4,#0x290]
  00176a74: ldr.w r3,[r2,r1,lsl #0x2]
  00176a78: cmp r0,r3
  00176a7a: itte ne
  00176a7c: ldr.ne.w r5,[r4,#0x284]
  00176a80: bic.ne.w r3,r5,r3
  00176a84: ldr.eq.w r3,[r4,#0x284]
  00176a88: add.w r5,r4,#0x284
  00176a8c: orrs r3,r0
  00176a8e: str r3,[r5,#0x0]
  00176a90: str.w r0,[r2,r1,lsl #0x2]
  00176a94: b 0x00176aac
  00176a96: str.w r6,[r2,r1,lsl #0x2]
  00176a9a: ldr.w r2,[r4,#0x290]
  00176a9e: str.w r0,[r2,r1,lsl #0x2]
  00176aa2: ldr.w r1,[r4,#0x284]
  00176aa6: orrs r0,r1
  00176aa8: str.w r0,[r4,#0x284]
  00176aac: ldr.w r0,[r4,#0x284]
  00176ab0: pop.w r11
  00176ab4: pop {r4,r5,r6,r7,pc}
```
