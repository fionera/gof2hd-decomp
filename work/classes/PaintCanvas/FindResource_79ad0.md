# PaintCanvas::FindResource
elf 0x79ad0 body 48
Sig: undefined __thiscall FindResource(PaintCanvas * this, ushort param_1)

## decompile
```c

/* AbyssEngine::PaintCanvas::FindResource(unsigned short) */

ushort * __thiscall AbyssEngine::PaintCanvas::FindResource(PaintCanvas *this,ushort param_1)

{
  uint uVar1;
  ushort *puVar2;
  
  uVar1 = 0;
  while( true ) {
    if (*(uint *)(this + 0x134) <= uVar1) {
      return (ushort *)0x0;
    }
    puVar2 = *(ushort **)(*(int *)(this + 0x138) + uVar1 * 4);
    if ((puVar2 != (ushort *)0x0) && (*puVar2 == param_1)) break;
    uVar1 = uVar1 + 1;
  }
  return puVar2;
}

```

## target disasm
```
  00089ad0: push {r7,lr}
  00089ad2: mov r7,sp
  00089ad4: ldr.w r12,[r0,#0x134]
  00089ad8: movs r3,#0x0
  00089ada: b 0x00089af4
  00089adc: ldr.w r2,[r0,#0x138]
  00089ae0: ldr.w lr,[r2,r3,lsl #0x2]
  00089ae4: cmp.w lr,#0x0
  00089ae8: beq 0x00089af2
  00089aea: ldrh.w r2,[lr,#0x0]
  00089aee: cmp r2,r1
  00089af0: beq 0x00089afc
  00089af2: adds r3,#0x1
  00089af4: cmp r3,r12
  00089af6: bcc 0x00089adc
  00089af8: mov.w lr,#0x0
  00089afc: mov r0,lr
  00089afe: pop {r7,pc}
```
