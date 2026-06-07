# TouchButton::setNumberText
elf 0x1692f8 body 6
Sig: undefined __stdcall setNumberText(String * param_1)

## decompile
```c

/* TouchButton::setNumberText(AbyssEngine::String const&) */

void TouchButton::setNumberText(String *param_1)

{
  (*(code *)(DAT_001ac554 + 0x1ac558))(param_1 + 0x2c);
  return;
}

```

## target disasm
```
  001792f8: adds r0,#0x2c
  001792fa: b.w 0x001ac548
```
