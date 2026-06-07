# ScrollTouchWindow::setText
elf 0x1742c4 body 80
Sig: undefined __thiscall setText(ScrollTouchWindow * this, String param_1, String param_2, int param_3)

## decompile
```c

/* ScrollTouchWindow::setText(AbyssEngine::String, AbyssEngine::String, int) */

void __thiscall
ScrollTouchWindow::setText
          (ScrollTouchWindow *this,String *param_2,String *param_3,undefined4 param_4)

{
  int iVar1;
  int iVar2;
  ScrollTouchBox *pSVar3;
  int *piVar4;
  
  piVar4 = *(int **)(DAT_00184324 + 0x1842d8);
  iVar1 = *piVar4;
  pSVar3 = *(ScrollTouchBox **)this;
  AbyssEngine::String::String((String *)&stack0xffffffd0,param_3,false);
  ScrollTouchBox::setText(pSVar3,(String *)&stack0xffffffd0,param_4);
  AbyssEngine::String::~String((String *)&stack0xffffffd0);
  AbyssEngine::String::operator=((String *)(this + 4),param_2);
  iVar2 = *piVar4;
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
  001842c4: push {r4,r5,r6,r7,lr}
  001842c6: add r7,sp,#0xc
  001842c8: push {r4,r5,r6,r7,r8,r9,r10}
  001842cc: mov r5,r0
  001842ce: ldr r0,[0x00184324]
  001842d0: mov r9,sp
  001842d2: mov r8,r1
  001842d4: add r0,pc
  001842d6: mov r1,r2
  001842d8: movs r2,#0x0
  001842da: mov r10,r3
  001842dc: ldr r6,[r0,#0x0]
  001842de: ldr r0,[r6,#0x0]
  001842e0: str r0,[sp,#0xc]
  001842e2: mov r0,r9
  001842e4: ldr r4,[r5,#0x0]
  001842e6: blx 0x0006f028
  001842ea: mov r0,r4
  001842ec: mov r1,r9
  001842ee: mov r2,r10
  001842f0: blx 0x00076a50
  001842f4: mov r0,sp
  001842f6: blx 0x0006ee30
  001842fa: adds r0,r5,#0x4
  001842fc: mov r1,r8
  001842fe: blx 0x0006f2b0
  00184302: ldr r0,[sp,#0xc]
  00184304: ldr r1,[r6,#0x0]
  00184306: subs r0,r1,r0
  00184308: itt eq
  0018430a: pop.eq.w {r0,r1,r2,r3,r8,r9,r10}
  0018430e: pop.eq {r4,r5,r6,r7,pc}
  00184310: blx 0x0006e824
```
