# PaintCanvas::MaterialResourceChangeTexture
elf 0x7c238 body 60
Sig: undefined __thiscall MaterialResourceChangeTexture(PaintCanvas * this, uint param_1, uint param_2, int param_3)

## decompile
```c

/* AbyssEngine::PaintCanvas::MaterialResourceChangeTexture(unsigned int, unsigned int, int) */

void __thiscall
AbyssEngine::PaintCanvas::MaterialResourceChangeTexture
          (PaintCanvas *this,uint param_1,uint param_2,int param_3)

{
  int iVar1;
  uint uVar2;
  
  if (((((uint)param_3 < 8) && (iVar1 = FindResource(this,(ushort)param_1), iVar1 != 0)) &&
      (uVar2 = *(uint *)(iVar1 + 8), uVar2 != 0xffffffff)) && (uVar2 < *(uint *)(this + 0x174))) {
    *(uint *)(*(int *)(*(int *)(this + 0x178) + uVar2 * 4) + param_3 * 4) = param_2;
  }
  return;
}

```

## target disasm
```
  0008c238: push {r4,r5,r6,r7,lr}
  0008c23a: add r7,sp,#0xc
  0008c23c: push.w r11
  0008c240: mov r4,r3
  0008c242: mov r5,r2
  0008c244: mov r6,r0
  0008c246: cmp r3,#0x7
  0008c248: bhi 0x0008c26e
  0008c24a: uxth r1,r1
  0008c24c: mov r0,r6
  0008c24e: blx 0x0006fb98
  0008c252: cbz r0,0x0008c26e
  0008c254: ldr r0,[r0,#0x8]
  0008c256: adds r1,r0,#0x1
  0008c258: beq 0x0008c26e
  0008c25a: ldr.w r1,[r6,#0x174]
  0008c25e: cmp r0,r1
  0008c260: bcs 0x0008c26e
  0008c262: ldr.w r1,[r6,#0x178]
  0008c266: ldr.w r0,[r1,r0,lsl #0x2]
  0008c26a: str.w r5,[r0,r4,lsl #0x2]
  0008c26e: pop.w r11
  0008c272: pop {r4,r5,r6,r7,pc}
```
