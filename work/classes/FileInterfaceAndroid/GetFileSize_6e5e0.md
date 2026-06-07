# FileInterfaceAndroid::GetFileSize
elf 0x6e5e0 body 38
Sig: undefined __thiscall GetFileSize(FileInterfaceAndroid * this)

## decompile
```c

/* FileInterfaceAndroid::GetFileSize() */

undefined4 __thiscall FileInterfaceAndroid::GetFileSize(FileInterfaceAndroid *this)

{
  undefined4 uVar1;
  
  fseek(*(undefined4 *)(this + 8),0,2);
  uVar1 = ftell(*(undefined4 *)(this + 8));
  fseek(*(undefined4 *)(this + 8),0,0);
  return uVar1;
}

```

## target disasm
```
  0007e5e0: push {r4,r5,r7,lr}
  0007e5e2: add r7,sp,#0x8
  0007e5e4: mov r4,r0
  0007e5e6: ldr r0,[r0,#0x8]
  0007e5e8: movs r1,#0x0
  0007e5ea: movs r2,#0x2
  0007e5ec: blx 0x0006f004
  0007e5f0: ldr r0,[r4,#0x8]
  0007e5f2: blx 0x0006f010
  0007e5f6: mov r5,r0
  0007e5f8: ldr r0,[r4,#0x8]
  0007e5fa: movs r1,#0x0
  0007e5fc: movs r2,#0x0
  0007e5fe: blx 0x0006f004
  0007e602: mov r0,r5
  0007e604: pop {r4,r5,r7,pc}
```
