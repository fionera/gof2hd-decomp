# KIPlayer::setVisible
elf 0xa5b2c body 18
Sig: undefined __thiscall setVisible(KIPlayer * this, bool param_1)

## decompile
```c

/* KIPlayer::setVisible(bool) */

void __thiscall KIPlayer::setVisible(KIPlayer *this,bool param_1)

{
  this[0xf5] = (KIPlayer)param_1;
  if (*(int *)(this + 8) == 0) {
    return;
  }
  (*(code *)(DAT_001abde4 + 0x1abde8))();
  return;
}

```

## target disasm
```
  000b5b2c: mov r2,r0
  000b5b2e: ldr r0,[r0,#0x8]
  000b5b30: strb.w r1,[r2,#0xf5]
  000b5b34: cmp r0,#0x0
  000b5b36: it ne
  000b5b38: b.ne.w 0x001abdd8
  000b5b3c: bx lr
```
