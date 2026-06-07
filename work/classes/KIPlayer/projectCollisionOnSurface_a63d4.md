# KIPlayer::projectCollisionOnSurface
elf 0xa63d4 body 10
Sig: undefined __stdcall projectCollisionOnSurface(Vector * param_1)

## decompile
```c

/* KIPlayer::projectCollisionOnSurface(AbyssEngine::AEMath::Vector const&) */

void KIPlayer::projectCollisionOnSurface(Vector *param_1)

{
  *(undefined4 *)param_1 = 0;
  *(undefined4 *)(param_1 + 4) = 0;
  *(undefined4 *)(param_1 + 8) = 0;
  return;
}

```

## target disasm
```
  000b63d4: movs r1,#0x0
  000b63d6: strd r1,r1,[r0,#0x0]
  000b63da: str r1,[r0,#0x8]
  000b63dc: bx lr
```
