# PlayerTurret::setHost
elf 0x15741e body 14
Sig: undefined __thiscall setHost(PlayerTurret * this, KIPlayer * param_1, Vector * param_2)

## decompile
```c

/* PlayerTurret::setHost(KIPlayer*, AbyssEngine::AEMath::Vector const&) */

void __thiscall PlayerTurret::setHost(PlayerTurret *this,KIPlayer *param_1,Vector *param_2)

{
  *(KIPlayer **)(this + 0x154) = param_1;
  (*(code *)(DAT_001ac5b4 + 0x1ac5b8))(this + 0x158,param_2);
  return;
}

```

## target disasm
```
  0016741e: str.w r1,[r0,#0x154]
  00167422: add.w r0,r0,#0x158
  00167426: mov r1,r2
  00167428: b.w 0x001ac5a8
```
