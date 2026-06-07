# PaintCanvas::MeshChangeResourceMaterial
elf 0x7a510 body 48
Sig: undefined __thiscall MeshChangeResourceMaterial(PaintCanvas * this, uint param_1, ushort param_2)

## decompile
```c

/* AbyssEngine::PaintCanvas::MeshChangeResourceMaterial(unsigned int, unsigned short) */

void __thiscall
AbyssEngine::PaintCanvas::MeshChangeResourceMaterial(PaintCanvas *this,uint param_1,ushort param_2)

{
  int iVar1;
  
  iVar1 = FindResource(this,param_2);
  if ((iVar1 != 0) && (*(int *)(iVar1 + 8) != -1)) {
    (*(code *)(DAT_001ab274 + 0x1ab278))
              (this,*(undefined4 *)(*(int *)(this + 0x28) + param_1 * 4),
               *(undefined4 *)(*(int *)(this + 0x178) + *(int *)(iVar1 + 8) * 4));
    return;
  }
  return;
}

```

## target disasm
```
  0008a510: push {r4,r5,r7,lr}
  0008a512: add r7,sp,#0x8
  0008a514: mov r5,r1
  0008a516: mov r1,r2
  0008a518: mov r4,r0
  0008a51a: blx 0x0006fb98
  0008a51e: cbz r0,0x0008a53e
  0008a520: ldr r0,[r0,#0x8]
  0008a522: adds r1,r0,#0x1
  0008a524: beq 0x0008a53e
  0008a526: ldr r1,[r4,#0x28]
  0008a528: ldr.w r2,[r4,#0x178]
  0008a52c: ldr.w r1,[r1,r5,lsl #0x2]
  0008a530: ldr.w r2,[r2,r0,lsl #0x2]
  0008a534: mov r0,r4
  0008a536: pop.w {r4,r5,r7,lr}
  0008a53a: b.w 0x001ab268
  0008a53e: pop {r4,r5,r7,pc}
```
