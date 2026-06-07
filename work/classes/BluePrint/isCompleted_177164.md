# BluePrint::isCompleted
elf 0x177164 body 32
Sig: undefined __thiscall isCompleted(BluePrint * this)

## decompile
```c

/* BluePrint::isCompleted() */

void __thiscall BluePrint::isCompleted(BluePrint *this)

{
  int iVar1;
  uint uVar2;
  
  uVar2 = 0;
  do {
    if (**(uint **)this <= uVar2) {
      return;
    }
    iVar1 = uVar2 * 4;
    uVar2 = uVar2 + 1;
  } while (*(int *)((*(uint **)this)[1] + iVar1) < 1);
  return;
}

```

## target disasm
```
  00187164: ldr r0,[r0,#0x0]
  00187166: movs r2,#0x0
  00187168: ldr r1,[r0,#0x0]
  0018716a: cmp r2,r1
  0018716c: itt cs
  0018716e: mov.cs r0,#0x1
  00187170: bx.cs lr
  00187172: ldr r3,[r0,#0x4]
  00187174: ldr.w r3,[r3,r2,lsl #0x2]
  00187178: adds r2,#0x1
  0018717a: cmp r3,#0x1
  0018717c: itt ge
  0018717e: mov.ge r0,#0x0
  00187180: bx.ge lr
  00187182: b 0x0018716a
```
