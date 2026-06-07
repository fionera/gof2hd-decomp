# Mission::setTargetSystemName
elf 0x15bbac body 6
Sig: undefined __stdcall setTargetSystemName(String * param_1)

## decompile
```c

/* Mission::setTargetSystemName(AbyssEngine::String const&) */

void Mission::setTargetSystemName(String *param_1)

{
  (*(code *)(DAT_001ac554 + 0x1ac558))(param_1 + 0x4c);
  return;
}

```

## target disasm
```
  0016bbac: adds r0,#0x4c
  0016bbae: b.w 0x001ac548
```
