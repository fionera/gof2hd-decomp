# Hud::setCurrentSecondaryWeapon
elf 0x1622ec body 10
Sig: undefined __stdcall setCurrentSecondaryWeapon(Item * param_1)

## decompile
```c

/* Hud::setCurrentSecondaryWeapon(Item*) */

void Hud::setCurrentSecondaryWeapon(Item *param_1)

{
  undefined4 in_r1;
  
  *(undefined4 *)(param_1 + 600) = in_r1;
  (*(code *)(DAT_001ac644 + 0x1ac648))();
  return;
}

```

## target disasm
```
  001722ec: str.w r1,[r0,#0x258]
  001722f0: b.w 0x001ac638
  001ac638: bx pc
```
