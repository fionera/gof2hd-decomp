# ScrollTouchBox::ScrollTouchBox
elf 0x135598 body 60
Sig: undefined __thiscall ScrollTouchBox(ScrollTouchBox * this, int param_1, int param_2, int param_3, int param_4)

## decompile
```c

/* ScrollTouchBox::ScrollTouchBox(int, int, int, int) */

void __thiscall
ScrollTouchBox::ScrollTouchBox(ScrollTouchBox *this,int param_1,int param_2,int param_3,int param_4)

{
  undefined4 *puVar1;
  
  puVar1 = (undefined4 *)(DAT_001455d4 + 0x1455ac);
  *(undefined4 *)(this + 0x28) = 0;
  *(undefined4 *)this = 0;
  *(int *)(this + 4) = param_1;
  *(int *)(this + 8) = param_2;
  *(int *)(this + 0xc) = param_3;
  *(int *)(this + 0x10) = param_4;
  puVar1 = (undefined4 *)*puVar1;
  *(int *)(this + 0x14) = param_3;
  this[0x30] = (ScrollTouchBox)0x0;
  *(undefined4 *)(this + 0x34) = 0;
  this[0x38] = (ScrollTouchBox)0x0;
  *(undefined4 *)(this + 0x18) = 0;
  *(undefined4 *)(this + 0x1c) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  *(undefined4 *)(this + 0x20) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  *(undefined4 *)(this + 0x24) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  *(undefined4 *)(this + 0x3c) = *puVar1;
  return;
}

```

## target disasm
```
  00145598: push {r4,r5,r7,lr}
  0014559a: add r7,sp,#0x8
  0014559c: ldr r5,[0x001455d4]
  0014559e: movs r4,#0x0
  001455a0: ldr.w r12,[r7,#0x8]
  001455a4: vmov.i32 q8,#0x0
  001455a8: add r5,pc
  001455aa: str r4,[r0,#0x28]
  001455ac: strd r4,r1,[r0,#0x0]
  001455b0: add.w r1,r0,#0x8
  001455b4: stm r1,{r2,r3,r12}
  001455b8: add.w r2,r0,#0x18
  001455bc: ldr r1,[r5,#0x0]
  001455be: str r3,[r0,#0x14]
  001455c0: strb.w r4,[r0,#0x30]
  001455c4: str r4,[r0,#0x34]
  001455c6: strb.w r4,[r0,#0x38]
  001455ca: vst1.32 {d16,d17},[r2]
  001455ce: ldr r1,[r1,#0x0]
  001455d0: str r1,[r0,#0x3c]
  001455d2: pop {r4,r5,r7,pc}
```
