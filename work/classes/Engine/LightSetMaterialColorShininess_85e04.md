# Engine::LightSetMaterialColorShininess
elf 0x85e04 body 32
Sig: undefined __thiscall LightSetMaterialColorShininess(Engine * this, float param_1)

## decompile
```c

/* AbyssEngine::Engine::LightSetMaterialColorShininess(float) */

void __thiscall AbyssEngine::Engine::LightSetMaterialColorShininess(Engine *this,float param_1)

{
  int iVar1;
  undefined4 in_r1;
  
  iVar1 = DAT_00095e24;
  *(undefined4 *)(this + 0x2c8) = in_r1;
  if (**(char **)(iVar1 + 0x95e10) == '\0') {
    (*(code *)(DAT_001ab6b4 + 0x1ab6b8))(0x408,0x1601);
    return;
  }
  return;
}

```

## target disasm
```
  00095e04: mov r2,r1
  00095e06: ldr r1,[0x00095e24]
  00095e08: str.w r2,[r0,#0x2c8]
  00095e0c: add r1,pc
  00095e0e: ldr r1,[r1,#0x0]
  00095e10: ldrb r0,[r1,#0x0]
  00095e12: cbz r0,0x00095e16
  00095e14: bx lr
  00095e16: mov.w r0,#0x408
  00095e1a: movw r1,#0x1601
  00095e1e: b.w 0x001ab6a8
  001ab6a8: bx pc
```
