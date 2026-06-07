# FModSound::setAudioLanguage
elf 0x92d80 body 32
Sig: undefined __thiscall setAudioLanguage(FModSound * this, int param_1)

## decompile
```c

/* FModSound::setAudioLanguage(int) */

void __thiscall FModSound::setAudioLanguage(FModSound *this,int param_1)

{
  if (*(int *)(this + 0x23fc) == 0) {
    return;
  }
  (*(code *)(DAT_001ab7f4 + 0x1ab7f8))
            (*(int *)(this + 0x23fc),
             *(undefined4 *)(DAT_000a2da0 + 0xa2d94 + (uint)(param_1 == 1) * 4));
  return;
}

```

## target disasm
```
  000a2d80: movw r2,#0x23fc
  000a2d84: ldr r0,[r0,r2]
  000a2d86: cbz r0,0x000a2d9c
  000a2d88: ldr r2,[0x000a2da0]
  000a2d8a: subs r1,#0x1
  000a2d8c: clz r1,r1
  000a2d90: add r2,pc
  000a2d92: lsrs r1,r1,#0x5
  000a2d94: ldr.w r1,[r2,r1,lsl #0x2]
  000a2d98: b.w 0x001ab7e8
  000a2d9c: bx lr
  001ab7e8: bx pc
```
