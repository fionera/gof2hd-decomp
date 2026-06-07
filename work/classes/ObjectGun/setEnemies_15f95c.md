# ObjectGun::setEnemies
elf 0x15f95c body 6
Sig: undefined __stdcall setEnemies(Array * param_1)

## decompile
```c

/* ObjectGun::setEnemies(Array<Player*>*) */

void ObjectGun::setEnemies(Array *param_1)

{
  (*(code *)(DAT_001ac634 + 0x1ac638))(*(undefined4 *)(param_1 + 8));
  return;
}

```

## target disasm
```
  0016f95c: ldr r0,[r0,#0x8]
  0016f95e: b.w 0x001ac628
```
