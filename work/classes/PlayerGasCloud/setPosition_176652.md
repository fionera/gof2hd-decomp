# PlayerGasCloud::setPosition
elf 0x176652 body 16
Sig: undefined __thiscall setPosition(PlayerGasCloud * this, Vector * param_1)

## decompile
```c

/* PlayerGasCloud::setPosition(AbyssEngine::AEMath::Vector const&) */

void __thiscall PlayerGasCloud::setPosition(PlayerGasCloud *this,Vector *param_1)

{
  (*(code *)(DAT_001ac294 + 0x1ac298))(*(undefined4 *)(this + 8));
  return;
}

```

## target disasm
```
  00186652: ldr r0,[r0,#0x8]
  00186654: b.w 0x001ac288
  001ac288: bx pc
  001ac28c: ldr r12,[0x1ac294]
  001ac290: add pc,r12,pc
```
