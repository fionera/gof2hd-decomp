# KIPlayer::getProjectionVector
elf 0xa63ca body 10
Sig: undefined __stdcall getProjectionVector(Vector * param_1)

## decompile
```c

/* KIPlayer::getProjectionVector(AbyssEngine::AEMath::Vector const&) */

void KIPlayer::getProjectionVector(Vector *param_1)

{
  *(undefined4 *)param_1 = 0;
  *(undefined4 *)(param_1 + 4) = 0;
  *(undefined4 *)(param_1 + 8) = 0;
  return;
}

```

## target disasm
```
  000b63ca: movs r1,#0x0
  000b63cc: strd r1,r1,[r0,#0x0]
  000b63d0: str r1,[r0,#0x8]
  000b63d2: bx lr
```
