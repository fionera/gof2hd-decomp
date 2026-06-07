# BombGun::setPlayer
elf 0x147868 body 6
Sig: undefined __thiscall setPlayer(BombGun * this, PlayerEgo * param_1)

## decompile
```c

/* BombGun::setPlayer(PlayerEgo*) */

void __thiscall BombGun::setPlayer(BombGun *this,PlayerEgo *param_1)

{
  *(PlayerEgo **)(this + 0xec) = param_1;
  return;
}

```

## target disasm
```
  00157868: str.w r1,[r0,#0xec]
  0015786c: bx lr
```
