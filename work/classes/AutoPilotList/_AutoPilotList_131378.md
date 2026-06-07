# AutoPilotList::~AutoPilotList
elf 0x131378 body 32
Sig: undefined __thiscall ~AutoPilotList(AutoPilotList * this)

## decompile
```c

/* AutoPilotList::~AutoPilotList() */

AutoPilotList * __thiscall AutoPilotList::~AutoPilotList(AutoPilotList *this)

{
  void *pvVar1;
  
  ArrayReleaseClasses<AbyssEngine::String*>(*(Array **)(this + 0x10));
  if (*(Array<AbyssEngine::String*> **)(this + 0x10) != (Array<AbyssEngine::String*> *)0x0) {
    pvVar1 = (void *)Array<AbyssEngine::String*>::~Array
                               (*(Array<AbyssEngine::String*> **)(this + 0x10));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x10) = 0;
  return this;
}

```

## target disasm
```
  00141378: push {r4,r6,r7,lr}
  0014137a: add r7,sp,#0x8
  0014137c: mov r4,r0
  0014137e: ldr r0,[r0,#0x10]
  00141380: blx 0x0006facc
  00141384: ldr r0,[r4,#0x10]
  00141386: cbz r0,0x00141390
  00141388: blx 0x0006f64c
  0014138c: blx 0x0006eb48
  00141390: movs r0,#0x0
  00141392: str r0,[r4,#0x10]
  00141394: mov r0,r4
  00141396: pop {r4,r6,r7,pc}
```
