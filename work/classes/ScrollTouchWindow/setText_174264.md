# ScrollTouchWindow::setText
elf 0x174264 body 76
Sig: undefined __thiscall setText(ScrollTouchWindow * this, String param_1, String param_2)

## decompile
```c

/* ScrollTouchWindow::setText(AbyssEngine::String, AbyssEngine::String) */

void __thiscall ScrollTouchWindow::setText(ScrollTouchWindow *this,String *param_2,String *param_3)

{
  int iVar1;
  int iVar2;
  int *piVar3;
  ScrollTouchBox *pSVar4;
  
  piVar3 = *(int **)(DAT_001842c0 + 0x184278);
  iVar1 = *piVar3;
  pSVar4 = *(ScrollTouchBox **)this;
  AbyssEngine::String::String((String *)&stack0xffffffd0,param_3,false);
  ScrollTouchBox::setText(pSVar4,(String *)&stack0xffffffd0);
  AbyssEngine::String::~String((String *)&stack0xffffffd0);
  AbyssEngine::String::operator=((String *)(this + 4),param_2);
  iVar2 = *piVar3;
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
  00184264: push {r4,r5,r6,r7,lr}
  00184266: add r7,sp,#0xc
  00184268: push {r4,r5,r6,r7,r8,r9,r11}
  0018426c: mov r5,r0
  0018426e: ldr r0,[0x001842c0]
  00184270: mov r9,sp
  00184272: mov r8,r1
  00184274: add r0,pc
  00184276: mov r1,r2
  00184278: movs r2,#0x0
  0018427a: ldr r4,[r0,#0x0]
  0018427c: ldr r0,[r4,#0x0]
  0018427e: str r0,[sp,#0xc]
  00184280: mov r0,r9
  00184282: ldr r6,[r5,#0x0]
  00184284: blx 0x0006f028
  00184288: mov r0,r6
  0018428a: mov r1,r9
  0018428c: blx 0x00075178
  00184290: mov r0,sp
  00184292: blx 0x0006ee30
  00184296: adds r0,r5,#0x4
  00184298: mov r1,r8
  0018429a: blx 0x0006f2b0
  0018429e: ldr r0,[sp,#0xc]
  001842a0: ldr r1,[r4,#0x0]
  001842a2: subs r0,r1,r0
  001842a4: itt eq
  001842a6: pop.eq.w {r0,r1,r2,r3,r8,r9,r11}
  001842aa: pop.eq {r4,r5,r6,r7,pc}
  001842ac: blx 0x0006e824
```
