# StarMap::setStart
elf 0xc7dd8 body 52
Sig: undefined __thiscall setStart(StarMap * this, int param_1, int param_2)

## decompile
```c

/* StarMap::setStart(int, int) */

void __thiscall StarMap::setStart(StarMap *this,int param_1,int param_2)

{
  void *pvVar1;
  undefined4 uVar2;
  
  *(int *)(this + 0x1dc) = param_2;
  *(int *)(this + 0x1e0) = param_1;
  if (*(Array<int> **)(this + 0xa0) != (Array<int> *)0x0) {
    pvVar1 = (void *)Array<int>::~Array(*(Array<int> **)(this + 0xa0));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0xa0) = 0;
  uVar2 = SystemPathFinder::getSystemPath
                    (*(SystemPathFinder **)(this + 0x50),*(Array **)(this + 0x54),param_1,
                     *(int *)(this + 0x104));
  *(undefined4 *)(this + 0xa0) = uVar2;
  return;
}

```

## target disasm
```
  000d7dd8: push {r4,r5,r7,lr}
  000d7dda: add r7,sp,#0x8
  000d7ddc: mov r4,r0
  000d7dde: ldr.w r0,[r0,#0xa0]
  000d7de2: mov r5,r1
  000d7de4: strd r2,r1,[r4,#0x1dc]
  000d7de8: cbz r0,0x000d7df2
  000d7dea: blx 0x00070204
  000d7dee: blx 0x0006eb48
  000d7df2: ldrd r0,r1,[r4,#0x50]
  000d7df6: movs r2,#0x0
  000d7df8: ldr.w r3,[r4,#0x104]
  000d7dfc: str.w r2,[r4,#0xa0]
  000d7e00: mov r2,r5
  000d7e02: blx 0x000737e0
  000d7e06: str.w r0,[r4,#0xa0]
  000d7e0a: pop {r4,r5,r7,pc}
```
