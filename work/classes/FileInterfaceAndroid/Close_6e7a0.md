# FileInterfaceAndroid::Close
elf 0x6e7a0 body 72
Sig: undefined __thiscall Close(FileInterfaceAndroid * this)

## decompile
```c

/* FileInterfaceAndroid::Close() */

void __thiscall FileInterfaceAndroid::Close(FileInterfaceAndroid *this)

{
  undefined4 *puVar1;
  
  if (*(int *)(this + 8) != 0) {
    fclose();
    *(undefined4 *)(this + 8) = 0;
  }
  if (*(int *)(this + 0xc) != 0) {
    zip_fclose();
    *(undefined4 *)(this + 0xc) = 0;
  }
  if (*(_jmethodID **)(this + 0x10) != (_jmethodID *)0x0) {
    if (this[0x14] == (FileInterfaceAndroid)0x0) {
      puVar1 = (undefined4 *)(DAT_0007e7f0 + 0x7e7dc);
    }
    else {
      puVar1 = (undefined4 *)(DAT_0007e7ec + 0x7e7d6);
    }
    _JNIEnv::CallVoidMethod
              ((_jobject *)**(undefined4 **)(DAT_0007e7e8 + 0x7e7ca),*(_jmethodID **)(this + 0x10),
               *(undefined4 *)*puVar1);
    *(undefined4 *)(this + 0x10) = 0;
  }
  return;
}

```

## target disasm
```
  0007e7a0: push {r4,r6,r7,lr}
  0007e7a2: add r7,sp,#0x8
  0007e7a4: mov r4,r0
  0007e7a6: ldr r0,[r0,#0x8]
  0007e7a8: cbz r0,0x0007e7b2
  0007e7aa: blx 0x0006e980
  0007e7ae: movs r0,#0x0
  0007e7b0: str r0,[r4,#0x8]
  0007e7b2: ldr r0,[r4,#0xc]
  0007e7b4: cbz r0,0x0007e7be
  0007e7b6: blx 0x0006f01c
  0007e7ba: movs r0,#0x0
  0007e7bc: str r0,[r4,#0xc]
  0007e7be: ldr r1,[r4,#0x10]
  0007e7c0: cbz r1,0x0007e7e6
  0007e7c2: ldr r0,[0x0007e7e8]
  0007e7c4: ldrb r2,[r4,#0x14]
  0007e7c6: add r0,pc
  0007e7c8: cmp r2,#0x0
  0007e7ca: ldr r0,[r0,#0x0]
  0007e7cc: ldr r0,[r0,#0x0]
  0007e7ce: beq 0x0007e7d6
  0007e7d0: ldr r2,[0x0007e7ec]
  0007e7d2: add r2,pc
  0007e7d4: b 0x0007e7da
  0007e7d6: ldr r2,[0x0007e7f0]
  0007e7d8: add r2,pc
  0007e7da: ldr r2,[r2,#0x0]
  0007e7dc: ldr r2,[r2,#0x0]
  0007e7de: blx 0x0006efec
  0007e7e2: movs r0,#0x0
  0007e7e4: str r0,[r4,#0x10]
  0007e7e6: pop {r4,r6,r7,pc}
```
