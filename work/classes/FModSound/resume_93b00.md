# FModSound::resume
elf 0x93b00 body 26
Sig: undefined __thiscall resume(FModSound * this, int param_1)

## decompile
```c

/* FModSound::resume(int) */

void __thiscall FModSound::resume(FModSound *this,int param_1)

{
  if ((*(int *)(this + 0x23fc) != 0) && (*(int *)(this + param_1 * 4 + 0x18) != 0)) {
    (*(code *)(DAT_001ab834 + 0x1ab838))(*(int *)(this + param_1 * 4 + 0x18),0);
    return;
  }
  return;
}

```

## target disasm
```
  000a3b00: movw r2,#0x23fc
  000a3b04: ldr r2,[r0,r2]
  000a3b06: cbz r2,0x000a3b16
  000a3b08: add.w r0,r0,r1, lsl #0x2
  000a3b0c: ldr r0,[r0,#0x18]
  000a3b0e: cbz r0,0x000a3b16
  000a3b10: movs r1,#0x0
  000a3b12: b.w 0x001ab828
  000a3b16: bx lr
  001ab828: bx pc
```
