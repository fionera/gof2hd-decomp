# BluePrint::getIngredientList
elf 0x177134 body 22
Sig: undefined __thiscall getIngredientList(BluePrint * this)

## decompile
```c

/* BluePrint::getIngredientList() */

void __thiscall BluePrint::getIngredientList(BluePrint *this)

{
  (*(code *)(DAT_001ac784 + 0x1ac788))
            (*(undefined4 *)
              (*(int *)(**(int **)(DAT_00187148 + 0x18713c) + 4) + *(int *)(this + 0x20) * 4));
  return;
}

```

## target disasm
```
  00187134: ldr r1,[0x00187148]
  00187136: ldr r0,[r0,#0x20]
  00187138: add r1,pc
  0018713a: ldr r1,[r1,#0x0]
  0018713c: ldr r1,[r1,#0x0]
  0018713e: ldr r1,[r1,#0x4]
  00187140: ldr.w r0,[r1,r0,lsl #0x2]
  00187144: b.w 0x001ac778
  001ac778: bx pc
```
