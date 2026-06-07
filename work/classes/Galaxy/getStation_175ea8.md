# Galaxy::getStation
elf 0x175ea8 body 58
Sig: undefined __stdcall getStation(int param_1)

## decompile
```c

/* Galaxy::getStation(int) */

undefined4 Galaxy::getStation(int param_1)

{
  FileRead *this;
  undefined4 uVar1;
  void *pvVar2;
  int in_r1;
  
  if (-1 < in_r1) {
    this = operator_new(1);
    FileRead::FileRead(this);
    uVar1 = FileRead::loadStation((int)this);
    pvVar2 = (void *)FileRead::~FileRead(this);
    operator_delete(pvVar2);
    return uVar1;
  }
  return *(undefined4 *)(**(int **)(DAT_00185ef0 + 0x185edc) + 0x78);
}

```

## target disasm
```
  00185ea8: push {r4,r5,r7,lr}
  00185eaa: add r7,sp,#0x8
  00185eac: mov r5,r1
  00185eae: cmp r1,#0x0
  00185eb0: blt 0x00185ed6
  00185eb2: movs r0,#0x1
  00185eb4: blx 0x0006eb24
  00185eb8: mov r4,r0
  00185eba: blx 0x00072124
  00185ebe: mov r0,r4
  00185ec0: mov r1,r5
  00185ec2: blx 0x000737a4
  00185ec6: mov r5,r0
  00185ec8: mov r0,r4
  00185eca: blx 0x0007213c
  00185ece: blx 0x0006eb48
  00185ed2: mov r0,r5
  00185ed4: pop {r4,r5,r7,pc}
  00185ed6: ldr r0,[0x00185ef0]
  00185ed8: add r0,pc
  00185eda: ldr r0,[r0,#0x0]
  00185edc: ldr r0,[r0,#0x0]
  00185ede: ldr r0,[r0,#0x78]
  00185ee0: pop {r4,r5,r7,pc}
```
