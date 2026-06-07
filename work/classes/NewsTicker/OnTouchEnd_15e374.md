# NewsTicker::OnTouchEnd
elf 0x15e374 body 22
Sig: undefined __stdcall OnTouchEnd(int param_1, int param_2)

## decompile
```c

/* NewsTicker::OnTouchEnd(int, int) */

undefined4 NewsTicker::OnTouchEnd(int param_1,int param_2)

{
  if (*(char *)(param_1 + 0x28) == '\0') {
    return 0;
  }
  *(undefined1 *)(param_1 + 0x28) = 0;
  return 1;
}

```

## target disasm
```
  0016e374: ldrb.w r2,[r0,#0x28]
  0016e378: mov r1,r0
  0016e37a: movs r0,#0x0
  0016e37c: cmp r2,#0x0
  0016e37e: it eq
  0016e380: bx.eq lr
  0016e382: strb.w r0,[r1,#0x28]
  0016e386: movs r0,#0x1
  0016e388: bx lr
```
