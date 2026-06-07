# FModSound::~FModSound
elf 0x92bd4 body 14
Sig: undefined __thiscall ~FModSound(FModSound * this)

## decompile
```c

/* FModSound::~FModSound() */

FModSound * __thiscall FModSound::~FModSound(FModSound *this)

{
  release(this);
  return this;
}

```

## target disasm
```
  000a2bd4: push {r4,r6,r7,lr}
  000a2bd6: add r7,sp,#0x8
  000a2bd8: mov r4,r0
  000a2bda: blx 0x0007144c
  000a2bde: mov r0,r4
  000a2be0: pop {r4,r6,r7,pc}
```
