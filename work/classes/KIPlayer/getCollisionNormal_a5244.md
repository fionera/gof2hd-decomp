# KIPlayer::getCollisionNormal
elf 0xa5244 body 10
Sig: undefined __stdcall getCollisionNormal(Vector * param_1)

## decompile
```c

/* KIPlayer::getCollisionNormal(AbyssEngine::AEMath::Vector const&) */

void KIPlayer::getCollisionNormal(Vector *param_1)

{
  *(undefined4 *)param_1 = 0;
  *(undefined4 *)(param_1 + 4) = 0;
  *(undefined4 *)(param_1 + 8) = 0;
  return;
}

```

## target disasm
```
  000b5244: movs r1,#0x0
  000b5246: strd r1,r1,[r0,#0x0]
  000b524a: str r1,[r0,#0x8]
  000b524c: bx lr
```
