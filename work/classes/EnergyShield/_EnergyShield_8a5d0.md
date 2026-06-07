# EnergyShield::~EnergyShield
elf 0x8a5d0 body 16
Sig: undefined __thiscall ~EnergyShield(EnergyShield * this)

## decompile
```c

/* AbyssEngine::EnergyShield::~EnergyShield() */

void __thiscall AbyssEngine::EnergyShield::~EnergyShield(EnergyShield *this)

{
  ShaderBaseStruct::~ShaderBaseStruct((ShaderBaseStruct *)this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  0009a5d0: push {r7,lr}
  0009a5d2: mov r7,sp
  0009a5d4: blx 0x0007093c
  0009a5d8: pop.w {r7,lr}
  0009a5dc: b.w 0x001ab098
```
