# PaintCanvas::SpriteSystemCreate
elf 0x7a648 body 170
Sig: undefined __thiscall SpriteSystemCreate(PaintCanvas * this, ushort param_1, bool param_2, ushort param_3, uint * param_4)

## decompile
```c

/* AbyssEngine::PaintCanvas::SpriteSystemCreate(unsigned short, bool, unsigned short, unsigned int&)
    */

void __thiscall
AbyssEngine::PaintCanvas::SpriteSystemCreate
          (PaintCanvas *this,ushort param_1,bool param_2,ushort param_3,uint *param_4)

{
  int iVar1;
  uint uVar2;
  int *piVar3;
  uint local_24;
  SpriteSystem *local_20;
  int local_1c;
  
  piVar3 = *(int **)(DAT_0008a6f4 + 0x8a65e);
  local_1c = *piVar3;
  local_20 = (SpriteSystem *)0x0;
  iVar1 = AbyssEngine::SpriteSystemCreate(*(Engine **)(this + 0x34),param_1,param_2,&local_20);
  if (iVar1 == 1) {
    local_24 = 0xffffffff;
    MaterialCreate(this,param_3,&local_24);
    if (local_24 <= *(uint *)(this + 0x174)) {
      *(undefined4 *)(*(int *)(local_20 + 0x10) + 0x30) =
           *(undefined4 *)(*(int *)(this + 0x178) + local_24 * 4);
    }
    for (uVar2 = 0; uVar2 < *(uint *)(this + 0x180); uVar2 = uVar2 + 1) {
      if (*(int *)(*(int *)(this + 0x184) + uVar2 * 4) == 0) {
        *(SpriteSystem **)(*(int *)(this + 0x184) + uVar2 * 4) = local_20;
        local_20 = (SpriteSystem *)0x0;
        goto LAB_0008a6da;
      }
    }
    if (local_20 == (SpriteSystem *)0x0) goto LAB_0008a6de;
    ArrayAdd<AbyssEngine::SpriteSystem*>(local_20,(Array *)(this + 0x180));
    uVar2 = *(int *)(this + 0x180) - 1;
  }
  else {
    uVar2 = 0xffffffff;
  }
LAB_0008a6da:
  *param_4 = uVar2;
LAB_0008a6de:
  if (*piVar3 != local_1c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  0008a648: push {r4,r5,r6,r7,lr}
  0008a64a: add r7,sp,#0xc
  0008a64c: push.w r8
  0008a650: sub sp,#0x10
  0008a652: mov r4,r0
  0008a654: ldr r0,[0x0008a6f4]
  0008a656: mov r5,r3
  0008a658: add r3,sp,#0x8
  0008a65a: add r0,pc
  0008a65c: ldr r6,[r0,#0x0]
  0008a65e: ldr r0,[r6,#0x0]
  0008a660: str r0,[sp,#0xc]
  0008a662: movs r0,#0x0
  0008a664: str r0,[sp,#0x8]
  0008a666: ldr r0,[r4,#0x34]
  0008a668: blx 0x0006fca0
  0008a66c: ldr.w r8,[r7,#0x8]
  0008a670: cmp r0,#0x1
  0008a672: bne 0x0008a6c6
  0008a674: mov.w r0,#0xffffffff
  0008a678: add r2,sp,#0x4
  0008a67a: str r0,[sp,#0x4]
  0008a67c: mov r0,r4
  0008a67e: mov r1,r5
  0008a680: blx 0x0006fc1c
  0008a684: ldr.w r1,[r4,#0x174]
  0008a688: ldr r0,[sp,#0x4]
  0008a68a: cmp r0,r1
  0008a68c: bhi 0x0008a69c
  0008a68e: ldr r1,[sp,#0x8]
  0008a690: ldr.w r2,[r4,#0x178]
  0008a694: ldr r1,[r1,#0x10]
  0008a696: ldr.w r0,[r2,r0,lsl #0x2]
  0008a69a: str r0,[r1,#0x30]
  0008a69c: ldr.w r1,[r4,#0x180]
  0008a6a0: add.w r5,r4,#0x180
  0008a6a4: movs r0,#0x0
  0008a6a6: b 0x0008a6aa
  0008a6a8: adds r0,#0x1
  0008a6aa: cmp r0,r1
  0008a6ac: bcs 0x0008a6cc
  0008a6ae: ldr.w r2,[r4,#0x184]
  0008a6b2: ldr.w r3,[r2,r0,lsl #0x2]
  0008a6b6: cmp r3,#0x0
  0008a6b8: bne 0x0008a6a8
  0008a6ba: ldr r1,[sp,#0x8]
  0008a6bc: str.w r1,[r2,r0,lsl #0x2]
  0008a6c0: movs r1,#0x0
  0008a6c2: str r1,[sp,#0x8]
  0008a6c4: b 0x0008a6da
  0008a6c6: mov.w r0,#0xffffffff
  0008a6ca: b 0x0008a6da
  0008a6cc: ldr r0,[sp,#0x8]
  0008a6ce: cbz r0,0x0008a6de
  0008a6d0: mov r1,r5
  0008a6d2: blx 0x0006fcac
  0008a6d6: ldr r0,[r5,#0x0]
  0008a6d8: subs r0,#0x1
  0008a6da: str.w r0,[r8,#0x0]
  0008a6de: ldr r0,[sp,#0xc]
  0008a6e0: ldr r1,[r6,#0x0]
  0008a6e2: subs r0,r1,r0
  0008a6e4: ittt eq
  0008a6e6: add.eq sp,#0x10
  0008a6e8: pop.eq.w r8
  0008a6ec: pop.eq {r4,r5,r6,r7,pc}
  0008a6ee: blx 0x0006e824
```
