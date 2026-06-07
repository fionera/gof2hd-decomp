# AEGeometry::setLodLastVisibleDistance
elf 0xa4a86 body 18
Sig: undefined __stdcall setLodLastVisibleDistance(ulonglong param_1)

## decompile
```c

/* AEGeometry::setLodLastVisibleDistance(unsigned long long) */

void AEGeometry::setLodLastVisibleDistance(ulonglong param_1)

{
  uint in_r2;
  int in_r3;
  
  *(int *)((int)param_1 + 0x70) = (int)((ulonglong)in_r2 * (ulonglong)in_r2);
  *(uint *)((int)param_1 + 0x74) =
       in_r2 * in_r3 + in_r2 * in_r3 + (int)((ulonglong)in_r2 * (ulonglong)in_r2 >> 0x20);
  return;
}

```

## target disasm
```
  000b4a86: umull r12,r1,r2,r2
  000b4a8a: mla r1,r2,r3,r1
  000b4a8e: mla r1,r2,r3,r1
  000b4a92: strd r12,r1,[r0,#0x70]
  000b4a96: bx lr
```
