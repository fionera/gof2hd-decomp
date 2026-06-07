# ScrollTouchBox::OnTouchBegin
elf 0x135a54 body 30
Sig: undefined __thiscall OnTouchBegin(ScrollTouchBox * this, int param_1, int param_2)

## decompile
```c

/* ScrollTouchBox::OnTouchBegin(int, int) */

void __thiscall ScrollTouchBox::OnTouchBegin(ScrollTouchBox *this,int param_1,int param_2)

{
  int iVar1;
  
  iVar1 = touchIsInside(this,param_1,param_2);
  if (iVar1 != 0) {
    *(int *)(this + 0x28) = param_2;
    *(int *)(this + 0x2c) = param_2;
    *(undefined4 *)(this + 0x1c) = 0;
    this[0x30] = (ScrollTouchBox)0x1;
  }
  return;
}

```

## target disasm
```
  00145a54: push {r4,r5,r7,lr}
  00145a56: add r7,sp,#0x8
  00145a58: mov r5,r2
  00145a5a: mov r4,r0
  00145a5c: blx 0x00076a5c
  00145a60: cbz r0,0x00145a70
  00145a62: movs r0,#0x0
  00145a64: strd r5,r5,[r4,#0x28]
  00145a68: str r0,[r4,#0x1c]
  00145a6a: movs r0,#0x1
  00145a6c: strb.w r0,[r4,#0x30]
  00145a70: pop {r4,r5,r7,pc}
```
