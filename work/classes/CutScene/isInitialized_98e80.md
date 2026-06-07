# CutScene::isInitialized
elf 0x98e80 body 6
Sig: undefined __thiscall isInitialized(CutScene * this)

## decompile
```c

/* CutScene::isInitialized() */

CutScene __thiscall CutScene::isInitialized(CutScene *this)

{
  return this[0x5c];
}

```

## target disasm
```
  000a8e80: ldrb.w r0,[r0,#0x5c]
  000a8e84: bx lr
```
