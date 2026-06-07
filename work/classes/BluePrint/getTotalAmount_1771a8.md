# BluePrint::getTotalAmount
elf 0x1771a8 body 62
Sig: undefined __thiscall getTotalAmount(BluePrint * this, int param_1)

## decompile
```c

/* BluePrint::getTotalAmount(int) */

undefined4 __thiscall BluePrint::getTotalAmount(BluePrint *this,int param_1)

{
  int iVar1;
  uint *puVar2;
  uint uVar3;
  
  iVar1 = getIngredientList(this);
  puVar2 = (uint *)getQuantityList(this);
  uVar3 = 0;
  while( true ) {
    if (*puVar2 <= uVar3) {
      return 0;
    }
    if (*(int *)(*(int *)(iVar1 + 4) + uVar3 * 4) == param_1) break;
    uVar3 = uVar3 + 1;
  }
  return *(undefined4 *)(puVar2[1] + uVar3 * 4);
}

```

## target disasm
```
  001871a8: push {r4,r5,r6,r7,lr}
  001871aa: add r7,sp,#0xc
  001871ac: push.w r11
  001871b0: mov r4,r1
  001871b2: mov r6,r0
  001871b4: blx 0x00073954
  001871b8: mov r5,r0
  001871ba: mov r0,r6
  001871bc: blx 0x00073960
  001871c0: ldr r2,[r0,#0x0]
  001871c2: movs r1,#0x0
  001871c4: b 0x001871d2
  001871c6: ldr r3,[r5,#0x4]
  001871c8: ldr.w r3,[r3,r1,lsl #0x2]
  001871cc: cmp r3,r4
  001871ce: beq 0x001871da
  001871d0: adds r1,#0x1
  001871d2: cmp r1,r2
  001871d4: bcc 0x001871c6
  001871d6: movs r0,#0x0
  001871d8: b 0x001871e0
  001871da: ldr r0,[r0,#0x4]
  001871dc: ldr.w r0,[r0,r1,lsl #0x2]
  001871e0: pop.w r11
  001871e4: pop {r4,r5,r6,r7,pc}
```
