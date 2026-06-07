# PlayerEgo::setPosition
elf 0x9b948 body 14
Sig: undefined __stdcall setPosition(Vector param_1)

## decompile
```c

/* PlayerEgo::setPosition(AbyssEngine::AEMath::Vector) */

void PlayerEgo::setPosition(void)

{
  (*(code *)(DAT_001abae4 + 0x1abae8))();
  return;
}

```

## target disasm
```
  000ab948: b.w 0x001abad8
  001abad8: bx pc
  001abadc: ldr r12,[0x1abae4]
  001abae0: add pc,r12,pc
```
