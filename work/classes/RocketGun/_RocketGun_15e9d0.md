# RocketGun::~RocketGun
elf 0x15e9d0 body 94
Sig: undefined __thiscall ~RocketGun(RocketGun * this)

## decompile
```c

/* RocketGun::~RocketGun() */

void __thiscall RocketGun::~RocketGun(RocketGun *this)

{
  void *pvVar1;
  
  *(int *)this = *(int *)(DAT_0016ea24 + 0x16e9dc) + 8;
  if (*(Array<AbyssEngine::AEMath::Matrix> **)(this + 0xd8) !=
      (Array<AbyssEngine::AEMath::Matrix> *)0x0) {
    pvVar1 = (void *)Array<AbyssEngine::AEMath::Matrix>::~Array
                               (*(Array<AbyssEngine::AEMath::Matrix> **)(this + 0xd8));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0xd8) = 0;
  if (*(Array<int> **)(this + 0xdc) != (Array<int> *)0x0) {
    pvVar1 = (void *)Array<int>::~Array(*(Array<int> **)(this + 0xdc));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0xdc) = 0;
  if (*(Array<int> **)(this + 0xe0) != (Array<int> *)0x0) {
    pvVar1 = (void *)Array<int>::~Array(*(Array<int> **)(this + 0xe0));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0xe0) = 0;
  (*(code *)(DAT_001ac594 + 0x1ac598))(this);
  return;
}

```

## target disasm
```
  0016e9d0: push {r4,r5,r7,lr}
  0016e9d2: add r7,sp,#0x8
  0016e9d4: mov r4,r0
  0016e9d6: ldr r0,[0x0016ea24]
  0016e9d8: add r0,pc
  0016e9da: ldr r1,[r0,#0x0]
  0016e9dc: ldr.w r0,[r4,#0xd8]
  0016e9e0: adds r1,#0x8
  0016e9e2: str r1,[r4,#0x0]
  0016e9e4: cbz r0,0x0016e9ee
  0016e9e6: blx 0x0006f8f8
  0016e9ea: blx 0x0006eb48
  0016e9ee: ldr.w r0,[r4,#0xdc]
  0016e9f2: movs r5,#0x0
  0016e9f4: str.w r5,[r4,#0xd8]
  0016e9f8: cbz r0,0x0016ea02
  0016e9fa: blx 0x00070204
  0016e9fe: blx 0x0006eb48
  0016ea02: ldr.w r0,[r4,#0xe0]
  0016ea06: str.w r5,[r4,#0xdc]
  0016ea0a: cbz r0,0x0016ea14
  0016ea0c: blx 0x00070204
  0016ea10: blx 0x0006eb48
  0016ea14: movs r0,#0x0
  0016ea16: str.w r0,[r4,#0xe0]
  0016ea1a: mov r0,r4
  0016ea1c: pop.w {r4,r5,r7,lr}
  0016ea20: b.w 0x001ac588
  001ac588: bx pc
  001ac58c: ldr r12,[0x1ac594]
  001ac590: add pc,r12,pc
```
