# PlayerFixedObject::setName
elf 0x154f12 body 8
Sig: undefined __stdcall setName(String param_1)

## decompile
```c

/* PlayerFixedObject::setName(AbyssEngine::String) */

void PlayerFixedObject::setName(int param_1)

{
  (*(code *)(DAT_001ac554 + 0x1ac558))(param_1 + 0x1ac);
  return;
}

```

## target disasm
```
  00164f12: add.w r0,r0,#0x1ac
  00164f16: b.w 0x001ac548
```
