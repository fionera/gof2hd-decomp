# PaintCanvas::MaterialCreate
elf 0x79e4c body 206
Sig: undefined __thiscall MaterialCreate(PaintCanvas * this, ushort param_1, uint * param_2)

## decompile
```c

/* AbyssEngine::PaintCanvas::MaterialCreate(unsigned short, unsigned int&) */

void __thiscall
AbyssEngine::PaintCanvas::MaterialCreate(PaintCanvas *this,ushort param_1,uint *param_2)

{
  ushort uVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  Material *this_00;
  int iVar5;
  int iVar6;
  int *piVar7;
  int iVar8;
  
  piVar7 = *(int **)(DAT_00089f28 + 0x89e5e);
  iVar2 = *piVar7;
  iVar3 = FindResource(this,param_1);
  if (iVar3 != 0) {
    uVar4 = *(uint *)(iVar3 + 8);
    if (uVar4 == 0xffffffff) {
      iVar8 = *(int *)(iVar3 + 0xc);
      this_00 = operator_new(0x74);
      Material::Material(this_00);
      for (uVar4 = 0; uVar4 < 8; uVar4 = uVar4 + 1) {
        uVar1 = *(ushort *)(iVar8 + uVar4 * 2);
        if (uVar1 != 0xffff) {
          iVar5 = FindResource(this,uVar1);
          if (iVar5 == 0) break;
          iVar6 = *(int *)(iVar5 + 8);
          if (iVar6 == -1) {
            TextureCreate((ushort)this,(uint *)(uint)*(ushort *)(iVar8 + uVar4 * 2),true);
            iVar6 = *(int *)(iVar5 + 8);
          }
          *(int *)(this_00 + uVar4 * 4) = iVar6;
        }
      }
      *(undefined4 *)(this_00 + 0x20) = *(undefined4 *)(iVar8 + 0x10);
      *(undefined4 *)(this_00 + 0x24) = *(undefined4 *)(iVar8 + 0x14);
      *(undefined4 *)(this_00 + 0x28) = *(undefined4 *)(iVar8 + 0x18);
      AEMath::Vector::operator=((Vector *)(this_00 + 0x68),(Vector *)(iVar8 + 0x1c));
      ArrayAdd<AbyssEngine::Material*>(this_00,(Array *)(this + 0x174));
      uVar4 = *(int *)(this + 0x174) - 1;
      *(uint *)(iVar3 + 8) = uVar4;
    }
    *param_2 = uVar4;
  }
  if (*piVar7 != iVar2) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  00089e4c: push {r4,r5,r6,r7,lr}
  00089e4e: add r7,sp,#0xc
  00089e50: push {r3,r4,r5,r6,r7,r8,r9,r10,r11}
  00089e54: mov r5,r0
  00089e56: ldr r0,[0x00089f28]
  00089e58: mov r4,r2
  00089e5a: add r0,pc
  00089e5c: ldr r6,[r0,#0x0]
  00089e5e: ldr r0,[r6,#0x0]
  00089e60: str r0,[sp,#0x10]
  00089e62: mov r0,r5
  00089e64: blx 0x0006fb98
  00089e68: mov r10,r0
  00089e6a: cmp r0,#0x0
  00089e6c: beq 0x00089f06
  00089e6e: ldr.w r0,[r10,#0x8]
  00089e72: adds r1,r0,#0x1
  00089e74: bne 0x00089f04
  00089e76: strd r6,r4,[sp,#0x4]
  00089e7a: movs r0,#0x74
  00089e7c: ldr.w r9,[r10,#0xc]
  00089e80: blx 0x0006eb24
  00089e84: mov r4,r0
  00089e86: blx 0x0006fc34
  00089e8a: movw r8,#0xffff
  00089e8e: mov.w r11,#0x0
  00089e92: b 0x00089ec8
  00089e94: ldrh.w r1,[r9,r11,lsl #0x1]
  00089e98: cmp r1,r8
  00089e9a: beq 0x00089ec4
  00089e9c: mov r0,r5
  00089e9e: blx 0x0006fb98
  00089ea2: mov r6,r0
  00089ea4: cbz r0,0x00089ece
  00089ea6: ldr r0,[r6,#0x8]
  00089ea8: adds r1,r0,#0x1
  00089eaa: bne 0x00089ec0
  00089eac: movs r0,#0x0
  00089eae: add r2,sp,#0xc
  00089eb0: str r0,[sp,#0xc]
  00089eb2: mov r0,r5
  00089eb4: ldrh.w r1,[r9,r11,lsl #0x1]
  00089eb8: movs r3,#0x1
  00089eba: blx 0x0006fbb0
  00089ebe: ldr r0,[r6,#0x8]
  00089ec0: str.w r0,[r4,r11,lsl #0x2]
  00089ec4: add.w r11,r11,#0x1
  00089ec8: cmp.w r11,#0x8
  00089ecc: bcc 0x00089e94
  00089ece: ldr.w r0,[r9,#0x10]
  00089ed2: add.w r1,r9,#0x1c
  00089ed6: str r0,[r4,#0x20]
  00089ed8: ldr.w r0,[r9,#0x14]
  00089edc: str r0,[r4,#0x24]
  00089ede: ldr.w r0,[r9,#0x18]
  00089ee2: str r0,[r4,#0x28]
  00089ee4: add.w r0,r4,#0x68
  00089ee8: blx 0x0006eb3c
  00089eec: add.w r1,r5,#0x174
  00089ef0: mov r0,r4
  00089ef2: blx 0x0006fc40
  00089ef6: ldr.w r0,[r5,#0x174]
  00089efa: subs r0,#0x1
  00089efc: str.w r0,[r10,#0x8]
  00089f00: ldrd r6,r4,[sp,#0x4]
  00089f04: str r0,[r4,#0x0]
  00089f06: ldr r0,[sp,#0x10]
  00089f08: ldr r1,[r6,#0x0]
  00089f0a: subs r0,r1,r0
  00089f0c: ittt eq
  00089f0e: add.eq sp,#0x14
  00089f10: pop.eq.w {r8,r9,r10,r11}
  00089f14: pop.eq {r4,r5,r6,r7,pc}
  00089f16: blx 0x0006e824
```
