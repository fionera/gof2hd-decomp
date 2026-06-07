# AEGeometry::addChild
elf 0xa453a body 30
Sig: undefined __stdcall addChild(uint param_1)

## decompile
```c

/* AEGeometry::addChild(unsigned int) */

void AEGeometry::addChild(uint param_1)

{
  uint in_r1;
  
  AbyssEngine::PaintCanvas::TransformAddChild
            (*(PaintCanvas **)(param_1 + 0x2c),*(uint *)(param_1 + 0xc),in_r1);
  if (*(int *)(param_1 + 0x14) != -1) {
    *(int *)(param_1 + 0x10) = *(int *)(param_1 + 0x14);
  }
  *(uint *)(param_1 + 0x14) = in_r1;
  return;
}

```

## target disasm
```
  000b453a: push {r4,r5,r7,lr}
  000b453c: add r7,sp,#0x8
  000b453e: mov r4,r1
  000b4540: mov r5,r0
  000b4542: ldr r1,[r0,#0xc]
  000b4544: mov r2,r4
  000b4546: ldr r0,[r0,#0x2c]
  000b4548: blx 0x000720d0
  000b454c: ldr r0,[r5,#0x14]
  000b454e: adds r1,r0,#0x1
  000b4550: it ne
  000b4552: str.ne r0,[r5,#0x10]
  000b4554: str r4,[r5,#0x14]
  000b4556: pop {r4,r5,r7,pc}
```
