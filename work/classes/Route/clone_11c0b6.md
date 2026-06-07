# Route::clone
elf 0x11c0b6 body 298
Sig: int __stdcall clone(__fn * __fn, void * __child_stack, int __flags, void * __arg, ...)

## decompile
```c

/* Route::clone() */

int Route::clone(__fn *__fn,void *__child_stack,int __flags,void *__arg,...)

{
  longlong lVar1;
  uint uVar2;
  int *piVar3;
  int *piVar4;
  Route *pRVar5;
  Array<KIPlayer*> *this;
  int iVar6;
  int iVar7;
  bool bVar8;
  int *piVar9;
  uint *puVar10;
  int iVar11;
  
  piVar9 = *(int **)(__fn + 0xc);
  iVar11 = *piVar9;
  lVar1 = (ulonglong)(uint)(iVar11 * 3) * 4;
  uVar2 = (uint)lVar1;
  if ((int)((ulonglong)lVar1 >> 0x20) != 0) {
    uVar2 = 0xffffffff;
  }
  piVar3 = operator_new__(uVar2);
  piVar4 = piVar3 + 1;
  for (iVar6 = 0; iVar11 != iVar6; iVar6 = iVar6 + 1) {
    iVar7 = *(int *)(piVar9[1] + iVar6 * 4);
    piVar4[-1] = *(int *)(iVar7 + 0x124);
    *piVar4 = *(int *)(iVar7 + 0x128);
    piVar4[1] = *(int *)(iVar7 + 300);
    piVar4 = piVar4 + 3;
  }
  piVar4 = *(int **)(__fn + 0x10);
  if (piVar4 != (int *)0x0) {
    iVar11 = 0;
    bVar8 = false;
    while (*piVar4 != iVar11) {
      iVar6 = iVar11 * 4;
      iVar11 = iVar11 + 1;
      if (*(int *)(piVar4[1] + iVar6) != 0) {
        bVar8 = true;
      }
    }
    if (bVar8) {
      puVar10 = *(uint **)(__fn + 0x14);
      uVar2 = (uint)((ulonglong)*puVar10 * 4);
      if ((int)((ulonglong)*puVar10 * 4 >> 0x20) != 0) {
        uVar2 = 0xffffffff;
      }
      piVar9 = operator_new__(uVar2);
      for (uVar2 = 0; uVar2 < *puVar10; uVar2 = uVar2 + 1) {
        piVar9[uVar2] = *(int *)(puVar10[1] + uVar2 * 4);
      }
      this = operator_new(0xc);
      Array<KIPlayer*>::Array(this);
      ArraySetLength<KIPlayer*>(**(uint **)(__fn + 0x10),(Array *)this);
      for (uVar2 = 0; uVar2 < **(uint **)(__fn + 0x10); uVar2 = uVar2 + 1) {
        *(undefined4 *)(*(int *)(this + 4) + uVar2 * 4) =
             *(undefined4 *)((*(uint **)(__fn + 0x10))[1] + uVar2 * 4);
      }
      pRVar5 = operator_new(0x18);
      Route(pRVar5,piVar3,(Array *)this,piVar9,**(int **)(__fn + 0xc) * 3);
      pRVar5[4] = *(Route *)(__fn + 4);
      operator_delete__(piVar9);
      return (int)pRVar5;
    }
  }
  pRVar5 = operator_new(0x18);
  Route(pRVar5,piVar3,*piVar9 * 3);
  pRVar5[4] = *(Route *)(__fn + 4);
  return (int)pRVar5;
}

```

## target disasm
```
  0012c0b6: push {r4,r5,r6,r7,lr}
  0012c0b8: add r7,sp,#0xc
  0012c0ba: push {r6,r7,r8,r9,r11}
  0012c0be: ldr r5,[r0,#0xc]
  0012c0c0: mov r4,r0
  0012c0c2: movs r1,#0x4
  0012c0c4: ldr r6,[r5,#0x0]
  0012c0c6: add.w r0,r6,r6, lsl #0x1
  0012c0ca: umull r0,r1,r0,r1
  0012c0ce: cmp r1,#0x0
  0012c0d0: it ne
  0012c0d2: mov.ne r1,#0x1
  0012c0d4: cmp r1,#0x0
  0012c0d6: it ne
  0012c0d8: mov.ne.w r0,#0xffffffff
  0012c0dc: blx 0x0006ec08
  0012c0e0: mov r8,r0
  0012c0e2: adds r0,#0x4
  0012c0e4: movs r1,#0x0
  0012c0e6: b 0x0012c106
  0012c0e8: ldr r2,[r5,#0x4]
  0012c0ea: ldr.w r2,[r2,r1,lsl #0x2]
  0012c0ee: adds r1,#0x1
  0012c0f0: ldr.w r3,[r2,#0x124]
  0012c0f4: str.w r3,[r0,#-0x4]
  0012c0f8: ldr.w r3,[r2,#0x128]
  0012c0fc: str r3,[r0,#0x0]
  0012c0fe: ldr.w r2,[r2,#0x12c]
  0012c102: str r2,[r0,#0x4]
  0012c104: adds r0,#0xc
  0012c106: cmp r6,r1
  0012c108: bne 0x0012c0e8
  0012c10a: ldr r0,[r4,#0x10]
  0012c10c: cbz r0,0x0012c12c
  0012c10e: ldr r1,[r0,#0x0]
  0012c110: movs r2,#0x0
  0012c112: movs r3,#0x0
  0012c114: b 0x0012c124
  0012c116: ldr r6,[r0,#0x4]
  0012c118: ldr.w r6,[r6,r2,lsl #0x2]
  0012c11c: adds r2,#0x1
  0012c11e: cmp r6,#0x0
  0012c120: it ne
  0012c122: mov.ne r3,#0x1
  0012c124: cmp r1,r2
  0012c126: bne 0x0012c116
  0012c128: lsls r0,r3,#0x1f
  0012c12a: bne 0x0012c14a
  0012c12c: movs r0,#0x18
  0012c12e: blx 0x0006eb24
  0012c132: mov r9,r0
  0012c134: ldr r0,[r5,#0x0]
  0012c136: add.w r2,r0,r0, lsl #0x1
  0012c13a: mov r0,r9
  0012c13c: mov r1,r8
  0012c13e: blx 0x00075fc4
  0012c142: ldrb r0,[r4,#0x4]
  0012c144: strb.w r0,[r9,#0x4]
  0012c148: b 0x0012c1d8
  0012c14a: ldr r5,[r4,#0x14]
  0012c14c: movs r1,#0x4
  0012c14e: ldr r0,[r5,#0x0]
  0012c150: umull r0,r1,r0,r1
  0012c154: cmp r1,#0x0
  0012c156: it ne
  0012c158: mov.ne r1,#0x1
  0012c15a: cmp r1,#0x0
  0012c15c: it ne
  0012c15e: mov.ne.w r0,#0xffffffff
  0012c162: blx 0x0006ec08
  0012c166: mov r6,r0
  0012c168: movs r0,#0x0
  0012c16a: b 0x0012c178
  0012c16c: ldr r1,[r5,#0x4]
  0012c16e: ldr.w r1,[r1,r0,lsl #0x2]
  0012c172: str.w r1,[r6,r0,lsl #0x2]
  0012c176: adds r0,#0x1
  0012c178: ldr r1,[r5,#0x0]
  0012c17a: cmp r0,r1
  0012c17c: bcc 0x0012c16c
  0012c17e: movs r0,#0xc
  0012c180: blx 0x0006eb24
  0012c184: mov r5,r0
  0012c186: blx 0x00073d08
  0012c18a: ldr r0,[r4,#0x10]
  0012c18c: mov r1,r5
  0012c18e: ldr r0,[r0,#0x0]
  0012c190: blx 0x00073d14
  0012c194: movs r0,#0x0
  0012c196: b 0x0012c1a6
  0012c198: ldr r1,[r1,#0x4]
  0012c19a: ldr r2,[r5,#0x4]
  0012c19c: ldr.w r1,[r1,r0,lsl #0x2]
  0012c1a0: str.w r1,[r2,r0,lsl #0x2]
  0012c1a4: adds r0,#0x1
  0012c1a6: ldr r1,[r4,#0x10]
  0012c1a8: ldr r2,[r1,#0x0]
  0012c1aa: cmp r0,r2
  0012c1ac: bcc 0x0012c198
  0012c1ae: movs r0,#0x18
  0012c1b0: blx 0x0006eb24
  0012c1b4: mov r9,r0
  0012c1b6: ldr r0,[r4,#0xc]
  0012c1b8: ldr r0,[r0,#0x0]
  0012c1ba: add.w r0,r0,r0, lsl #0x1
  0012c1be: str r0,[sp,#0x0]
  0012c1c0: mov r0,r9
  0012c1c2: mov r1,r8
  0012c1c4: mov r2,r5
  0012c1c6: mov r3,r6
  0012c1c8: blx 0x00075fd0
  0012c1cc: ldrb r0,[r4,#0x4]
  0012c1ce: strb.w r0,[r9,#0x4]
  0012c1d2: mov r0,r6
  0012c1d4: blx 0x0006ebfc
  0012c1d8: mov r0,r9
  0012c1da: pop.w {r2,r3,r8,r9,r11}
  0012c1de: pop {r4,r5,r6,r7,pc}
```
