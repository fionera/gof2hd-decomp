# FileRead::loadStation
elf 0x11f948 body 38
Sig: undefined __stdcall loadStation(int param_1)

## decompile
```c

/* FileRead::loadStation(int) */

undefined4 FileRead::loadStation(int param_1)

{
  FileRead *this;
  int iVar1;
  undefined2 in_r1;
  undefined4 uVar2;
  
  this = operator_new__(2);
  *(undefined2 *)this = in_r1;
  iVar1 = loadStationsBinary(this,(short *)this,1);
  uVar2 = **(undefined4 **)(iVar1 + 4);
  operator_delete__(this);
  return uVar2;
}

```

## target disasm
```
  0012f948: push {r4,r5,r7,lr}
  0012f94a: add r7,sp,#0x8
  0012f94c: movs r0,#0x2
  0012f94e: mov r4,r1
  0012f950: blx 0x0006ec08
  0012f954: mov r1,r0
  0012f956: movs r2,#0x1
  0012f958: mov r5,r0
  0012f95a: strh r4,[r0,#0x0]
  0012f95c: blx 0x000762e8
  0012f960: ldr r0,[r0,#0x4]
  0012f962: ldr r4,[r0,#0x0]
  0012f964: mov r0,r5
  0012f966: blx 0x0006ebfc
  0012f96a: mov r0,r4
  0012f96c: pop {r4,r5,r7,pc}
```
