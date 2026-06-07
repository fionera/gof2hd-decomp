# ApplicationManager::OnKeyRelease
elf 0x83d98 body 236
Sig: undefined __thiscall OnKeyRelease(ApplicationManager * this, int param_1)

## decompile
```c

/* AbyssEngine::ApplicationManager::OnKeyRelease(int) */

void __thiscall AbyssEngine::ApplicationManager::OnKeyRelease(ApplicationManager *this,int param_1)

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
  *(undefined4 *)(this + 0x9c) = 0;
  do {
    if (0x3f < uVar9) {
      uVar7 = 0;
      uVar8 = 0;
      uVar3 = 0;
      uVar4 = 0;
LAB_00093e66:
      piVar2 = *(int **)(this + 0x18);
      if ((piVar2 != (int *)0x0) && (*(int *)(this + 0x3c) == 5)) {
        (**(code **)(*piVar2 + 0x14))(piVar2,piVar2,uVar3,uVar4,uVar7,uVar8);
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
      *(uint *)(this + 0x80) = *(uint *)(this + 0x80) & ~uVar3;
      *(uint *)(this + 0x84) = *(uint *)(this + 0x84) & ~uVar4;
      iVar1 = 0;
      for (uVar5 = 0; uVar5 < *(uint *)(this + 0x88); uVar5 = uVar5 + 2) {
        iVar6 = *(int *)(this + 0x8c) + iVar1;
        if (*(uint *)(iVar6 + 8) == uVar9 && *(int *)(iVar6 + 0xc) == 0) {
          uVar7 = uVar7 | *(uint *)(*(int *)(this + 0x8c) + iVar1);
          uVar8 = uVar8 | *(uint *)(iVar6 + 4);
          *(uint *)(this + 0x98) = uVar7;
          *(uint *)(this + 0x9c) = uVar8;
          *(uint *)(this + 0xa0) = *(uint *)(this + 0xa0) & ~uVar7;
          *(uint *)(this + 0xa4) = *(uint *)(this + 0xa4) & ~uVar8;
        }
        iVar1 = iVar1 + 0x10;
      }
      goto LAB_00093e66;
    }
    piVar2 = piVar2 + 4;
    uVar9 = uVar9 + 1;
  } while( true );
}

```

## target disasm
```
  00093d98: push {r4,r5,r6,r7,lr}
  00093d9a: add r7,sp,#0xc
  00093d9c: push {r3,r4,r5,r6,r7,r8,r9,r10,r11}
  00093da0: mov.w r12,#0x0
  00093da4: mov r5,r0
  00093da6: str.w r12,[r0,#0x98]!
  00093daa: str r0,[sp,#0xc]
  00093dac: ldr.w r2,[r0,#-0x88]
  00093db0: str.w r12,[r0,#0x4]
  00093db4: b 0x00093dc2
  00093db6: ldr r3,[r2,#0x0]
  00093db8: cmp r3,r1
  00093dba: beq 0x00093dd6
  00093dbc: adds r2,#0x10
  00093dbe: add.w r12,r12,#0x1
  00093dc2: cmp.w r12,#0x3f
  00093dc6: bls 0x00093db6
  00093dc8: mov.w r10,#0x0
  00093dcc: mov.w r11,#0x0
  00093dd0: movs r2,#0x0
  00093dd2: movs r3,#0x0
  00093dd4: b 0x00093e66
  00093dd6: movs r1,#0x1
  00093dd8: sub.w r4,r12,#0x20
  00093ddc: lsl.w r2,r1,r12
  00093de0: cmp r4,#0x0
  00093de2: it ge
  00093de4: mov.ge r2,#0x0
  00093de6: ldrd r3,r0,[r5,#0x80]
  00093dea: ldr.w r6,[r5,#0x88]
  00093dee: cmp r4,#0x0
  00093df0: str r6,[sp,#0x10]
  00093df2: bic.w r6,r3,r2
  00093df6: rsb.w r3,r12,#0x20
  00093dfa: str r2,[sp,#0x8]
  00093dfc: add.w r9,r5,#0xa0
  00093e00: mov.w r10,#0x0
  00093e04: lsr.w r3,r1,r3
  00093e08: mov.w r11,#0x0
  00093e0c: it ge
  00093e0e: lsl.ge.w r3,r1,r4
  00093e12: bic.w r1,r0,r3
  00093e16: strd r6,r1,[r5,#0x80]
  00093e1a: movs r1,#0x0
  00093e1c: movs r6,#0x0
  00093e1e: b 0x00093e5e
  00093e20: ldr.w r4,[r5,#0x8c]
  00093e24: mov r0,r5
  00093e26: adds r5,r4,r1
  00093e28: ldrd lr,r8,[r5,#0x8]
  00093e2c: eor.w r2,lr,r12
  00093e30: orrs.w r2,r2,r8
  00093e34: bne 0x00093e58
  00093e36: ldr r2,[r4,r1]
  00093e38: ldr r5,[r5,#0x4]
  00093e3a: orr.w r10,r10,r2
  00093e3e: ldr r2,[sp,#0xc]
  00093e40: orr.w r11,r11,r5
  00093e44: strd r10,r11,[r2,#0x0]
  00093e48: ldrd r2,r5,[r9,#0x0]
  00093e4c: bic.w r5,r5,r11
  00093e50: bic.w r2,r2,r10
  00093e54: strd r2,r5,[r9,#0x0]
  00093e58: adds r1,#0x10
  00093e5a: adds r6,#0x2
  00093e5c: mov r5,r0
  00093e5e: ldr r0,[sp,#0x10]
  00093e60: cmp r6,r0
  00093e62: bcc 0x00093e20
  00093e64: ldr r2,[sp,#0x8]
  00093e66: ldr r1,[r5,#0x18]
  00093e68: cbz r1,0x00093e7c
  00093e6a: ldr r0,[r5,#0x3c]
  00093e6c: cmp r0,#0x5
  00093e6e: bne 0x00093e7c
  00093e70: ldr r0,[r1,#0x0]
  00093e72: ldr r6,[r0,#0x14]
  00093e74: mov r0,r1
  00093e76: strd r10,r11,[sp,#0x0]
  00093e7a: blx r6
  00093e7c: add sp,#0x14
  00093e7e: pop.w {r8,r9,r10,r11}
  00093e82: pop {r4,r5,r6,r7,pc}
```
