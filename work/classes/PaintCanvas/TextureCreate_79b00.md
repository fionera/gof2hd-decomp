# PaintCanvas::TextureCreate
elf 0x79b00 body 18
Sig: undefined __stdcall TextureCreate(ushort param_1, uint * param_2, bool param_3)

## decompile
```c

/* AbyssEngine::PaintCanvas::TextureCreate(unsigned short, unsigned int&, bool) */

void AbyssEngine::PaintCanvas::TextureCreate(ushort param_1,uint *param_2,bool param_3)

{
  bool in_r3;
  
  TextureCreate((PaintCanvas *)(uint)param_1,(ushort)param_2,(_func_void_Image_ptr_void_ptr *)0x0,
                (void *)0x0,(uint *)(uint)param_3,in_r3);
  return;
}

```

## target disasm
```
  00089b00: push {r5,r6,r7,lr}
  00089b02: add r7,sp,#0x8
  00089b04: strd r2,r3,[sp,#0x0]
  00089b08: movs r2,#0x0
  00089b0a: movs r3,#0x0
  00089b0c: blx 0x0006fba4
  00089b10: pop {r2,r3,r7,pc}
```
