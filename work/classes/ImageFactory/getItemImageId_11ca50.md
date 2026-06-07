# ImageFactory::getItemImageId
elf 0x11ca50 body 16
Sig: undefined __thiscall getItemImageId(ImageFactory * this, int param_1)

## decompile
```c

/* ImageFactory::getItemImageId(int) */

int __thiscall ImageFactory::getItemImageId(ImageFactory *this,int param_1)

{
  int iVar1;
  
  iVar1 = 0xef0;
  if (param_1 < 0xb0) {
    iVar1 = 0x898;
  }
  return iVar1 + param_1;
}

```

## target disasm
```
  0012ca50: mov.w r0,#0xef0
  0012ca54: cmp r1,#0xb0
  0012ca56: it lt
  0012ca58: movw.lt r0,#0x898
  0012ca5c: add r0,r1
  0012ca5e: bx lr
```
