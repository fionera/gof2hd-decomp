# Engine::ResetUVMatrix
elf 0x85184 body 104
Sig: undefined __thiscall ResetUVMatrix(Engine * this)

## decompile
```c

/* AbyssEngine::Engine::ResetUVMatrix() */

void __thiscall AbyssEngine::Engine::ResetUVMatrix(Engine *this)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  
  if (**(char **)(DAT_000951f0 + 0x9518a) == '\0') {
    glMatrixMode(0x1702);
    glLoadIdentity();
    glScalef(0x3f800000,DAT_000951ec,0x3f800000);
    (*(code *)(DAT_001ab644 + 0x1ab648))(0x1700);
    return;
  }
  uVar1 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  uVar2 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  uVar3 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  *(undefined4 *)(this + 0x1c4) = 0x3f800000;
  *(undefined4 *)(this + 0x1d8) = 0x3f800000;
  *(undefined4 *)(this + 0x1ec) = 0x3f800000;
  *(undefined4 *)(this + 0x200) = 0x3f800000;
  *(undefined4 *)(this + 0x1c8) = 0;
  *(undefined4 *)(this + 0x1cc) = uVar1;
  *(undefined4 *)(this + 0x1d0) = uVar2;
  *(undefined4 *)(this + 0x1d4) = uVar3;
  *(undefined4 *)(this + 0x1dc) = 0;
  *(undefined4 *)(this + 0x1e0) = uVar1;
  *(undefined4 *)(this + 0x1e4) = uVar2;
  *(undefined4 *)(this + 0x1e8) = uVar3;
  *(undefined4 *)(this + 0x1f0) = 0;
  *(undefined4 *)(this + 500) = uVar1;
  *(undefined4 *)(this + 0x1f8) = uVar2;
  *(undefined4 *)(this + 0x1fc) = uVar3;
  return;
}

```

## target disasm
```
  00095184: ldr r1,[0x000951f0]
  00095186: add r1,pc
  00095188: ldr r1,[r1,#0x0]
  0009518a: ldrb r1,[r1,#0x0]
  0009518c: cbz r1,0x000951c0
  0009518e: mov.w r1,#0x3f800000
  00095192: vmov.i32 q8,#0x0
  00095196: str.w r1,[r0,#0x1c4]
  0009519a: str.w r1,[r0,#0x1d8]
  0009519e: str.w r1,[r0,#0x1ec]
  000951a2: str.w r1,[r0,#0x200]
  000951a6: add.w r1,r0,#0x1c8
  000951aa: vst1.32 {d16,d17},[r1]
  000951ae: add.w r1,r0,#0x1dc
  000951b2: add.w r0,r0,#0x1f0
  000951b6: vst1.32 {d16,d17},[r1]
  000951ba: vst1.32 {d16,d17},[r0]
  000951be: bx lr
  000951c0: push {r7,lr}
  000951c2: mov r7,sp
  000951c4: movw r0,#0x1702
  000951c8: blx 0x0006ff64
  000951cc: blx 0x0006fe8c
  000951d0: ldr r1,[0x000951ec]
  000951d2: mov.w r0,#0x3f800000
  000951d6: mov.w r2,#0x3f800000
  000951da: blx 0x0006fe98
  000951de: mov.w r0,#0x1700
  000951e2: pop.w {r7,lr}
  000951e6: b.w 0x001ab638
  001ab638: bx pc
```
