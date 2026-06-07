# SpriteGun::setEnemy
elf 0x169aca body 18
Sig: undefined __stdcall setEnemy(Player * param_1)

## decompile
```c

/* SpriteGun::setEnemy(Player*) */

void SpriteGun::setEnemy(Player *param_1)

{
                    /* WARNING: Could not recover jumptable at 0x001ac690. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_001ac694 + 0x1ac698))(*(undefined4 *)(param_1 + 8));
  return;
}

```

## target disasm
```
  00077c98: adr r12,0x177ca0
  00077c9c: add r12,r12,#0xab000
  00077ca0: ldr pc,[r12,#0xb80]!
  00179aca: ldr r0,[r0,#0x8]
  00179acc: b.w 0x001ac688
```
