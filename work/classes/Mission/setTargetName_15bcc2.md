# Mission::setTargetName
elf 0x15bcc2 body 6
Sig: undefined __stdcall setTargetName(String param_1)

## decompile
```c

/* Mission::setTargetName(AbyssEngine::String) */

void Mission::setTargetName(int param_1)

{
  (*(code *)(DAT_001ac554 + 0x1ac558))(param_1 + 0x1c);
  return;
}

```

## target disasm
```
  0016bcc2: adds r0,#0x1c
  0016bcc4: b.w 0x001ac548
```
