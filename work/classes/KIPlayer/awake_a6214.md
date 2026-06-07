# KIPlayer::awake
elf 0xa6214 body 16
Sig: undefined __thiscall awake(KIPlayer * this)

## decompile
```c

/* KIPlayer::awake() */

void __thiscall KIPlayer::awake(KIPlayer *this)

{
  *(undefined4 *)(this + 0x88) = 1;
  (*(code *)(DAT_001abad4 + 0x1abad8))(*(undefined4 *)(this + 4),1);
  return;
}

```

## target disasm
```
  000b6214: ldr r1,[r0,#0x4]
  000b6216: movs r2,#0x1
  000b6218: str.w r2,[r0,#0x88]
  000b621c: mov r0,r1
  000b621e: movs r1,#0x1
  000b6220: b.w 0x001abac8
```
