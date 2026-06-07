# FileInterfaceAndroid::FileInterfaceAndroid
elf 0x6ddf4 body 28
Sig: undefined __thiscall FileInterfaceAndroid(FileInterfaceAndroid * this)

## decompile
```c

/* FileInterfaceAndroid::FileInterfaceAndroid() */

void __thiscall FileInterfaceAndroid::FileInterfaceAndroid(FileInterfaceAndroid *this)

{
  int iVar1;
  
  iVar1 = DAT_0007de10;
  this[4] = (FileInterfaceAndroid)0x1;
  *(undefined4 *)(this + 0x30) = 0;
  *(undefined4 *)(this + 0x34) = 0;
  this[0x24] = (FileInterfaceAndroid)0x0;
  iVar1 = *(int *)(iVar1 + 0x7de00);
  *(undefined4 *)(this + 0x28) = 0;
  *(int *)this = iVar1 + 8;
  return;
}

```

## target disasm
```
  0007ddf4: ldr r1,[0x0007de10]
  0007ddf6: movs r2,#0x1
  0007ddf8: strb r2,[r0,#0x4]
  0007ddfa: movs r2,#0x0
  0007ddfc: add r1,pc
  0007ddfe: strd r2,r2,[r0,#0x30]
  0007de02: strb.w r2,[r0,#0x24]
  0007de06: ldr r1,[r1,#0x0]
  0007de08: str r2,[r0,#0x28]
  0007de0a: adds r1,#0x8
  0007de0c: str r1,[r0,#0x0]
  0007de0e: bx lr
```
