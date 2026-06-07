# PaintCanvas::SpriteSystemCreate
elf 0x7a71c body 126
Sig: undefined __thiscall SpriteSystemCreate(PaintCanvas * this, ushort param_1, bool param_2, uint * param_3)

## decompile
```c

/* AbyssEngine::PaintCanvas::SpriteSystemCreate(unsigned short, bool, unsigned int&) */

void __thiscall
AbyssEngine::PaintCanvas::SpriteSystemCreate
          (PaintCanvas *this,ushort param_1,bool param_2,uint *param_3)

{
  int iVar1;
  uint uVar2;
  int *piVar3;
  SpriteSystem *local_20;
  int local_1c;
  
  piVar3 = *(int **)(DAT_0008a79c + 0x8a732);
  local_1c = *piVar3;
  local_20 = (SpriteSystem *)0x0;
  iVar1 = AbyssEngine::SpriteSystemCreate(*(Engine **)(this + 0x34),param_1,param_2,&local_20);
  if (iVar1 == 1) {
    for (uVar2 = 0; uVar2 < *(uint *)(this + 0x180); uVar2 = uVar2 + 1) {
      if (*(int *)(*(int *)(this + 0x184) + uVar2 * 4) == 0) {
        *(SpriteSystem **)(*(int *)(this + 0x184) + uVar2 * 4) = local_20;
        local_20 = (SpriteSystem *)0x0;
        goto LAB_0008a784;
      }
    }
    if (local_20 == (SpriteSystem *)0x0) goto LAB_0008a786;
    ArrayAdd<AbyssEngine::SpriteSystem*>(local_20,(Array *)(this + 0x180));
    uVar2 = *(int *)(this + 0x180) - 1;
  }
  else {
    uVar2 = 0xffffffff;
  }
LAB_0008a784:
  *param_3 = uVar2;
LAB_0008a786:
  if (*piVar3 != local_1c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  0008a71c: push {r4,r5,r6,r7,lr}
  0008a71e: add r7,sp,#0xc
  0008a720: push.w r8
  0008a724: sub sp,#0x8
  0008a726: mov r5,r0
  0008a728: ldr r0,[0x0008a79c]
  0008a72a: mov r4,r3
  0008a72c: mov r3,sp
  0008a72e: add r0,pc
  0008a730: ldr r6,[r0,#0x0]
  0008a732: ldr r0,[r6,#0x0]
  0008a734: str r0,[sp,#0x4]
  0008a736: movs r0,#0x0
  0008a738: str r0,[sp,#0x0]
  0008a73a: ldr r0,[r5,#0x34]
  0008a73c: blx 0x0006fca0
  0008a740: cmp r0,#0x1
  0008a742: bne 0x0008a76e
  0008a744: ldr.w r1,[r5,#0x180]
  0008a748: add.w r8,r5,#0x180
  0008a74c: movs r0,#0x0
  0008a74e: b 0x0008a752
  0008a750: adds r0,#0x1
  0008a752: cmp r0,r1
  0008a754: bcs 0x0008a774
  0008a756: ldr.w r2,[r5,#0x184]
  0008a75a: ldr.w r3,[r2,r0,lsl #0x2]
  0008a75e: cmp r3,#0x0
  0008a760: bne 0x0008a750
  0008a762: ldr r1,[sp,#0x0]
  0008a764: str.w r1,[r2,r0,lsl #0x2]
  0008a768: movs r1,#0x0
  0008a76a: str r1,[sp,#0x0]
  0008a76c: b 0x0008a784
  0008a76e: mov.w r0,#0xffffffff
  0008a772: b 0x0008a784
  0008a774: ldr r0,[sp,#0x0]
  0008a776: cbz r0,0x0008a786
  0008a778: mov r1,r8
  0008a77a: blx 0x0006fcac
  0008a77e: ldr.w r0,[r8,#0x0]
  0008a782: subs r0,#0x1
  0008a784: str r0,[r4,#0x0]
  0008a786: ldr r0,[sp,#0x4]
  0008a788: ldr r1,[r6,#0x0]
  0008a78a: subs r0,r1,r0
  0008a78c: ittt eq
  0008a78e: add.eq sp,#0x8
  0008a790: pop.eq.w r8
  0008a794: pop.eq {r4,r5,r6,r7,pc}
  0008a796: blx 0x0006e824
```
