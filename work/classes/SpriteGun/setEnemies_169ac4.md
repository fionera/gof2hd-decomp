# SpriteGun::setEnemies
elf 0x169ac4 body 6
Sig: undefined __stdcall setEnemies(Array * param_1)

## decompile
```c

/* SpriteGun::setEnemies(Array<Player*>*) */

void SpriteGun::setEnemies(Array *param_1)

{
  (*(code *)(DAT_001ac634 + 0x1ac638))(*(undefined4 *)(param_1 + 8));
  return;
}

```

## target disasm
```
  00179ac4: ldr r0,[r0,#0x8]
  00179ac6: b.w 0x001ac628
```
