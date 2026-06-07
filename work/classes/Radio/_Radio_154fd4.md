# Radio::~Radio
elf 0x154fd4 body 68
Sig: undefined __thiscall ~Radio(Radio * this)

## decompile
```c

/* Radio::~Radio() */

Radio * __thiscall Radio::~Radio(Radio *this)

{
  void *pvVar1;
  
  if (*(Array **)(this + 0xc) != (Array *)0x0) {
    ArrayReleaseClasses<ImagePart*>(*(Array **)(this + 0xc));
    if (*(Array<ImagePart*> **)(this + 0xc) != (Array<ImagePart*> *)0x0) {
      pvVar1 = (void *)Array<ImagePart*>::~Array(*(Array<ImagePart*> **)(this + 0xc));
      operator_delete(pvVar1);
    }
  }
  *(undefined4 *)(this + 0xc) = 0;
  if (*(void **)(this + 0x10) != (void *)0x0) {
    operator_delete__(*(void **)(this + 0x10));
  }
  *(undefined4 *)(this + 0x10) = 0;
  if (*(Array **)(this + 8) != (Array *)0x0) {
    ArrayReleaseClasses<AbyssEngine::String*>(*(Array **)(this + 8));
    if (*(Array<AbyssEngine::String*> **)(this + 8) != (Array<AbyssEngine::String*> *)0x0) {
      pvVar1 = (void *)Array<AbyssEngine::String*>::~Array
                                 (*(Array<AbyssEngine::String*> **)(this + 8));
      operator_delete(pvVar1);
    }
  }
  *(undefined4 *)(this + 8) = 0;
  return this;
}

```

## target disasm
```
  00164fd4: push {r4,r5,r7,lr}
  00164fd6: add r7,sp,#0x8
  00164fd8: mov r4,r0
  00164fda: ldr r0,[r0,#0xc]
  00164fdc: cbz r0,0x00164fee
  00164fde: blx 0x00075ae4
  00164fe2: ldr r0,[r4,#0xc]
  00164fe4: cbz r0,0x00164fee
  00164fe6: blx 0x00075af0
  00164fea: blx 0x0006eb48
  00164fee: ldr r0,[r4,#0x10]
  00164ff0: movs r5,#0x0
  00164ff2: str r5,[r4,#0xc]
  00164ff4: cbz r0,0x00164ffa
  00164ff6: blx 0x0006ebfc
  00164ffa: ldr r0,[r4,#0x8]
  00164ffc: str r5,[r4,#0x10]
  00164ffe: cbz r0,0x00165010
  00165000: blx 0x0006facc
  00165004: ldr r0,[r4,#0x8]
  00165006: cbz r0,0x00165010
  00165008: blx 0x0006f64c
  0016500c: blx 0x0006eb48
  00165010: movs r0,#0x0
  00165012: str r0,[r4,#0x8]
  00165014: mov r0,r4
  00165016: pop {r4,r5,r7,pc}
```
