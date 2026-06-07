# BeamGun::setEnemy
elf 0x177b26 body 16
Sig: undefined __stdcall setEnemy(Player * param_1)

## decompile
```c

/* BeamGun::setEnemy(Player*) */

void BeamGun::setEnemy(Player *param_1)

{
                    /* WARNING: Could not recover jumptable at 0x001ac690. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_001ac694 + 0x1ac698))(*(undefined4 *)(param_1 + 8));
  return;
}

```

## target disasm
```
  00187b26: ldr r0,[r0,#0x8]
  00187b28: b.w 0x001ac688
  001ac688: bx pc
  001ac68c: ldr r12,[0x1ac694]
  001ac690: add pc,r12,pc
```
