# Achievements::getValue
elf 0x157018 body 22
Sig: undefined __thiscall getValue(Achievements * this, int param_1, int param_2)

## decompile
```c

/* Achievements::getValue(int, int) */

undefined4 __thiscall Achievements::getValue(Achievements *this,int param_1,int param_2)

{
  return *(undefined4 *)(DAT_00167030 + param_1 * 0xc + param_2 * 4 + 0x16701e);
}

```

## target disasm
```
  00167018: ldr r0,[0x00167030]
  0016701a: add.w r1,r1,r1, lsl #0x1
  0016701e: add r0,pc
  00167020: add.w r0,r0,r1, lsl #0x2
  00167024: add.w r0,r0,r2, lsl #0x2
  00167028: ldr.w r0,[r0,#-0x4]
  0016702c: bx lr
```
