# PlayerTurret::setScaling
elf 0x1573f0 body 14
Sig: undefined __thiscall setScaling(PlayerTurret * this, float param_1)

## decompile
```c

/* PlayerTurret::setScaling(float) */

void __thiscall PlayerTurret::setScaling(PlayerTurret *this,float param_1)

{
  (*(code *)(DAT_001abd94 + 0x1abd98))(*(undefined4 *)(this + 0x148));
  return;
}

```

## target disasm
```
  001673f0: ldr.w r0,[r0,#0x148]
  001673f4: mov r2,r1
  001673f6: mov r3,r1
  001673f8: b.w 0x001abd88
  001abd88: bx pc
```
