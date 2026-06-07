# SpriteGun::update
elf 0x169abe body 16
Sig: undefined __stdcall update(int param_1)

## decompile
```c

/* SpriteGun::update(int) */

void SpriteGun::update(int param_1)

{
  (*(code *)(DAT_001ac684 + 0x1ac688))(*(undefined4 *)(param_1 + 8));
  return;
}

```

## target disasm
```
  00179abe: ldr r0,[r0,#0x8]
  00179ac0: b.w 0x001ac678
  001ac678: bx pc
  001ac67c: ldr r12,[0x1ac684]
  001ac680: add pc,r12,pc
```
