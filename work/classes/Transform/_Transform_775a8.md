# Transform::~Transform
elf 0x775a8 body 64
Sig: undefined __thiscall ~Transform(Transform * this)

## decompile
```c

/* AbyssEngine::Transform::~Transform() */

Transform * __thiscall AbyssEngine::Transform::~Transform(Transform *this)

{
  if (this[0x17d] == (Transform)0x0) {
    ArrayReleaseClasses<AbyssEngine::KeyFrame*>((Array *)(this + 0x11c));
  }
  Quaternion::~Quaternion((Quaternion *)(this + 0x150));
  Quaternion::~Quaternion((Quaternion *)(this + 0x128));
  Array<AbyssEngine::KeyFrame*>::~Array((Array<AbyssEngine::KeyFrame*> *)(this + 0x11c));
  Array<AbyssEngine::Transform*>::~Array((Array<AbyssEngine::Transform*> *)(this + 0x4c));
  Array<AbyssEngine::Mesh*>::~Array((Array<AbyssEngine::Mesh*> *)(this + 0x3c));
  return this;
}

```

## target disasm
```
  000875a8: push {r4,r5,r7,lr}
  000875aa: add r7,sp,#0x8
  000875ac: mov r4,r0
  000875ae: ldrb.w r0,[r0,#0x17d]
  000875b2: add.w r5,r4,#0x11c
  000875b6: cbnz r0,0x000875be
  000875b8: mov r0,r5
  000875ba: blx 0x0006f880
  000875be: add.w r0,r4,#0x150
  000875c2: blx 0x0006f13c
  000875c6: add.w r0,r4,#0x128
  000875ca: blx 0x0006f13c
  000875ce: mov r0,r5
  000875d0: blx 0x0006f748
  000875d4: add.w r0,r4,#0x4c
  000875d8: blx 0x0006f754
  000875dc: add.w r0,r4,#0x3c
  000875e0: blx 0x0006f760
  000875e4: mov r0,r4
  000875e6: pop {r4,r5,r7,pc}
```
