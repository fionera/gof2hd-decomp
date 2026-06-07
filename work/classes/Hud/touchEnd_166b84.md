# Hud::touchEnd
elf 0x166b84 body 118
Sig: undefined __stdcall touchEnd(uint param_1, uint param_2, void * param_3)

## decompile
```c

/* Hud::touchEnd(unsigned int, unsigned int, void*) */

uint Hud::touchEnd(uint param_1,uint param_2,void *param_3)

{
  uint *puVar1;
  int iVar2;
  int iVar3;
  int in_r3;
  uint uVar4;
  uint uVar5;
  
  uVar4 = 0;
  for (iVar2 = 0; iVar2 != 0x19; iVar2 = iVar2 + 1) {
    iVar3 = *(int *)(*(int *)(param_1 + 0x28c) + 4);
    if (*(int *)(iVar3 + iVar2 * 4) == in_r3) {
      uVar4 = *(uint *)(*(int *)(param_1 + 0x290) + iVar2 * 4);
      *(uint *)(param_1 + 0x284) = *(uint *)(param_1 + 0x284) & ~uVar4;
      *(undefined4 *)(iVar3 + iVar2 * 4) = 0;
      *(undefined4 *)(*(int *)(param_1 + 0x290) + iVar2 * 4) = 0;
    }
  }
  if ((*(char *)(param_1 + 0x282) != '\0') &&
     (puVar1 = *(uint **)(param_1 + 0x18), puVar1 != (uint *)0x0)) {
    for (uVar5 = 0; uVar5 < *puVar1; uVar5 = uVar5 + 1) {
      TouchButton::OnTouchEnd(*(int *)(puVar1[1] + uVar5 * 4),param_2);
      puVar1 = *(uint **)(param_1 + 0x18);
    }
  }
  return uVar4;
}

```

## target disasm
```
  00176b84: push {r4,r5,r6,r7,lr}
  00176b86: add r7,sp,#0xc
  00176b88: push {r8,r9,r11}
  00176b8c: mov r9,r1
  00176b8e: mov r6,r0
  00176b90: mov r8,r2
  00176b92: movs r0,#0x0
  00176b94: movs r4,#0x0
  00176b96: movs r1,#0x0
  00176b98: b 0x00176bc8
  00176b9a: ldr.w r2,[r6,#0x28c]
  00176b9e: ldr r2,[r2,#0x4]
  00176ba0: ldr.w r5,[r2,r1,lsl #0x2]
  00176ba4: cmp r5,r3
  00176ba6: bne 0x00176bc6
  00176ba8: ldr.w r4,[r6,#0x290]
  00176bac: ldr.w r5,[r6,#0x284]
  00176bb0: ldr.w r4,[r4,r1,lsl #0x2]
  00176bb4: bics r5,r4
  00176bb6: str.w r5,[r6,#0x284]
  00176bba: str.w r0,[r2,r1,lsl #0x2]
  00176bbe: ldr.w r2,[r6,#0x290]
  00176bc2: str.w r0,[r2,r1,lsl #0x2]
  00176bc6: adds r1,#0x1
  00176bc8: cmp r1,#0x19
  00176bca: bne 0x00176b9a
  00176bcc: ldrb.w r0,[r6,#0x282]
  00176bd0: cbz r0,0x00176bf2
  00176bd2: ldr r0,[r6,#0x18]
  00176bd4: cbz r0,0x00176bf2
  00176bd6: movs r5,#0x0
  00176bd8: b 0x00176bec
  00176bda: ldr r0,[r0,#0x4]
  00176bdc: mov r1,r9
  00176bde: mov r2,r8
  00176be0: ldr.w r0,[r0,r5,lsl #0x2]
  00176be4: blx 0x00074788
  00176be8: ldr r0,[r6,#0x18]
  00176bea: adds r5,#0x1
  00176bec: ldr r1,[r0,#0x0]
  00176bee: cmp r5,r1
  00176bf0: bcc 0x00176bda
  00176bf2: mov r0,r4
  00176bf4: pop.w {r8,r9,r11}
  00176bf8: pop {r4,r5,r6,r7,pc}
```
