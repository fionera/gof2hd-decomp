# PaintCanvas::MeshChangeShaderAnimValue
elf 0x7a2a4 body 48
Sig: undefined __thiscall MeshChangeShaderAnimValue(PaintCanvas * this, Mesh * param_1, float param_2, uint param_3)

## decompile
```c

/* AbyssEngine::PaintCanvas::MeshChangeShaderAnimValue(AbyssEngine::Mesh*, float, unsigned int) */

void __thiscall
AbyssEngine::PaintCanvas::MeshChangeShaderAnimValue
          (PaintCanvas *this,Mesh *param_1,float param_2,uint param_3)

{
  int in_r3;
  Mesh *pMVar1;
  
  if (param_1 == (Mesh *)0x0) {
    return;
  }
  if (in_r3 == 4) {
    pMVar1 = param_1 + 0x24;
  }
  else if (in_r3 == 2) {
    pMVar1 = param_1 + 0x20;
  }
  else {
    if (in_r3 != 1) goto LAB_0008a2cc;
    pMVar1 = param_1 + 0x1c;
  }
  *(uint *)pMVar1 = param_3;
LAB_0008a2cc:
  (*(code *)(DAT_001ab254 + 0x1ab258))(this,*(undefined4 *)(param_1 + 0x34));
  return;
}

```

## target disasm
```
  0008a2a4: cbz r1,0x0008a2bc
  0008a2a6: vmov s0,r2
  0008a2aa: cmp r3,#0x4
  0008a2ac: beq 0x0008a2be
  0008a2ae: cmp r3,#0x2
  0008a2b0: beq 0x0008a2c4
  0008a2b2: cmp r3,#0x1
  0008a2b4: bne 0x0008a2cc
  0008a2b6: add.w r12,r1,#0x1c
  0008a2ba: b 0x0008a2c8
  0008a2bc: bx lr
  0008a2be: add.w r12,r1,#0x24
  0008a2c2: b 0x0008a2c8
  0008a2c4: add.w r12,r1,#0x20
  0008a2c8: vstr.32 s0,[r12]
  0008a2cc: ldr r1,[r1,#0x34]
  0008a2ce: b.w 0x001ab248
  001ab248: bx pc
```
