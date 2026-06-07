# AEGeometry::setMesh
elf 0xa4558 body 28
Sig: undefined __thiscall setMesh(AEGeometry * this, ushort param_1)

## decompile
```c

/* AEGeometry::setMesh(unsigned short) */

void __thiscall AEGeometry::setMesh(AEGeometry *this,ushort param_1)

{
  if (*(int *)(this + 0xc) == 0) {
    (*(code *)(&UNK_001abd58 + DAT_001abd54))(*(undefined4 *)(this + 0x2c),param_1,this + 0xc);
    return;
  }
  (*(code *)(DAT_001abd44 + 0x1abd48))();
  return;
}

```

## target disasm
```
  000b4558: mov r3,r0
  000b455a: mov r2,r1
  000b455c: ldr.w r1,[r3,#0xc]!
  000b4560: ldr r0,[r3,#0x20]
  000b4562: cbz r1,0x000b456a
  000b4564: movs r3,#0x0
  000b4566: b.w 0x001abd38
  000b456a: mov r1,r2
  000b456c: mov r2,r3
  000b456e: b.w 0x001abd48
  001abd48: bx pc
```
