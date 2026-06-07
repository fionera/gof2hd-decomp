# Route::~Route
elf 0x11bcf0 body 64
Sig: undefined __thiscall ~Route(Route * this)

## decompile
```c

/* Route::~Route() */

Route * __thiscall Route::~Route(Route *this)

{
  void *pvVar1;
  
  if (*(Array **)(this + 0xc) != (Array *)0x0) {
    ArrayReleaseClasses<Waypoint*>(*(Array **)(this + 0xc));
    if (*(Array<Waypoint*> **)(this + 0xc) != (Array<Waypoint*> *)0x0) {
      pvVar1 = (void *)Array<Waypoint*>::~Array(*(Array<Waypoint*> **)(this + 0xc));
      operator_delete(pvVar1);
    }
  }
  *(undefined4 *)(this + 0xc) = 0;
  if (*(Array<KIPlayer*> **)(this + 0x10) != (Array<KIPlayer*> *)0x0) {
    pvVar1 = (void *)Array<KIPlayer*>::~Array(*(Array<KIPlayer*> **)(this + 0x10));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x10) = 0;
  if (*(Array<int> **)(this + 0x14) != (Array<int> *)0x0) {
    pvVar1 = (void *)Array<int>::~Array(*(Array<int> **)(this + 0x14));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x14) = 0;
  return this;
}

```

## target disasm
```
  0012bcf0: push {r4,r5,r7,lr}
  0012bcf2: add r7,sp,#0x8
  0012bcf4: mov r4,r0
  0012bcf6: ldr r0,[r0,#0xc]
  0012bcf8: cbz r0,0x0012bd0a
  0012bcfa: blx 0x00075f7c
  0012bcfe: ldr r0,[r4,#0xc]
  0012bd00: cbz r0,0x0012bd0a
  0012bd02: blx 0x00075f88
  0012bd06: blx 0x0006eb48
  0012bd0a: ldr r0,[r4,#0x10]
  0012bd0c: movs r5,#0x0
  0012bd0e: str r5,[r4,#0xc]
  0012bd10: cbz r0,0x0012bd1a
  0012bd12: blx 0x00073b94
  0012bd16: blx 0x0006eb48
  0012bd1a: ldr r0,[r4,#0x14]
  0012bd1c: str r5,[r4,#0x10]
  0012bd1e: cbz r0,0x0012bd28
  0012bd20: blx 0x00070204
  0012bd24: blx 0x0006eb48
  0012bd28: movs r0,#0x0
  0012bd2a: str r0,[r4,#0x14]
  0012bd2c: mov r0,r4
  0012bd2e: pop {r4,r5,r7,pc}
```
