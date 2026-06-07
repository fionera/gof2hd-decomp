# SystemPathFinder::getJumpDistance
elf 0x11b780 body 38
Sig: undefined __stdcall getJumpDistance(Array * param_1, int param_2, int param_3)

## decompile
```c

/* SystemPathFinder::getJumpDistance(Array<SolarSystem*>*, int, int) */

int SystemPathFinder::getJumpDistance(Array *param_1,int param_2,int param_3)

{
  Array *this;
  void *pvVar1;
  int in_r3;
  int iVar2;
  
  this = (Array *)getSystemPath((SystemPathFinder *)param_1,(Array *)param_2,param_3,in_r3);
  if (this != (Array *)0x0) {
    iVar2 = *(int *)this;
    ArrayRelease<int>(this);
    pvVar1 = (void *)Array<int>::~Array((Array<int> *)this);
    operator_delete(pvVar1);
    return iVar2 + -1;
  }
  return 0;
}

```

## target disasm
```
  0012b780: push {r4,r5,r7,lr}
  0012b782: add r7,sp,#0x8
  0012b784: blx 0x000737e0
  0012b788: mov r4,r0
  0012b78a: cbz r0,0x0012b7a2
  0012b78c: mov r0,r4
  0012b78e: ldr r5,[r4,#0x0]
  0012b790: blx 0x0007369c
  0012b794: mov r0,r4
  0012b796: blx 0x00070204
  0012b79a: blx 0x0006eb48
  0012b79e: subs r0,r5,#0x1
  0012b7a0: pop {r4,r5,r7,pc}
  0012b7a2: movs r0,#0x0
  0012b7a4: pop {r4,r5,r7,pc}
```
