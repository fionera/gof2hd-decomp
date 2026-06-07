# LODManager::~LODManager
elf 0x951dc body 26
Sig: undefined __thiscall ~LODManager(LODManager * this)

## decompile
```c

/* LODManager::~LODManager() */

LODManager * __thiscall LODManager::~LODManager(LODManager *this)

{
  void *pvVar1;
  
  if (*(Array<AEGeometry*> **)this != (Array<AEGeometry*> *)0x0) {
    pvVar1 = (void *)Array<AEGeometry*>::~Array(*(Array<AEGeometry*> **)this);
    operator_delete(pvVar1);
  }
  *(undefined4 *)this = 0;
  return this;
}

```

## target disasm
```
  000a51dc: push {r4,r6,r7,lr}
  000a51de: add r7,sp,#0x8
  000a51e0: mov r4,r0
  000a51e2: ldr r0,[r0,#0x0]
  000a51e4: cbz r0,0x000a51ee
  000a51e6: blx 0x000717c4
  000a51ea: blx 0x0006eb48
  000a51ee: movs r0,#0x0
  000a51f0: str r0,[r4,#0x0]
  000a51f2: mov r0,r4
  000a51f4: pop {r4,r6,r7,pc}
```
