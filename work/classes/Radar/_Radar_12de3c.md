# Radar::~Radar
elf 0x12de3c body 34
Sig: undefined __thiscall ~Radar(Radar * this)

## decompile
```c

/* Radar::~Radar() */

Radar * __thiscall Radar::~Radar(Radar *this)

{
  void *pvVar1;
  
  if (*(Array<KIPlayer*> **)(this + 0x34) != (Array<KIPlayer*> *)0x0) {
    pvVar1 = (void *)Array<KIPlayer*>::~Array(*(Array<KIPlayer*> **)(this + 0x34));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x34) = 0;
  AbyssEngine::String::~String((String *)(this + 0x18c));
  return this;
}

```

## target disasm
```
  0013de3c: push {r4,r6,r7,lr}
  0013de3e: add r7,sp,#0x8
  0013de40: mov r4,r0
  0013de42: ldr r0,[r0,#0x34]
  0013de44: cbz r0,0x0013de4e
  0013de46: blx 0x00073b94
  0013de4a: blx 0x0006eb48
  0013de4e: movs r0,#0x0
  0013de50: str r0,[r4,#0x34]
  0013de52: add.w r0,r4,#0x18c
  0013de56: blx 0x0006ee30
  0013de5a: mov r0,r4
  0013de5c: pop {r4,r6,r7,pc}
```
