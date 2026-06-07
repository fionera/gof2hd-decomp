# TouchButton::setSplitText
elf 0x16934c body 6
Sig: undefined __stdcall setSplitText(String * param_1)

## decompile
```c

/* TouchButton::setSplitText(AbyssEngine::String const&) */

void TouchButton::setSplitText(String *param_1)

{
  (*(code *)(DAT_001ac554 + 0x1ac558))(param_1 + 0x18);
  return;
}

```

## target disasm
```
  0017934c: adds r0,#0x18
  0017934e: b.w 0x001ac548
```
