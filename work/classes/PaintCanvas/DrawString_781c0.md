# PaintCanvas::DrawString
elf 0x781c0 body 82
Sig: undefined __thiscall DrawString(PaintCanvas * this, uint param_1, ushort * param_2, int param_3, int param_4, bool param_5)

## decompile
```c

/* AbyssEngine::PaintCanvas::DrawString(unsigned int, unsigned short const*, int, int, bool) */

undefined8 __thiscall
AbyssEngine::PaintCanvas::DrawString
          (PaintCanvas *this,uint param_1,ushort *param_2,int param_3,int param_4,bool param_5)

{
  PaintCanvas *unaff_r4;
  Engine *unaff_r5;
  
  if (param_1 < *(uint *)(this + 0x140)) {
    SetTexture((uint)this,*(uint *)(*(int *)(*(int *)(this + 0x144) + param_1 * 4) + 8));
    unaff_r5 = *(Engine **)(this + 0x34);
    ImageFontDrawString(*(ImageFont **)(*(int *)(this + 0x144) + param_1 * 4),param_2,param_3,
                        param_4,this,unaff_r5,param_5);
    unaff_r4 = this;
  }
  return CONCAT44(unaff_r5,unaff_r4);
}

```

## target disasm
```
  000881c0: push {r4,r5,r6,r7,lr}
  000881c2: add r7,sp,#0xc
  000881c4: push {r4,r5,r6,r7,r8,r9,r10}
  000881c8: mov r6,r0
  000881ca: ldr.w r0,[r0,#0x140]
  000881ce: mov r8,r3
  000881d0: mov r5,r2
  000881d2: mov r4,r1
  000881d4: cmp r0,r1
  000881d6: bls 0x0008820c
  000881d8: ldrd r9,r10,[r7,#0x8]
  000881dc: mov.w r2,#0xffffffff
  000881e0: ldr.w r0,[r6,#0x144]
  000881e4: ldr.w r0,[r0,r4,lsl #0x2]
  000881e8: ldr r1,[r0,#0x8]
  000881ea: mov r0,r6
  000881ec: blx 0x0006fa6c
  000881f0: ldr.w r0,[r6,#0x144]
  000881f4: mov r2,r8
  000881f6: ldr r1,[r6,#0x34]
  000881f8: mov r3,r9
  000881fa: ldr.w r0,[r0,r4,lsl #0x2]
  000881fe: strd r6,r1,[sp,#0x0]
  00088202: mov r1,r5
  00088204: str.w r10,[sp,#0x8]
  00088208: blx 0x0006fa78
  0008820c: pop.w {r0,r1,r2,r3,r8,r9,r10}
  00088210: pop {r4,r5,r6,r7,pc}
```
