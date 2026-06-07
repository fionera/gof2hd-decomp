# ImageFactory::drawShip
elf 0x11cad0 body 132
Sig: undefined __thiscall drawShip(ImageFactory * this, int param_1, int param_2, int param_3)

## decompile
```c

/* ImageFactory::drawShip(int, int, int) */

void __thiscall ImageFactory::drawShip(ImageFactory *this,int param_1,int param_2,int param_3)

{
  int iVar1;
  int iVar2;
  uint *puVar3;
  int *piVar4;
  undefined8 uVar5;
  
  piVar4 = *(int **)(DAT_0012cb54 + 0x12cae4);
  puVar3 = *(uint **)(DAT_0012cb58 + 0x12cae8);
  iVar1 = *piVar4;
  AbyssEngine::PaintCanvas::SetColor(*puVar3);
  Sprite::setFrame(*(Sprite **)this,5);
  uVar5 = Sprite::setPosition(*(Sprite **)this,param_2,param_3);
  Sprite::draw((float)uVar5,(float)((ulonglong)uVar5 >> 0x20));
  AbyssEngine::PaintCanvas::Image2DCreate
            ((PaintCanvas *)*puVar3,(short)param_1 + 0x971,(uint *)&stack0xffffffd4);
  AbyssEngine::PaintCanvas::DrawImage2D(*puVar3,-1,param_2);
  iVar2 = *piVar4;
  iVar1 = iVar2 - iVar1;
  if (iVar1 != 0) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar1,iVar2);
  }
  return;
}

```

## target disasm
```
  0012cad0: push {r4,r5,r6,r7,lr}
  0012cad2: add r7,sp,#0xc
  0012cad4: push {r5,r6,r7,r8,r9,r10,r11}
  0012cad8: mov r6,r0
  0012cada: ldr r0,[0x0012cb54]
  0012cadc: mov r8,r1
  0012cade: ldr r1,[0x0012cb58]
  0012cae0: add r0,pc
  0012cae2: mov r9,r3
  0012cae4: add r1,pc
  0012cae6: mov r5,r2
  0012cae8: ldr.w r10,[r0,#0x0]
  0012caec: mov.w r11,#0xffffffff
  0012caf0: ldr r4,[r1,#0x0]
  0012caf2: ldr.w r1,[r10,#0x0]
  0012caf6: ldr r0,[r4,#0x0]
  0012caf8: str r1,[sp,#0x8]
  0012cafa: mov.w r1,#0xffffffff
  0012cafe: blx 0x0006fac0
  0012cb02: ldr r0,[r6,#0x0]
  0012cb04: movs r1,#0x5
  0012cb06: blx 0x00074d70
  0012cb0a: ldr r0,[r6,#0x0]
  0012cb0c: mov r1,r5
  0012cb0e: mov r2,r9
  0012cb10: blx 0x00076060
  0012cb14: ldr r0,[r6,#0x0]
  0012cb16: mov.w r1,#0x3f800000
  0012cb1a: mov.w r2,#0x3f800000
  0012cb1e: blx 0x0007606c
  0012cb22: addw r1,r8,#0x971
  0012cb26: ldr r0,[r4,#0x0]
  0012cb28: add r2,sp,#0x4
  0012cb2a: str.w r11,[sp,#0x4]
  0012cb2e: uxth r1,r1
  0012cb30: blx 0x00071cf8
  0012cb34: ldr r0,[r4,#0x0]
  0012cb36: mov r2,r5
  0012cb38: ldr r1,[sp,#0x4]
  0012cb3a: mov r3,r9
  0012cb3c: blx 0x00072dc0
  0012cb40: ldr r0,[sp,#0x8]
  0012cb42: ldr.w r1,[r10,#0x0]
  0012cb46: subs r0,r1,r0
  0012cb48: itt eq
  0012cb4a: pop.eq.w {r1,r2,r3,r8,r9,r10,r11}
  0012cb4e: pop.eq {r4,r5,r6,r7,pc}
  0012cb50: blx 0x0006e824
```
