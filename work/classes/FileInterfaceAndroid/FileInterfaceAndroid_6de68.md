# FileInterfaceAndroid::FileInterfaceAndroid
elf 0x6de68 body 42
Sig: undefined __thiscall FileInterfaceAndroid(FileInterfaceAndroid * this, __sFILE * param_1, bool param_2)

## decompile
```c

/* FileInterfaceAndroid::FileInterfaceAndroid(__sFILE*, bool) */

void __thiscall
FileInterfaceAndroid::FileInterfaceAndroid(FileInterfaceAndroid *this,__sFILE *param_1,bool param_2)

{
  int iVar1;
  undefined4 *puVar2;
  int *piVar3;
  
  iVar1 = DAT_0007de98;
  puVar2 = (undefined4 *)(DAT_0007de94 + 0x7de78);
  *(__sFILE **)(this + 8) = param_1;
  *(undefined4 *)(this + 0xc) = 0;
  *(undefined4 *)(this + 0x10) = 0;
  piVar3 = (int *)*puVar2;
  iVar1 = *(int *)(iVar1 + 0x7de7e);
  this[0x14] = (FileInterfaceAndroid)param_2;
  *(int *)this = iVar1 + 8;
  *piVar3 = *piVar3 + 1;
  return;
}

```

## target disasm
```
  0007de68: push {r4,r6,r7,lr}
  0007de6a: add r7,sp,#0x8
  0007de6c: ldr r3,[0x0007de94]
  0007de6e: mov.w r12,#0x0
  0007de72: ldr r4,[0x0007de98]
  0007de74: add r3,pc
  0007de76: strd r1,r12,[r0,#0x8]
  0007de7a: add r4,pc
  0007de7c: str.w r12,[r0,#0x10]
  0007de80: ldr r3,[r3,#0x0]
  0007de82: ldr r1,[r4,#0x0]
  0007de84: strb r2,[r0,#0x14]
  0007de86: adds r1,#0x8
  0007de88: str r1,[r0,#0x0]
  0007de8a: ldr r1,[r3,#0x0]
  0007de8c: adds r1,#0x1
  0007de8e: str r1,[r3,#0x0]
  0007de90: pop {r4,r6,r7,pc}
```
