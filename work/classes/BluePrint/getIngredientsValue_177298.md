# BluePrint::getIngredientsValue
elf 0x177298 body 82
Sig: undefined __thiscall getIngredientsValue(BluePrint * this)

## decompile
```c

/* BluePrint::getIngredientsValue() */

int __thiscall BluePrint::getIngredientsValue(BluePrint *this)

{
  uint *puVar1;
  int iVar2;
  uint uVar3;
  int iVar4;
  int *piVar5;
  
  puVar1 = (uint *)getIngredientList(this);
  iVar4 = 0;
  if (puVar1 != (uint *)0x0) {
    piVar5 = *(int **)(DAT_001872ec + 0x1872b4);
    for (uVar3 = 0; uVar3 < *puVar1; uVar3 = uVar3 + 1) {
      iVar2 = Item::getSinglePrice
                        (*(Item **)(*(int *)(*piVar5 + 4) + *(int *)(puVar1[1] + uVar3 * 4) * 4));
      iVar4 = *(int *)(*(int *)(*(int *)this + 4) + uVar3 * 4) * iVar2 + iVar4;
    }
  }
  return iVar4;
}

```

## target disasm
```
  00187298: push {r4,r5,r6,r7,lr}
  0018729a: add r7,sp,#0xc
  0018729c: push {r8,r9,r11}
  001872a0: mov r8,r0
  001872a2: blx 0x00073954
  001872a6: mov r6,r0
  001872a8: movs r5,#0x0
  001872aa: cbz r0,0x001872e2
  001872ac: ldr r0,[0x001872ec]
  001872ae: movs r4,#0x0
  001872b0: add r0,pc
  001872b2: ldr.w r9,[r0,#0x0]
  001872b6: b 0x001872dc
  001872b8: ldr r0,[r6,#0x4]
  001872ba: ldr.w r1,[r9,#0x0]
  001872be: ldr.w r0,[r0,r4,lsl #0x2]
  001872c2: ldr r1,[r1,#0x4]
  001872c4: ldr.w r0,[r1,r0,lsl #0x2]
  001872c8: blx 0x00071944
  001872cc: ldr.w r1,[r8,#0x0]
  001872d0: ldr r1,[r1,#0x4]
  001872d2: ldr.w r1,[r1,r4,lsl #0x2]
  001872d6: adds r4,#0x1
  001872d8: mla r5,r1,r0,r5
  001872dc: ldr r0,[r6,#0x0]
  001872de: cmp r4,r0
  001872e0: bcc 0x001872b8
  001872e2: mov r0,r5
  001872e4: pop.w {r8,r9,r11}
  001872e8: pop {r4,r5,r6,r7,pc}
```
