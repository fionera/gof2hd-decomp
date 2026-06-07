# PlayerAsteroid::setPosition
elf 0xe29d6 body 6
Sig: undefined __stdcall setPosition(Vector * param_1)

## decompile
```c

/* PlayerAsteroid::setPosition(AbyssEngine::AEMath::Vector const&) */

void PlayerAsteroid::setPosition(Vector *param_1)

{
  (*(code *)(DAT_001ac294 + 0x1ac298))(*(undefined4 *)(param_1 + 8));
  return;
}

```

## target disasm
```
  000f29d6: ldr r0,[r0,#0x8]
  000f29d8: b.w 0x001ac288
```
