# Engine::ShaderUpdate
elf 0x8538c body 34
Sig: undefined __thiscall ShaderUpdate(Engine * this)

## decompile
```c

/* AbyssEngine::Engine::ShaderUpdate() */

void __thiscall AbyssEngine::Engine::ShaderUpdate(Engine *this)

{
  uint uVar1;
  
  for (uVar1 = 0; uVar1 < *(uint *)(this + 0x510); uVar1 = uVar1 + 1) {
    ShaderBaseStruct::Update(*(ShaderBaseStruct **)(*(int *)(this + 0x514) + uVar1 * 4));
  }
  return;
}

```

## target disasm
```
  0009538c: push {r4,r5,r7,lr}
  0009538e: add r7,sp,#0x8
  00095390: mov r4,r0
  00095392: movs r5,#0x0
  00095394: b 0x000953a4
  00095396: ldr.w r0,[r4,#0x514]
  0009539a: ldr.w r0,[r0,r5,lsl #0x2]
  0009539e: blx 0x00070774
  000953a2: adds r5,#0x1
  000953a4: ldr.w r0,[r4,#0x510]
  000953a8: cmp r5,r0
  000953aa: bcc 0x00095396
  000953ac: pop {r4,r5,r7,pc}
```
