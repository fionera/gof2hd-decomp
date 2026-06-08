# ResourceMesh::~ResourceMesh
elf 0x77ed0 body 20
Sig: undefined __thiscall ~ResourceMesh(ResourceMesh * this)

## decompile
```c

/* AbyssEngine::ResourceMesh::~ResourceMesh() */

ResourceMesh * __thiscall AbyssEngine::ResourceMesh::~ResourceMesh(ResourceMesh *this)

{
  operator_delete__(*(void **)this);
  *(undefined4 *)this = 0;
  return this;
}

```

## target disasm
```
  00087ed0: push {r4,r6,r7,lr}
  00087ed2: add r7,sp,#0x8
  00087ed4: mov r4,r0
  00087ed6: ldr r0,[r0,#0x0]
  00087ed8: blx 0x0006ebfc
  00087edc: movs r0,#0x0
  00087ede: str r0,[r4,#0x0]
  00087ee0: mov r0,r4
  00087ee2: pop {r4,r6,r7,pc}
```
