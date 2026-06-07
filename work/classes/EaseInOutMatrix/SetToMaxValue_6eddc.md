# EaseInOutMatrix::SetToMaxValue
elf 0x6eddc body 8
Sig: undefined __thiscall SetToMaxValue(EaseInOutMatrix * this)

## decompile
```c

/* AbyssEngine::EaseInOutMatrix::SetToMaxValue() */

void __thiscall AbyssEngine::EaseInOutMatrix::SetToMaxValue(EaseInOutMatrix *this)

{
  *(undefined4 *)(this + 0x74) = DAT_0007ede4;
  (*(code *)(DAT_001ab0b4 + 0x1ab0b8))();
  return;
}

```

## target disasm
```
  0007eddc: ldr r1,[0x0007ede4]
  0007edde: str r1,[r0,#0x74]
  0007ede0: b.w 0x001ab0a8
```
