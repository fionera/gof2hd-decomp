# ImagePart::draw
elf 0xa7aac body 68
Sig: undefined __thiscall draw(ImagePart * this, int param_1, int param_2, bool param_3)

## decompile
```c

/* ImagePart::draw(int, int, bool) */

void __thiscall ImagePart::draw(ImagePart *this,int param_1,int param_2,bool param_3)

{
  AbyssEngine::PaintCanvas::DrawImage2D
            ((PaintCanvas *)**(undefined4 **)(DAT_000b7af0 + 0xb7abe),this->id,param_1,
             this->pos_y + param_2,this->scale_x,this->scale_y,'\x11',(byte)this->field_04 | 1,
             param_3);
  return;
}

```

## target disasm
```
  000b7aac: push {r4,r5,r6,r7,lr}
  000b7aae: add r7,sp,#0xc
  000b7ab0: push.w r8
  000b7ab4: sub sp,#0x18
  000b7ab6: mov r12,r1
  000b7ab8: ldr r1,[0x000b7af0]
  000b7aba: add r1,pc
  000b7abc: ldr.w lr,[r1,#0x0]
  000b7ac0: ldr r1,[r0,#0x0]
  000b7ac2: ldrd r8,r5,[r0,#0x8]
  000b7ac6: ldr r6,[r0,#0x10]
  000b7ac8: ldrb r4,[r0,#0x4]
  000b7aca: ldr.w r0,[lr,#0x0]
  000b7ace: mov.w lr,#0x11
  000b7ad2: orr r4,r4,#0x1
  000b7ad6: stm sp,{r5,r6,lr}
  000b7ada: strd r4,r3,[sp,#0xc]
  000b7ade: add.w r3,r8,r2
  000b7ae2: mov r2,r12
  000b7ae4: blx 0x00073564
  000b7ae8: add sp,#0x18
  000b7aea: pop.w r8
  000b7aee: pop {r4,r5,r6,r7,pc}
```
