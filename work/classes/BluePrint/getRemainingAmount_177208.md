# BluePrint::getRemainingAmount
elf 0x177208 body 48
Sig: undefined __thiscall getRemainingAmount(BluePrint * this, int param_1)

## decompile
```c

/* BluePrint::getRemainingAmount(int) */

undefined4 __thiscall BluePrint::getRemainingAmount(BluePrint *this,int param_1)

{
  int iVar1;
  uint uVar2;
  
  iVar1 = getIngredientList(this);
  uVar2 = 0;
  while( true ) {
    if (**(uint **)this <= uVar2) {
      return 0;
    }
    if (*(int *)(*(int *)(iVar1 + 4) + uVar2 * 4) == param_1) break;
    uVar2 = uVar2 + 1;
  }
  return *(undefined4 *)((*(uint **)this)[1] + uVar2 * 4);
}

```

## target disasm
```
  00187208: push {r4,r5,r7,lr}
  0018720a: add r7,sp,#0x8
  0018720c: mov r4,r1
  0018720e: mov r5,r0
  00187210: blx 0x00073954
  00187214: ldr r1,[r5,#0x0]
  00187216: movs r2,#0x0
  00187218: ldr r3,[r1,#0x0]
  0018721a: b 0x00187228
  0018721c: ldr r5,[r0,#0x4]
  0018721e: ldr.w r5,[r5,r2,lsl #0x2]
  00187222: cmp r5,r4
  00187224: beq 0x00187230
  00187226: adds r2,#0x1
  00187228: cmp r2,r3
  0018722a: bcc 0x0018721c
  0018722c: movs r0,#0x0
  0018722e: pop {r4,r5,r7,pc}
  00187230: ldr r0,[r1,#0x4]
  00187232: ldr.w r0,[r0,r2,lsl #0x2]
  00187236: pop {r4,r5,r7,pc}
```
