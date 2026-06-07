# PlayerAsteroid::translate
elf 0xe29dc body 6
Sig: undefined __stdcall translate(Vector * param_1)

## decompile
```c

/* PlayerAsteroid::translate(AbyssEngine::AEMath::Vector const&) */

void PlayerAsteroid::translate(Vector *param_1)

{
  (*(code *)(DAT_001abef4 + 0x1abef8))(*(undefined4 *)(param_1 + 8));
  return;
}

```

## target disasm
```
  000f29dc: ldr r0,[r0,#0x8]
  000f29de: b.w 0x001abee8
```
