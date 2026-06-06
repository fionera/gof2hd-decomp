# Status::~Status
elf 0xa8d96 body 130
Sig: undefined __thiscall ~Status(Status * this)

## decompile
```c

/* Status::~Status() */

Status * __thiscall Status::~Status(Status *this)

{
  void *pvVar1;
  
  if (*(Ship **)(this + 400) != (Ship *)0x0) {
    pvVar1 = (void *)Ship::~Ship(*(Ship **)(this + 400));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 400) = 0;
  if (*(int **)(this + 0x194) != (int *)0x0) {
    (**(code **)(**(int **)(this + 0x194) + 4))();
  }
  *(undefined4 *)(this + 0x194) = 0;
  if (*(Array **)(this + 0x20) != (Array *)0x0) {
    ArrayReleaseClasses<Agent*>(*(Array **)(this + 0x20));
    if (*(Array<Agent*> **)(this + 0x20) != (Array<Agent*> *)0x0) {
      pvVar1 = (void *)Array<Agent*>::~Array(*(Array<Agent*> **)(this + 0x20));
      operator_delete(pvVar1);
    }
  }
  *(undefined4 *)(this + 0x20) = 0;
  if (*(Array **)(this + 0x1a0) != (Array *)0x0) {
    ArrayReleaseClasses<Station*>(*(Array **)(this + 0x1a0));
    if (*(Array<Station*> **)(this + 0x1a0) != (Array<Station*> *)0x0) {
      pvVar1 = (void *)Array<Station*>::~Array(*(Array<Station*> **)(this + 0x1a0));
      operator_delete(pvVar1);
    }
  }
  *(undefined4 *)(this + 0x1a0) = 0;
  if (*(Array **)this != (Array *)0x0) {
    ArrayReleaseClasses<Wanted*>(*(Array **)this);
    if (*(Array<Wanted*> **)this != (Array<Wanted*> *)0x0) {
      pvVar1 = (void *)Array<Wanted*>::~Array(*(Array<Wanted*> **)this);
      operator_delete(pvVar1);
    }
  }
  *(undefined4 *)this = 0;
  AbyssEngine::String::~String((String *)(this + 0x168));
  return this;
}

```
## target disasm
```
  000b8d96: push {r4,r5,r7,lr}
  000b8d98: add r7,sp,#0x8
  000b8d9a: mov r4,r0
  000b8d9c: ldr.w r0,[r0,#0x190]
  000b8da0: cbz r0,0x000b8daa
  000b8da2: blx 0x00073468
  000b8da6: blx 0x0006eb48
  000b8daa: ldr.w r0,[r4,#0x194]
  000b8dae: movs r5,#0x0
  000b8db0: str.w r5,[r4,#0x190]
  000b8db4: cbz r0,0x000b8dbc
  000b8db6: ldr r1,[r0,#0x0]
  000b8db8: ldr r1,[r1,#0x4]
  000b8dba: blx r1
  000b8dbc: ldr r0,[r4,#0x20]
  000b8dbe: str.w r5,[r4,#0x194]
  000b8dc2: cbz r0,0x000b8dd4
  000b8dc4: blx 0x000734d4
  000b8dc8: ldr r0,[r4,#0x20]
  000b8dca: cbz r0,0x000b8dd4
  000b8dcc: blx 0x0007345c
  000b8dd0: blx 0x0006eb48
  000b8dd4: ldr.w r0,[r4,#0x1a0]
  000b8dd8: movs r5,#0x0
  000b8dda: str r5,[r4,#0x20]
  000b8ddc: cbz r0,0x000b8df0
  000b8dde: blx 0x000735dc
  000b8de2: ldr.w r0,[r4,#0x1a0]
  000b8de6: cbz r0,0x000b8df0
  000b8de8: blx 0x000735e8
  000b8dec: blx 0x0006eb48
  000b8df0: ldr r0,[r4,#0x0]
  000b8df2: str.w r5,[r4,#0x1a0]
  000b8df6: cbz r0,0x000b8e08
  000b8df8: blx 0x000735f4
  000b8dfc: ldr r0,[r4,#0x0]
  000b8dfe: cbz r0,0x000b8e08
  000b8e00: blx 0x00073600
  000b8e04: blx 0x0006eb48
  000b8e08: movs r0,#0x0
  000b8e0a: str r0,[r4,#0x0]
  000b8e0c: add.w r0,r4,#0x168
  000b8e10: blx 0x0006ee30
  000b8e14: mov r0,r4
  000b8e16: pop {r4,r5,r7,pc}
```
