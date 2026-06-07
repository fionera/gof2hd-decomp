# PaintCanvas::MeshCloneMaterial
elf 0x7a380 body 74
Sig: undefined __thiscall MeshCloneMaterial(PaintCanvas * this, uint param_1, uint * param_2)

## decompile
```c

/* AbyssEngine::PaintCanvas::MeshCloneMaterial(unsigned int, unsigned int&) */

void __thiscall
AbyssEngine::PaintCanvas::MeshCloneMaterial(PaintCanvas *this,uint param_1,uint *param_2)

{
  Material *this_00;
  uint uVar1;
  
  if (param_1 < *(uint *)(this + 0x24)) {
    this_00 = operator_new(0x74);
    Material::Material(this_00,*(Material **)(*(int *)(*(int *)(this + 0x28) + param_1 * 4) + 0x30))
    ;
    ArrayAdd<AbyssEngine::Material*>(this_00,(Array *)(this + 0x174));
    uVar1 = *(int *)(this + 0x174) - 1;
  }
  else {
    uVar1 = 0xffffffff;
  }
  *param_2 = uVar1;
  return;
}

```

## target disasm
```
  0008a380: push {r4,r5,r6,r7,lr}
  0008a382: add r7,sp,#0xc
  0008a384: push.w r8
  0008a388: mov r5,r0
  0008a38a: ldr r0,[r0,#0x24]
  0008a38c: mov r8,r2
  0008a38e: mov r4,r1
  0008a390: cmp r0,r1
  0008a392: bls 0x0008a3bc
  0008a394: movs r0,#0x74
  0008a396: blx 0x0006eb24
  0008a39a: mov r6,r0
  0008a39c: ldr r0,[r5,#0x28]
  0008a39e: ldr.w r0,[r0,r4,lsl #0x2]
  0008a3a2: ldr r1,[r0,#0x30]
  0008a3a4: mov r0,r6
  0008a3a6: blx 0x0006fc7c
  0008a3aa: add.w r1,r5,#0x174
  0008a3ae: mov r0,r6
  0008a3b0: blx 0x0006fc40
  0008a3b4: ldr.w r0,[r5,#0x174]
  0008a3b8: subs r0,#0x1
  0008a3ba: b 0x0008a3c0
  0008a3bc: mov.w r0,#0xffffffff
  0008a3c0: str.w r0,[r8,#0x0]
  0008a3c4: pop.w r8
  0008a3c8: pop {r4,r5,r6,r7,pc}
```
