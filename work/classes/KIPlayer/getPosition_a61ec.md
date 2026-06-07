# KIPlayer::getPosition
elf 0xa61ec body 14
Sig: undefined __thiscall getPosition(KIPlayer * this)

## decompile
```c

/* KIPlayer::getPosition() */

void __thiscall KIPlayer::getPosition(KIPlayer *this)

{
  AbyssEngine::AEMath::MatrixGetPosition((Matrix *)this);
  return;
}

```

## target disasm
```
  000b61ec: push {r7,lr}
  000b61ee: mov r7,sp
  000b61f0: ldr r1,[r1,#0x4]
  000b61f2: adds r1,#0x4
  000b61f4: blx 0x0006f16c
  000b61f8: pop {r7,pc}
```
