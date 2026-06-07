# PaintCanvas::MeshGetTriCount
elf 0x7a5cc body 38
Sig: undefined __thiscall MeshGetTriCount(PaintCanvas * this, Mesh * param_1)

## decompile
```c

/* AbyssEngine::PaintCanvas::MeshGetTriCount(AbyssEngine::Mesh*) */

int __thiscall AbyssEngine::PaintCanvas::MeshGetTriCount(PaintCanvas *this,Mesh *param_1)

{
  int iVar1;
  int iVar2;
  
  if (param_1 != (Mesh *)0x0) {
    if (*(Transform **)(param_1 + 0x34) == (Transform *)0x0) {
      iVar1 = 0;
    }
    else {
      iVar1 = TransformGetTriCount(this,*(Transform **)(param_1 + 0x34));
    }
    iVar2 = __aeabi_uidiv(*(undefined2 *)(param_1 + 0x28),3);
    return iVar2 + iVar1;
  }
  return 0;
}

```

## target disasm
```
  0008a5cc: push {r4,r5,r7,lr}
  0008a5ce: add r7,sp,#0x8
  0008a5d0: mov r4,r1
  0008a5d2: cbz r1,0x0008a5e0
  0008a5d4: ldr r1,[r4,#0x34]
  0008a5d6: cbz r1,0x0008a5e4
  0008a5d8: blx 0x0006fc88
  0008a5dc: mov r5,r0
  0008a5de: b 0x0008a5e6
  0008a5e0: movs r0,#0x0
  0008a5e2: pop {r4,r5,r7,pc}
  0008a5e4: movs r5,#0x0
  0008a5e6: ldrh r0,[r4,#0x28]
  0008a5e8: movs r1,#0x3
  0008a5ea: blx 0x0006ec2c
  0008a5ee: add r0,r5
  0008a5f0: pop {r4,r5,r7,pc}
```
