# ResourceMaterial::ResourceMaterial
elf 0x112d20 body 40
Sig: undefined __thiscall ResourceMaterial(ResourceMaterial * this, ushort param_1, BlendMode param_2)

## decompile
```c

/* AbyssEngine::ResourceMaterial::ResourceMaterial(unsigned short, AbyssEngine::BlendMode) */

void __thiscall
AbyssEngine::ResourceMaterial::ResourceMaterial
          (ResourceMaterial *this,undefined2 param_1,undefined4 param_3)

{
  undefined4 uVar1;
  int iVar2;
  
  uVar1 = DAT_00122d48;
  *(undefined4 *)(this + 0x10) = param_3;
  *(undefined4 *)(this + 0x14) = 0;
  *(undefined4 *)(this + 0x18) = 0;
  *(undefined4 *)(this + 0x1c) = uVar1;
  *(undefined4 *)(this + 0x20) = 0;
  *(undefined4 *)(this + 0x24) = 0;
  for (iVar2 = 0; iVar2 != 8; iVar2 = iVar2 + 1) {
    *(undefined2 *)(this + iVar2 * 2) = 0xffff;
  }
  *(undefined2 *)this = param_1;
  return;
}

```

## target disasm
```
  00122d20: push {r4,r6,r7,lr}
  00122d22: add r7,sp,#0x8
  00122d24: ldr r4,[0x00122d48]
  00122d26: movs r3,#0x0
  00122d28: strd r2,r3,[r0,#0x10]
  00122d2c: movw r2,#0xffff
  00122d30: strd r3,r4,[r0,#0x18]
  00122d34: strd r3,r3,[r0,#0x20]
  00122d38: b 0x00122d40
  00122d3a: strh.w r2,[r0,r3,lsl #0x1]
  00122d3e: adds r3,#0x1
  00122d40: cmp r3,#0x8
  00122d42: bne 0x00122d3a
  00122d44: strh r1,[r0,#0x0]
  00122d46: pop {r4,r6,r7,pc}
```
