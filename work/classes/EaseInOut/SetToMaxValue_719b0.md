# EaseInOut::SetToMaxValue
elf 0x719b0 body 8
Sig: undefined __thiscall SetToMaxValue(EaseInOut * this)

## decompile
```c

/* AbyssEngine::EaseInOut::SetToMaxValue() */

void __thiscall AbyssEngine::EaseInOut::SetToMaxValue(EaseInOut *this)

{
  *(undefined4 *)(this + 8) = DAT_000819b8;
  (*(code *)(DAT_001ab0f4 + 0x1ab0f8))();
  return;
}

```

## target disasm
```
  000819b0: ldr r1,[0x000819b8]
  000819b2: str r1,[r0,#0x8]
  000819b4: b.w 0x001ab0e8
```
