# PaintCanvas::ResourceLoaded
elf 0x7c7a4 body 74
Sig: undefined __thiscall ResourceLoaded(PaintCanvas * this, uint param_1, ResourceType param_2)

## decompile
```c

/* AbyssEngine::PaintCanvas::ResourceLoaded(unsigned int, AbyssEngine::ResourceType) */

bool __thiscall
AbyssEngine::PaintCanvas::ResourceLoaded(PaintCanvas *this,uint param_1,undefined4 param_3)

{
  int iVar1;
  uint uVar2;
  
  switch(param_3) {
  case 1:
    uVar2 = *(uint *)(this + 0x140);
    break;
  case 2:
    iVar1 = *(int *)(*(int *)(this + 0x138) + param_1 * 4);
    if (*(int *)(iVar1 + 4) != 2) {
      return false;
    }
    return *(int *)(iVar1 + 8) != -1;
  case 3:
    uVar2 = *(uint *)(this + 0x14c);
    break;
  case 4:
    uVar2 = *(uint *)(this + 0x24);
    break;
  case 5:
    uVar2 = *(uint *)(this + 0x158);
    break;
  case 6:
    uVar2 = *(uint *)(this + 0x174);
    break;
  default:
    return false;
  }
  return param_1 < uVar2;
}

```

## target disasm
```
  0008c7a4: subs r2,#0x1
  0008c7a6: cmp r2,#0x5
  0008c7a8: bhi 0x0008c7d2
  0008c7aa: tbb [pc,r2]
  0008c7b4: ldr.w r2,[r0,#0x140]
  0008c7b8: b 0x0008c7ea
  0008c7ba: ldr.w r0,[r0,#0x138]
  0008c7be: ldr.w r0,[r0,r1,lsl #0x2]
  0008c7c2: ldr r1,[r0,#0x4]
  0008c7c4: cmp r1,#0x2
  0008c7c6: bne 0x0008c7d2
  0008c7c8: ldr r0,[r0,#0x8]
  0008c7ca: adds r0,#0x1
  0008c7cc: it ne
  0008c7ce: mov.ne r0,#0x1
  0008c7d0: bx lr
  0008c7d2: movs r0,#0x0
  0008c7d4: bx lr
  0008c7d6: ldr.w r2,[r0,#0x14c]
  0008c7da: b 0x0008c7ea
  0008c7dc: ldr r2,[r0,#0x24]
  0008c7de: b 0x0008c7ea
  0008c7e0: ldr.w r2,[r0,#0x158]
  0008c7e4: b 0x0008c7ea
  0008c7e6: ldr.w r2,[r0,#0x174]
  0008c7ea: movs r0,#0x0
  0008c7ec: cmp r2,r1
  0008c7ee: it hi
  0008c7f0: mov.hi r0,#0x1
  0008c7f2: bx lr
```
