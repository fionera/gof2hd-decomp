# FileInterfaceAndroid::GetDirPreFix
elf 0x6e80c body 16
Sig: undefined __thiscall GetDirPreFix(FileInterfaceAndroid * this)

## decompile
```c

/* FileInterfaceAndroid::GetDirPreFix() */

void __thiscall FileInterfaceAndroid::GetDirPreFix(FileInterfaceAndroid *this)

{
  AbyssEngine::String::String((String *)this,(char *)(DAT_0007e81c + 0x7e818),false);
  return;
}

```

## target disasm
```
  0007e80c: push {r7,lr}
  0007e80e: mov r7,sp
  0007e810: ldr r1,[0x0007e81c]
  0007e812: movs r2,#0x0
  0007e814: add r1,pc
  0007e816: blx 0x0006ee18
  0007e81a: pop {r7,pc}
```
