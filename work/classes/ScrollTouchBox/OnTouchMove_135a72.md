# ScrollTouchBox::OnTouchMove
elf 0x135a72 body 36
Sig: undefined __thiscall OnTouchMove(ScrollTouchBox * this, int param_1, int param_2)

## decompile
```c

/* ScrollTouchBox::OnTouchMove(int, int) */

void __thiscall ScrollTouchBox::OnTouchMove(ScrollTouchBox *this,int param_1,int param_2)

{
  int iVar1;
  
  if ((this[0x30] != (ScrollTouchBox)0x0) && (*(int *)(this + 0x10) < *(int *)(this + 0x18))) {
    iVar1 = *(int *)(this + 0x2c);
    *(int *)(this + 0x2c) = param_2;
    *(int *)(this + 0x1c) = param_2 - iVar1;
    *(undefined4 *)(this + 0x20) = 0x3f800000;
    *(int *)(this + 0x34) = (param_2 - iVar1) + *(int *)(this + 0x34);
  }
  return;
}

```

## target disasm
```
  00145a72: ldrb.w r1,[r0,#0x30]
  00145a76: cbz r1,0x00145a94
  00145a78: ldr r1,[r0,#0x10]
  00145a7a: ldr r3,[r0,#0x18]
  00145a7c: cmp r3,r1
  00145a7e: ble 0x00145a94
  00145a80: ldr r1,[r0,#0x2c]
  00145a82: mov.w r12,#0x3f800000
  00145a86: ldr r3,[r0,#0x34]
  00145a88: subs r1,r2,r1
  00145a8a: str r2,[r0,#0x2c]
  00145a8c: strd r1,r12,[r0,#0x1c]
  00145a90: add r1,r3
  00145a92: str r1,[r0,#0x34]
  00145a94: bx lr
```
