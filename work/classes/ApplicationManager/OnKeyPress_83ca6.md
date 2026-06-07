# ApplicationManager::OnKeyPress
elf 0x83ca6 body 242
Sig: undefined __thiscall OnKeyPress(ApplicationManager * this, int param_1)

## decompile
```c

/* AbyssEngine::ApplicationManager::OnKeyPress(int) */

void __thiscall AbyssEngine::ApplicationManager::OnKeyPress(ApplicationManager *this,int param_1)

{
  int iVar1;
  int *piVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  int iVar6;
  uint uVar7;
  uint uVar8;
  uint uVar9;
  
  uVar9 = 0;
  *(undefined4 *)(this + 0x98) = 0;
  piVar2 = *(int **)(this + 0x10);
  *(int *)(this + 8) = param_1;
  *(int *)(this + 0xc) = param_1 >> 0x1f;
  *(undefined4 *)(this + 0x9c) = 0;
  do {
    if (0x3f < uVar9) {
      uVar7 = 0;
      uVar8 = 0;
      uVar3 = 0;
      uVar4 = 0;
LAB_00093d7a:
      piVar2 = *(int **)(this + 0x18);
      if ((piVar2 != (int *)0x0) && (*(int *)(this + 0x3c) == 5)) {
        (**(code **)(*piVar2 + 0x10))(piVar2,piVar2,uVar3,uVar4,uVar7,uVar8);
      }
      return;
    }
    if (*piVar2 == param_1) {
      uVar5 = uVar9 - 0x20;
      uVar3 = 1 << (uVar9 & 0xff);
      if (-1 < (int)uVar5) {
        uVar3 = 0;
      }
      uVar7 = 0;
      uVar4 = 1 >> (0x20 - uVar9 & 0xff);
      uVar8 = 0;
      if (-1 < (int)uVar5) {
        uVar4 = 1 << (uVar5 & 0xff);
      }
      *(uint *)(this + 0x80) = *(uint *)(this + 0x80) | uVar3;
      *(uint *)(this + 0x84) = *(uint *)(this + 0x84) | uVar4;
      iVar1 = 0;
      for (uVar5 = 0; uVar5 < *(uint *)(this + 0x88); uVar5 = uVar5 + 2) {
        iVar6 = *(int *)(this + 0x8c) + iVar1;
        if (*(uint *)(iVar6 + 8) == uVar9 && *(int *)(iVar6 + 0xc) == 0) {
          uVar7 = uVar7 | *(uint *)(*(int *)(this + 0x8c) + iVar1);
          uVar8 = uVar8 | *(uint *)(iVar6 + 4);
          *(uint *)(this + 0x98) = uVar7;
          *(uint *)(this + 0x9c) = uVar8;
          *(uint *)(this + 0xa0) = *(uint *)(this + 0xa0) | uVar7;
          *(uint *)(this + 0xa4) = *(uint *)(this + 0xa4) | uVar8;
        }
        iVar1 = iVar1 + 0x10;
      }
      goto LAB_00093d7a;
    }
    piVar2 = piVar2 + 4;
    uVar9 = uVar9 + 1;
  } while( true );
}

```

## target disasm
```
  00093ca6: push {r4,r5,r6,r7,lr}
  00093ca8: add r7,sp,#0xc
  00093caa: push {r3,r4,r5,r6,r7,r8,r9,r10,r11}
  00093cae: mov.w r12,#0x0
  00093cb2: mov r5,r0
  00093cb4: str.w r12,[r0,#0x98]!
  00093cb8: asrs r3,r1,#0x1f
  00093cba: ldr.w r2,[r0,#-0x88]
  00093cbe: strd r1,r3,[r0,#-0x90]
  00093cc2: str r0,[sp,#0xc]
  00093cc4: str.w r12,[r0,#0x4]
  00093cc8: b 0x00093cd6
  00093cca: ldr r3,[r2,#0x0]
  00093ccc: cmp r3,r1
  00093cce: beq 0x00093cea
  00093cd0: adds r2,#0x10
  00093cd2: add.w r12,r12,#0x1
  00093cd6: cmp.w r12,#0x3f
  00093cda: bls 0x00093cca
  00093cdc: mov.w r10,#0x0
  00093ce0: mov.w r11,#0x0
  00093ce4: movs r2,#0x0
  00093ce6: movs r3,#0x0
  00093ce8: b 0x00093d7a
  00093cea: movs r1,#0x1
  00093cec: sub.w r4,r12,#0x20
  00093cf0: lsl.w r2,r1,r12
  00093cf4: cmp r4,#0x0
  00093cf6: it ge
  00093cf8: mov.ge r2,#0x0
  00093cfa: ldrd r3,r0,[r5,#0x80]
  00093cfe: ldr.w r6,[r5,#0x88]
  00093d02: cmp r4,#0x0
  00093d04: str r6,[sp,#0x10]
  00093d06: orr.w r6,r3,r2
  00093d0a: rsb.w r3,r12,#0x20
  00093d0e: str r2,[sp,#0x8]
  00093d10: add.w r9,r5,#0xa0
  00093d14: mov.w r10,#0x0
  00093d18: lsr.w r3,r1,r3
  00093d1c: mov.w r11,#0x0
  00093d20: it ge
  00093d22: lsl.ge.w r3,r1,r4
  00093d26: orr.w r1,r0,r3
  00093d2a: strd r6,r1,[r5,#0x80]
  00093d2e: movs r1,#0x0
  00093d30: movs r6,#0x0
  00093d32: b 0x00093d72
  00093d34: ldr.w r4,[r5,#0x8c]
  00093d38: mov r0,r5
  00093d3a: adds r5,r4,r1
  00093d3c: ldrd lr,r8,[r5,#0x8]
  00093d40: eor.w r2,lr,r12
  00093d44: orrs.w r2,r2,r8
  00093d48: bne 0x00093d6c
  00093d4a: ldr r2,[r4,r1]
  00093d4c: ldr r5,[r5,#0x4]
  00093d4e: orr.w r10,r10,r2
  00093d52: ldr r2,[sp,#0xc]
  00093d54: orr.w r11,r11,r5
  00093d58: strd r10,r11,[r2,#0x0]
  00093d5c: ldrd r2,r5,[r9,#0x0]
  00093d60: orr.w r5,r5,r11
  00093d64: orr.w r2,r2,r10
  00093d68: strd r2,r5,[r9,#0x0]
  00093d6c: adds r1,#0x10
  00093d6e: adds r6,#0x2
  00093d70: mov r5,r0
  00093d72: ldr r0,[sp,#0x10]
  00093d74: cmp r6,r0
  00093d76: bcc 0x00093d34
  00093d78: ldr r2,[sp,#0x8]
  00093d7a: ldr r1,[r5,#0x18]
  00093d7c: cbz r1,0x00093d90
  00093d7e: ldr r0,[r5,#0x3c]
  00093d80: cmp r0,#0x5
  00093d82: bne 0x00093d90
  00093d84: ldr r0,[r1,#0x0]
  00093d86: ldr r6,[r0,#0x10]
  00093d88: mov r0,r1
  00093d8a: strd r10,r11,[sp,#0x0]
  00093d8e: blx r6
  00093d90: add sp,#0x14
  00093d92: pop.w {r8,r9,r10,r11}
  00093d96: pop {r4,r5,r6,r7,pc}
```
