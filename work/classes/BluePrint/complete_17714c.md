# BluePrint::complete
elf 0x17714c body 24
Sig: undefined __thiscall complete(BluePrint * this)

## decompile
```c

/* BluePrint::complete() */

void __thiscall BluePrint::complete(BluePrint *this)

{
  uint *puVar1;
  uint uVar2;
  
  puVar1 = *(uint **)this;
  for (uVar2 = 0; uVar2 < *puVar1; uVar2 = uVar2 + 1) {
    *(undefined4 *)(puVar1[1] + uVar2 * 4) = 0;
  }
  return;
}

```

## target disasm
```
  0018714c: ldr r0,[r0,#0x0]
  0018714e: movs r1,#0x0
  00187150: movs r2,#0x0
  00187152: ldr r3,[r0,#0x0]
  00187154: cmp r2,r3
  00187156: it cs
  00187158: bx.cs lr
  0018715a: ldr r3,[r0,#0x4]
  0018715c: str.w r1,[r3,r2,lsl #0x2]
  00187160: adds r2,#0x1
  00187162: b 0x00187152
```
