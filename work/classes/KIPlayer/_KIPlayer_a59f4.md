# KIPlayer::~KIPlayer
elf 0xa59f4 body 168
Sig: undefined __thiscall ~KIPlayer(KIPlayer * this)

## decompile
```c

/* KIPlayer::~KIPlayer() */

KIPlayer * __thiscall KIPlayer::~KIPlayer(KIPlayer *this)

{
  void *pvVar1;
  
  *(int *)this = *(int *)(DAT_000b5aac + 0xb5a00) + 8;
  if (*(Player **)(this + 4) != (Player *)0x0) {
    pvVar1 = (void *)Player::~Player(*(Player **)(this + 4));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 4) = 0;
  if (*(Route **)(this + 0x6c) != (Route *)0x0) {
    pvVar1 = (void *)Route::~Route(*(Route **)(this + 0x6c));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x6c) = 0;
  if (*(AEGeometry **)(this + 0x78) != (AEGeometry *)0x0) {
    pvVar1 = (void *)AEGeometry::~AEGeometry(*(AEGeometry **)(this + 0x78));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x78) = 0;
  if (*(Array<unsigned_int> **)(this + 0xc4) != (Array<unsigned_int> *)0x0) {
    pvVar1 = (void *)Array<unsigned_int>::~Array(*(Array<unsigned_int> **)(this + 0xc4));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0xc4) = 0;
  if (*(AEGeometry **)(this + 8) != (AEGeometry *)0x0) {
    pvVar1 = (void *)AEGeometry::~AEGeometry(*(AEGeometry **)(this + 8));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 8) = 0;
  if (*(AEGeometry **)(this + 0xc) != (AEGeometry *)0x0) {
    pvVar1 = (void *)AEGeometry::~AEGeometry(*(AEGeometry **)(this + 0xc));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0xc) = 0;
  if (*(Array<int> **)(this + 0x50) != (Array<int> *)0x0) {
    pvVar1 = (void *)Array<int>::~Array(*(Array<int> **)(this + 0x50));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x50) = 0;
  if (*(Array **)(this + 0xcc) != (Array *)0x0) {
    ArrayReleaseClasses<SpacePoint*>(*(Array **)(this + 0xcc));
    if (*(Array<SpacePoint*> **)(this + 0xcc) != (Array<SpacePoint*> *)0x0) {
      pvVar1 = (void *)Array<SpacePoint*>::~Array(*(Array<SpacePoint*> **)(this + 0xcc));
      operator_delete(pvVar1);
    }
    *(undefined4 *)(this + 0xcc) = 0;
  }
  AbyssEngine::String::~String((String *)(this + 0x18));
  return this;
}

```

## target disasm
```
  000b59f4: push {r4,r5,r7,lr}
  000b59f6: add r7,sp,#0x8
  000b59f8: mov r4,r0
  000b59fa: ldr r0,[0x000b5aac]
  000b59fc: add r0,pc
  000b59fe: ldr r1,[r0,#0x0]
  000b5a00: ldr r0,[r4,#0x4]
  000b5a02: adds r1,#0x8
  000b5a04: str r1,[r4,#0x0]
  000b5a06: cbz r0,0x000b5a10
  000b5a08: blx 0x00072304
  000b5a0c: blx 0x0006eb48
  000b5a10: ldr r0,[r4,#0x6c]
  000b5a12: movs r5,#0x0
  000b5a14: str r5,[r4,#0x4]
  000b5a16: cbz r0,0x000b5a20
  000b5a18: blx 0x00072310
  000b5a1c: blx 0x0006eb48
  000b5a20: ldr r0,[r4,#0x78]
  000b5a22: str r5,[r4,#0x6c]
  000b5a24: cbz r0,0x000b5a2e
  000b5a26: blx 0x00071fc8
  000b5a2a: blx 0x0006eb48
  000b5a2e: ldr.w r0,[r4,#0xc4]
  000b5a32: movs r5,#0x0
  000b5a34: str r5,[r4,#0x78]
  000b5a36: cbz r0,0x000b5a40
  000b5a38: blx 0x0006f904
  000b5a3c: blx 0x0006eb48
  000b5a40: ldr r0,[r4,#0x8]
  000b5a42: str.w r5,[r4,#0xc4]
  000b5a46: cbz r0,0x000b5a50
  000b5a48: blx 0x00071fc8
  000b5a4c: blx 0x0006eb48
  000b5a50: ldr r0,[r4,#0xc]
  000b5a52: movs r5,#0x0
  000b5a54: str r5,[r4,#0x8]
  000b5a56: cbz r0,0x000b5a60
  000b5a58: blx 0x00071fc8
  000b5a5c: blx 0x0006eb48
  000b5a60: ldr r0,[r4,#0x50]
  000b5a62: str r5,[r4,#0xc]
  000b5a64: cbz r0,0x000b5a6e
  000b5a66: blx 0x00070204
  000b5a6a: blx 0x0006eb48
  000b5a6e: ldr.w r0,[r4,#0xcc]
  000b5a72: movs r1,#0x0
  000b5a74: str r1,[r4,#0x50]
  000b5a76: cbz r0,0x000b5a90
  000b5a78: blx 0x00073294
  000b5a7c: ldr.w r0,[r4,#0xcc]
  000b5a80: cbz r0,0x000b5a8a
  000b5a82: blx 0x000732a0
  000b5a86: blx 0x0006eb48
  000b5a8a: movs r0,#0x0
  000b5a8c: str.w r0,[r4,#0xcc]
  000b5a90: add.w r0,r4,#0x18
  000b5a94: blx 0x0006ee30
  000b5a98: mov r0,r4
  000b5a9a: pop {r4,r5,r7,pc}
```
