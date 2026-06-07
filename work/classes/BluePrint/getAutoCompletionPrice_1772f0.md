# BluePrint::getAutoCompletionPrice
elf 0x1772f0 body 72
Sig: undefined __thiscall getAutoCompletionPrice(BluePrint * this)

## decompile
```c

/* BluePrint::getAutoCompletionPrice() */

int __thiscall BluePrint::getAutoCompletionPrice(BluePrint *this)

{
  int iVar1;
  uint in_fpscr;
  float fVar2;
  
  if (*(int *)(this + 0x20) == 0xd2) {
    iVar1 = getIngredientsValue(this);
    return iVar1 + DAT_00187338;
  }
  iVar1 = Item::getMaxPrice(*(Item **)(*(int *)(**(int **)(DAT_0018733c + 0x18730e) + 4) +
                                      *(int *)(this + 0x20) * 4));
  fVar2 = (float)VectorSignedToFloat(*(int *)(this + 0x24) * iVar1,(byte)(in_fpscr >> 0x16) & 3);
  return (int)(fVar2 * 1.25);
}

```

## target disasm
```
  001872f0: push {r4,r6,r7,lr}
  001872f2: add r7,sp,#0x8
  001872f4: mov r4,r0
  001872f6: ldr r0,[r0,#0x20]
  001872f8: cmp r0,#0xd2
  001872fa: bne 0x00187308
  001872fc: mov r0,r4
  001872fe: blx 0x00078028
  00187302: ldr r1,[0x00187338]
  00187304: add r0,r1
  00187306: pop {r4,r6,r7,pc}
  00187308: ldr r1,[0x0018733c]
  0018730a: add r1,pc
  0018730c: ldr r1,[r1,#0x0]
  0018730e: ldr r1,[r1,#0x0]
  00187310: ldr r1,[r1,#0x4]
  00187312: ldr.w r0,[r1,r0,lsl #0x2]
  00187316: blx 0x00071cbc
  0018731a: ldr r1,[r4,#0x24]
  0018731c: vmov.f32 s0,0x3fa00000
  00187320: muls r0,r1
  00187322: vmov s2,r0
  00187326: vcvt.f32.s32 s2,s2
  0018732a: vmul.f32 s0,s2,s0
  0018732e: vcvt.s32.f32 s0,s0
  00187332: vmov r0,s0
  00187336: pop {r4,r6,r7,pc}
```
