# String::Trim
elf 0x731a8 body 114
Sig: undefined __thiscall Trim(String * this)

## decompile
```c

/* AbyssEngine::String::Trim() */

void __thiscall AbyssEngine::String::Trim(String *this)

{
  short sVar1;
  uint uVar2;
  int iVar3;
  uint uVar4;
  int *piVar5;
  String aSStack_28 [4];
  ushort *local_24;
  int local_1c;
  
  piVar5 = *(int **)(DAT_00083228 + 0x831ba);
  local_1c = *piVar5;
  iVar3 = *(int *)(this + 4);
  if ((iVar3 != 0) && (uVar2 = *(uint *)(this + 8), uVar2 != 0)) {
    uVar4 = 0;
    while ((uVar4 < uVar2 && (sVar1 = *(short *)(iVar3 + uVar4 * 2), sVar1 == 0x20 || sVar1 == 9)))
    {
      uVar4 = uVar4 + 1;
    }
    do {
      do {
        sVar1 = *(short *)(iVar3 + -2 + uVar2 * 2);
        uVar2 = uVar2 - 1;
      } while (sVar1 == 9);
    } while (sVar1 == 0x20);
    SubString((uint)aSStack_28,(uint)this);
    Set(this,local_24);
    ~String(aSStack_28);
  }
  iVar3 = *piVar5 - local_1c;
  if (iVar3 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar3);
  }
  return;
}

```

## target disasm
```
  000831a8: push {r4,r5,r6,r7,lr}
  000831aa: add r7,sp,#0xc
  000831ac: push.w r11
  000831b0: sub sp,#0x10
  000831b2: mov r4,r0
  000831b4: ldr r0,[0x00083228]
  000831b6: add r0,pc
  000831b8: ldr r6,[r0,#0x0]
  000831ba: ldr r0,[r6,#0x0]
  000831bc: str r0,[sp,#0xc]
  000831be: ldr r1,[r4,#0x4]
  000831c0: cbz r1,0x00083204
  000831c2: ldr r0,[r4,#0x8]
  000831c4: cbz r0,0x00083204
  000831c6: movs r2,#0x0
  000831c8: b 0x000831d8
  000831ca: ldrh.w r3,[r1,r2,lsl #0x1]
  000831ce: cmp r3,#0x20
  000831d0: it ne
  000831d2: cmp.ne r3,#0x9
  000831d4: bne 0x000831dc
  000831d6: adds r2,#0x1
  000831d8: cmp r2,r0
  000831da: bcc 0x000831ca
  000831dc: subs r1,#0x2
  000831de: ldrh.w r5,[r1,r0,lsl #0x1]
  000831e2: mov r3,r0
  000831e4: subs r0,#0x1
  000831e6: cmp r5,#0x9
  000831e8: beq 0x000831de
  000831ea: cmp r5,#0x20
  000831ec: beq 0x000831de
  000831ee: mov r0,sp
  000831f0: mov r1,r4
  000831f2: blx 0x0006f604
  000831f6: ldr r1,[sp,#0x4]
  000831f8: mov r0,r4
  000831fa: blx 0x0006f3f4
  000831fe: mov r0,sp
  00083200: blx 0x0006f580
  00083204: ldr r0,[sp,#0xc]
  00083206: ldr r1,[r6,#0x0]
  00083208: subs r0,r1,r0
  0008320a: itttt eq
  0008320c: mov.eq r0,r4
  0008320e: add.eq sp,#0x10
  00083210: pop.eq.w r11
  00083214: pop.eq {r4,r5,r6,r7,pc}
  00083216: blx 0x0006e824
```
