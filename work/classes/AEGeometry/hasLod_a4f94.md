# AEGeometry::hasLod
elf 0xa4f94 body 10
Sig: undefined __thiscall hasLod(AEGeometry * this)

## decompile
```c

/* AEGeometry::hasLod() */

bool __thiscall AEGeometry::hasLod(AEGeometry *this)

{
  return *(int *)(this + 0x54) != 0;
}

```

## target disasm
```
  000b4f94: ldr r0,[r0,#0x54]
  000b4f96: cmp r0,#0x0
  000b4f98: it ne
  000b4f9a: mov.ne r0,#0x1
  000b4f9c: bx lr
```
