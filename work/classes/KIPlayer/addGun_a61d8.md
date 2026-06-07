# KIPlayer::addGun
elf 0xa61d8 body 8
Sig: undefined __stdcall addGun(Array * param_1, int param_2)

## decompile
```c

/* KIPlayer::addGun(Array<Gun*>*, int) */

void KIPlayer::addGun(Array *param_1,int param_2)

{
  (*(code *)(DAT_001abe04 + 0x1abe08))(*(undefined4 *)(param_1 + 4));
  return;
}

```

## target disasm
```
  000b61d8: ldr r0,[r0,#0x4]
  000b61da: b.w 0x001abdf8
  001abdf8: bx pc
```
