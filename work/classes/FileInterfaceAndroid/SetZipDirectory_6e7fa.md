# FileInterfaceAndroid::SetZipDirectory
elf 0x6e7fa body 6
Sig: undefined __thiscall SetZipDirectory(FileInterfaceAndroid * this, void * param_1)

## decompile
```c

/* FileInterfaceAndroid::SetZipDirectory(void*) */

void __thiscall FileInterfaceAndroid::SetZipDirectory(FileInterfaceAndroid *this,void *param_1)

{
  if (param_1 != (void *)0x0) {
    *(void **)(this + 0x34) = param_1;
  }
  return;
}

```

## target disasm
```
  0007e7fa: cbz r1,0x0007e7fe
  0007e7fc: str r1,[r0,#0x34]
  0007e7fe: bx lr
```
