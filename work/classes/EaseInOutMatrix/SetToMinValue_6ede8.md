# EaseInOutMatrix::SetToMinValue
elf 0x6ede8 body 12
Sig: undefined __thiscall SetToMinValue(EaseInOutMatrix * this)

## decompile
```c

/* AbyssEngine::EaseInOutMatrix::SetToMinValue() */

void __thiscall AbyssEngine::EaseInOutMatrix::SetToMinValue(EaseInOutMatrix *this)

{
  *(undefined4 *)(this + 0x74) = 0x3f400000;
  (*(code *)(DAT_001ab0b4 + 0x1ab0b8))();
  return;
}

```

## target disasm
```
  0007ede8: mov.w r1,#0x3f400000
  0007edec: str r1,[r0,#0x74]
  0007edee: b.w 0x001ab0a8
  001ab0a8: bx pc
```
