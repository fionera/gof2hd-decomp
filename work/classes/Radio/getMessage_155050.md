# Radio::getMessage
elf 0x155050 body 10
Sig: undefined __thiscall getMessage(Radio * this, int param_1)

## decompile
```c

/* Radio::getMessage(int) */

undefined4 __thiscall Radio::getMessage(Radio *this,int param_1)

{
  return *(undefined4 *)(*(int *)(*(int *)this + 4) + param_1 * 4);
}

```

## target disasm
```
  00165050: ldr r0,[r0,#0x0]
  00165052: ldr r0,[r0,#0x4]
  00165054: ldr.w r0,[r0,r1,lsl #0x2]
  00165058: bx lr
```
