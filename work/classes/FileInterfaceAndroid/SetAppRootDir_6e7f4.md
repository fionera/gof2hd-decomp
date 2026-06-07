# FileInterfaceAndroid::SetAppRootDir
elf 0x6e7f4 body 6
Sig: undefined __thiscall SetAppRootDir(FileInterfaceAndroid * this, void * param_1)

## decompile
```c

/* FileInterfaceAndroid::SetAppRootDir(void*) */

void __thiscall FileInterfaceAndroid::SetAppRootDir(FileInterfaceAndroid *this,void *param_1)

{
  if (param_1 != (void *)0x0) {
    *(void **)(this + 0x30) = param_1;
  }
  return;
}

```

## target disasm
```
  0007e7f4: cbz r1,0x0007e7f8
  0007e7f6: str r1,[r0,#0x30]
  0007e7f8: bx lr
```
