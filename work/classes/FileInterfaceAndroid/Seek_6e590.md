# FileInterfaceAndroid::Seek
elf 0x6e590 body 80
Sig: undefined __thiscall Seek(FileInterfaceAndroid * this, uint param_1)

## decompile
```c

/* FileInterfaceAndroid::Seek(unsigned int) */

bool __thiscall FileInterfaceAndroid::Seek(FileInterfaceAndroid *this,uint param_1)

{
  int iVar1;
  int iVar2;
  
  if (param_1 == 0) {
    return true;
  }
  iVar2 = *(int *)(this + 0xc);
  if (iVar2 == 0) {
    if (*(int *)(this + 8) == 0) {
      return false;
    }
    iVar2 = fseek(*(int *)(this + 8),param_1,1);
  }
  else {
    iVar1 = malloc(param_1);
    if (iVar1 == 0) {
      return false;
    }
    iVar2 = zip_fread(iVar2,iVar1,param_1);
    free(iVar1);
    iVar2 = iVar2 - param_1;
  }
  return iVar2 == 0;
}

```

## target disasm
```
  0007e590: push {r4,r5,r6,r7,lr}
  0007e592: add r7,sp,#0xc
  0007e594: push.w r11
  0007e598: mov r4,r1
  0007e59a: cbz r1,0x0007e5c0
  0007e59c: ldr r5,[r0,#0xc]
  0007e59e: cbz r5,0x0007e5c4
  0007e5a0: mov r0,r4
  0007e5a2: blx 0x0006e9a4
  0007e5a6: mov r6,r0
  0007e5a8: cbz r0,0x0007e5d8
  0007e5aa: mov r0,r5
  0007e5ac: mov r1,r6
  0007e5ae: mov r2,r4
  0007e5b0: blx 0x0006efa4
  0007e5b4: mov r5,r0
  0007e5b6: mov r0,r6
  0007e5b8: blx 0x0006eff8
  0007e5bc: subs r0,r5,r4
  0007e5be: b 0x0007e5d0
  0007e5c0: movs r0,#0x1
  0007e5c2: b 0x0007e5da
  0007e5c4: ldr r0,[r0,#0x8]
  0007e5c6: cbz r0,0x0007e5d8
  0007e5c8: mov r1,r4
  0007e5ca: movs r2,#0x1
  0007e5cc: blx 0x0006f004
  0007e5d0: clz r0,r0
  0007e5d4: lsrs r0,r0,#0x5
  0007e5d6: b 0x0007e5da
  0007e5d8: movs r0,#0x0
  0007e5da: pop.w r11
  0007e5de: pop {r4,r5,r6,r7,pc}
```
