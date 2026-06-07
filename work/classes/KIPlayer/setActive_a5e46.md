# KIPlayer::setActive
elf 0xa5e46 body 6
Sig: undefined __stdcall setActive(bool param_1)

## decompile
```c

/* KIPlayer::setActive(bool) */

void KIPlayer::setActive(bool param_1)

{
  (*(code *)(DAT_001abad4 + 0x1abad8))(*(undefined4 *)(param_1 + 4));
  return;
}

```

## target disasm
```
  000b5e46: ldr r0,[r0,#0x4]
  000b5e48: b.w 0x001abac8
```
