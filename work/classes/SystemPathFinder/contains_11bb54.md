# SystemPathFinder::contains
elf 0x11bb54 body 32
Sig: undefined __thiscall contains(SystemPathFinder * this, Array * param_1, Node * param_2)

## decompile
```c

/* SystemPathFinder::contains(Array<Node*>*, Node*) */

undefined4 __thiscall
SystemPathFinder::contains(SystemPathFinder *this,Array *param_1,Node *param_2)

{
  int iVar1;
  uint uVar2;
  
  uVar2 = 0;
  do {
    if (*(uint *)param_1 <= uVar2) {
      return 0;
    }
    iVar1 = uVar2 * 4;
    uVar2 = uVar2 + 1;
  } while (*(Node **)(*(int *)(param_1 + 4) + iVar1) != param_2);
  return 1;
}

```

## target disasm
```
  0012bb54: ldr.w r12,[r1,#0x0]
  0012bb58: movs r3,#0x0
  0012bb5a: cmp r3,r12
  0012bb5c: itt cs
  0012bb5e: mov.cs r0,#0x0
  0012bb60: bx.cs lr
  0012bb62: ldr r0,[r1,#0x4]
  0012bb64: ldr.w r0,[r0,r3,lsl #0x2]
  0012bb68: adds r3,#0x1
  0012bb6a: cmp r0,r2
  0012bb6c: itt eq
  0012bb6e: mov.eq r0,#0x1
  0012bb70: bx.eq lr
  0012bb72: b 0x0012bb5a
```
