# BluePrint::getQuantityList
elf 0x177190 body 22
Sig: undefined __thiscall getQuantityList(BluePrint * this)

## decompile
```c

/* BluePrint::getQuantityList() */

void __thiscall BluePrint::getQuantityList(BluePrint *this)

{
  (*(code *)(DAT_001ac794 + 0x1ac798))
            (*(undefined4 *)
              (*(int *)(**(int **)(DAT_001871a4 + 0x187198) + 4) + *(int *)(this + 0x20) * 4));
  return;
}

```

## target disasm
```
  00187190: ldr r1,[0x001871a4]
  00187192: ldr r0,[r0,#0x20]
  00187194: add r1,pc
  00187196: ldr r1,[r1,#0x0]
  00187198: ldr r1,[r1,#0x0]
  0018719a: ldr r1,[r1,#0x4]
  0018719c: ldr.w r0,[r1,r0,lsl #0x2]
  001871a0: b.w 0x001ac788
  001ac788: bx pc
```
