# String::Reverse
elf 0x722e8 body 134
Sig: undefined __thiscall Reverse(String * this)

## decompile
```c

/* AbyssEngine::String::Reverse() */

void __thiscall AbyssEngine::String::Reverse(String *this)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  undefined2 *puVar4;
  undefined2 *puVar5;
  undefined2 *puVar6;
  int unaff_r5;
  int unaff_r6;
  int *piVar7;
  
  piVar7 = *(int **)(DAT_00082370 + 0x822f8);
  iVar1 = *piVar7;
  if ((*(int *)(this + 4) != 0) && (iVar2 = GameText::getLanguage(), iVar2 == 9)) {
    iVar2 = 0;
    String((String *)&stack0xffffffd0,*(ushort **)(this + 4),false);
    uVar3 = *(int *)(this + 8) - 1;
    puVar4 = (undefined2 *)(unaff_r5 + uVar3 * 2);
    puVar6 = *(undefined2 **)(DAT_00082374 + 0x8232c);
    for (; uVar3 != 0xffffffff; uVar3 = uVar3 - 1) {
      puVar5 = puVar6;
      if ((int)uVar3 < unaff_r6) {
        puVar5 = puVar4;
      }
      if (0x7fffffff < uVar3) {
        puVar5 = puVar6;
      }
      puVar4 = puVar4 + -1;
      *(undefined2 *)(*(int *)(this + 4) + iVar2) = *puVar5;
      iVar2 = iVar2 + 2;
    }
    ~String((String *)&stack0xffffffd0);
  }
  iVar2 = *piVar7;
  iVar1 = iVar2 - iVar1;
  if (iVar1 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar1,iVar2);
  }
  return;
}

```

## target disasm
```
  000822e8: push {r4,r5,r6,r7,lr}
  000822ea: add r7,sp,#0xc
  000822ec: push {r4,r5,r6,r7,r8,r9,r11}
  000822f0: mov r9,r0
  000822f2: ldr r0,[0x00082370]
  000822f4: add r0,pc
  000822f6: ldr.w r8,[r0,#0x0]
  000822fa: ldr.w r0,[r8,#0x0]
  000822fe: str r0,[sp,#0xc]
  00082300: ldr.w r0,[r9,#0x4]
  00082304: cbz r0,0x0008235a
  00082306: blx 0x0006f544
  0008230a: cmp r0,#0x9
  0008230c: bne 0x0008235a
  0008230e: ldr.w r1,[r9,#0x4]
  00082312: mov r0,sp
  00082314: movs r2,#0x0
  00082316: movs r6,#0x0
  00082318: blx 0x0006f574
  0008231c: ldr.w r1,[r9,#0x8]
  00082320: ldrd r2,r0,[sp,#0x4]
  00082324: ldr r3,[0x00082374]
  00082326: subs r1,#0x1
  00082328: add r3,pc
  0008232a: add.w r2,r2,r1, lsl #0x1
  0008232e: ldr r4,[r3,#0x0]
  00082330: b 0x00082350
  00082332: mov r3,r4
  00082334: ldr.w r5,[r9,#0x4]
  00082338: cmp r0,r1
  0008233a: it gt
  0008233c: mov.gt r3,r2
  0008233e: cmp.w r1,#0xffffffff
  00082342: it le
  00082344: mov.le r3,r4
  00082346: ldrh r3,[r3,#0x0]
  00082348: subs r2,#0x2
  0008234a: strh r3,[r5,r6]
  0008234c: adds r6,#0x2
  0008234e: subs r1,#0x1
  00082350: adds r5,r1,#0x1
  00082352: bne 0x00082332
  00082354: mov r0,sp
  00082356: blx 0x0006f580
  0008235a: ldr r0,[sp,#0xc]
  0008235c: ldr.w r1,[r8,#0x0]
  00082360: subs r0,r1,r0
  00082362: itt eq
  00082364: pop.eq.w {r0,r1,r2,r3,r8,r9,r11}
  00082368: pop.eq {r4,r5,r6,r7,pc}
  0008236a: blx 0x0006e824
```
