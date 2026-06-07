# KIPlayer::addGun
elf 0xa61d2 body 8
Sig: undefined __stdcall addGun(Gun * param_1, int param_2)

## decompile
```c

/* KIPlayer::addGun(Gun*, int) */

void KIPlayer::addGun(Gun *param_1,int param_2)

{
  (*(code *)(DAT_001abdf4 + 0x1abdf8))(*(undefined4 *)(param_1 + 4));
  return;
}

```

## target disasm
```
  000b61d2: ldr r0,[r0,#0x4]
  000b61d4: b.w 0x001abde8
  001abde8: bx pc
```
