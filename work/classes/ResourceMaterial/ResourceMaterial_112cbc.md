# ResourceMaterial::ResourceMaterial
elf 0x112cbc body 48
Sig: undefined __thiscall ResourceMaterial(ResourceMaterial * this, ushort param_1, ushort param_2, BlendMode param_3)

## decompile
```c

/* AbyssEngine::ResourceMaterial::ResourceMaterial(unsigned short, unsigned short,
   AbyssEngine::BlendMode) */

void __thiscall
AbyssEngine::ResourceMaterial::ResourceMaterial
          (ResourceMaterial *this,undefined2 param_1,undefined2 param_2,undefined4 param_4)

{
  undefined4 uVar1;
  int iVar2;
  
  uVar1 = DAT_00122cec;
  *(undefined4 *)(this + 0x10) = param_4;
  *(undefined4 *)(this + 0x14) = 0;
  *(undefined4 *)(this + 0x18) = 0;
  *(undefined4 *)(this + 0x1c) = uVar1;
  *(undefined4 *)(this + 0x20) = 0;
  *(undefined4 *)(this + 0x24) = 0;
  for (iVar2 = 0; iVar2 != 8; iVar2 = iVar2 + 1) {
    *(undefined2 *)(this + iVar2 * 2) = 0xffff;
  }
  *(undefined2 *)(this + 2) = param_2;
  *(undefined2 *)this = param_1;
  return;
}

```

## target disasm
```
  00122cbc: push {r4,r6,r7,lr}
  00122cbe: add r7,sp,#0x8
  00122cc0: ldr r4,[0x00122cec]
  00122cc2: mov.w r12,#0x0
  00122cc6: strd r3,r12,[r0,#0x10]
  00122cca: movw r3,#0xffff
  00122cce: strd r12,r4,[r0,#0x18]
  00122cd2: strd r12,r12,[r0,#0x20]
  00122cd6: b 0x00122ce0
  00122cd8: strh.w r3,[r0,r12,lsl #0x1]
  00122cdc: add.w r12,r12,#0x1
  00122ce0: cmp.w r12,#0x8
  00122ce4: bne 0x00122cd8
  00122ce6: strh r2,[r0,#0x2]
  00122ce8: strh r1,[r0,#0x0]
  00122cea: pop {r4,r6,r7,pc}
```
