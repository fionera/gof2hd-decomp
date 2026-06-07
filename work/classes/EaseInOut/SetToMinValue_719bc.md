# EaseInOut::SetToMinValue
elf 0x719bc body 10
Sig: undefined __thiscall SetToMinValue(EaseInOut * this)

## decompile
```c

/* AbyssEngine::EaseInOut::SetToMinValue() */

void __thiscall AbyssEngine::EaseInOut::SetToMinValue(EaseInOut *this)

{
  *(undefined4 *)(this + 8) = DAT_000819c4;
  (*(code *)(DAT_001ab0f4 + 0x1ab0f8))();
  return;
}

```

## target disasm
```
  000819bc: ldr r1,[0x000819c4]
  000819be: str r1,[r0,#0x8]
  000819c0: b.w 0x001ab0e8
  001ab0e8: bx pc
```
