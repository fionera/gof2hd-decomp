# FileInterfaceAndroid::~FileInterfaceAndroid
elf 0x6df88 body 46
Sig: undefined __thiscall ~FileInterfaceAndroid(FileInterfaceAndroid * this)

## decompile
```c

/* FileInterfaceAndroid::~FileInterfaceAndroid() */

FileInterfaceAndroid * __thiscall
FileInterfaceAndroid::~FileInterfaceAndroid(FileInterfaceAndroid *this)

{
  int iVar1;
  
  *(int *)this = *(int *)(DAT_0007dfbc + 0x7df94) + 8;
  Close(this);
  iVar1 = **(int **)(DAT_0007dfc0 + 0x7dfa4);
  if (iVar1 == 0) {
    this[4] = (FileInterfaceAndroid)0x0;
  }
  else {
    **(int **)(DAT_0007dfc0 + 0x7dfa4) = iVar1 + -1;
  }
  return this;
}

```

## target disasm
```
  0007df88: push {r4,r6,r7,lr}
  0007df8a: add r7,sp,#0x8
  0007df8c: mov r4,r0
  0007df8e: ldr r0,[0x0007dfbc]
  0007df90: add r0,pc
  0007df92: ldr r0,[r0,#0x0]
  0007df94: adds r0,#0x8
  0007df96: str r0,[r4,#0x0]
  0007df98: mov r0,r4
  0007df9a: blx 0x0006ef14
  0007df9e: ldr r0,[0x0007dfc0]
  0007dfa0: add r0,pc
  0007dfa2: ldr r0,[r0,#0x0]
  0007dfa4: ldr r1,[r0,#0x0]
  0007dfa6: cbz r1,0x0007dfae
  0007dfa8: subs r1,#0x1
  0007dfaa: str r1,[r0,#0x0]
  0007dfac: b 0x0007dfb2
  0007dfae: movs r0,#0x0
  0007dfb0: strb r0,[r4,#0x4]
  0007dfb2: mov r0,r4
  0007dfb4: pop {r4,r6,r7,pc}
```
