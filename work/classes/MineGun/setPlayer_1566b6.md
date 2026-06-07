# MineGun::setPlayer
elf 0x1566b6 body 6
Sig: undefined __thiscall setPlayer(MineGun * this, PlayerEgo * param_1)

## decompile
```c

/* MineGun::setPlayer(PlayerEgo*) */

void __thiscall MineGun::setPlayer(MineGun *this,PlayerEgo *param_1)

{
  *(PlayerEgo **)(this + 0xb0) = param_1;
  return;
}

```

## target disasm
```
  001666b6: str.w r1,[r0,#0xb0]
  001666ba: bx lr
```
