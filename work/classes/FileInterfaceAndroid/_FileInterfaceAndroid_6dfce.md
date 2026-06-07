# FileInterfaceAndroid::~FileInterfaceAndroid
elf 0x6dfce body 16
Sig: undefined __thiscall ~FileInterfaceAndroid(FileInterfaceAndroid * this)

## decompile
```c

/* FileInterfaceAndroid::~FileInterfaceAndroid() */

void __thiscall FileInterfaceAndroid::~FileInterfaceAndroid(FileInterfaceAndroid *this)

{
  ~FileInterfaceAndroid(this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  0007dfce: push {r7,lr}
  0007dfd0: mov r7,sp
  0007dfd2: blx 0x0006ef38
  0007dfd6: pop.w {r7,lr}
  0007dfda: b.w 0x001ab098
```
