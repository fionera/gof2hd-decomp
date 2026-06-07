# PlayerEgo::setActive
elf 0x9b7f4 body 6
Sig: undefined __stdcall setActive(bool param_1)

## decompile
```c

/* PlayerEgo::setActive(bool) */

void PlayerEgo::setActive(bool param_1)

{
  (*(code *)(DAT_001abad4 + 0x1abad8))(*(undefined4 *)(uint)param_1);
  return;
}

```

## target disasm
```
  000ab7f4: ldr r0,[r0,#0x0]
  000ab7f6: b.w 0x001abac8
```
