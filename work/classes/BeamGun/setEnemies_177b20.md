# BeamGun::setEnemies
elf 0x177b20 body 16
Sig: undefined __stdcall setEnemies(Array * param_1)

## decompile
```c

/* BeamGun::setEnemies(Array<Player*>*) */

void BeamGun::setEnemies(Array *param_1)

{
  (*(code *)(DAT_001ac634 + 0x1ac638))(*(undefined4 *)(param_1 + 8));
  return;
}

```

## target disasm
```
  00187b20: ldr r0,[r0,#0x8]
  00187b22: b.w 0x001ac628
  001ac628: bx pc
  001ac62c: ldr r12,[0x1ac634]
  001ac630: add pc,r12,pc
```
