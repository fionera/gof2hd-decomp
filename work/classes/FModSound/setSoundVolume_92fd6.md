# FModSound::setSoundVolume
elf 0x92fd6 body 26
Sig: undefined __thiscall setSoundVolume(FModSound * this, int param_1, float param_2)

## decompile
```c

/* FModSound::setSoundVolume(int, float) */

void __thiscall FModSound::setSoundVolume(FModSound *this,int param_1,float param_2)

{
  if ((*(int *)(this + 0x23fc) != 0) && (*(int *)(this + param_1 * 4 + 0x18) != 0)) {
    (*(code *)(DAT_001ab804 + 0x1ab808))();
    return;
  }
  return;
}

```

## target disasm
```
  000a2fd6: movw r3,#0x23fc
  000a2fda: ldr r3,[r0,r3]
  000a2fdc: cbz r3,0x000a2fec
  000a2fde: add.w r0,r0,r1, lsl #0x2
  000a2fe2: ldr r0,[r0,#0x18]
  000a2fe4: cbz r0,0x000a2fec
  000a2fe6: mov r1,r2
  000a2fe8: b.w 0x001ab7f8
  000a2fec: bx lr
  001ab7f8: bx pc
```
