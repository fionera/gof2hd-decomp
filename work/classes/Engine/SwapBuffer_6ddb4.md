# Engine::SwapBuffer
elf 0x6ddb4 body 26
Sig: undefined __thiscall SwapBuffer(Engine * this)

## decompile
```c

/* AbyssEngine::Engine::SwapBuffer() */

void __thiscall AbyssEngine::Engine::SwapBuffer(Engine *this)

{
  uint uVar1;
  
  for (uVar1 = 0; uVar1 < *(uint *)(this + 0x3d8); uVar1 = uVar1 + 1) {
    *(undefined4 *)(*(int *)(this + 0x3dc) + uVar1 * 4) = 0;
  }
  return;
}

```

## target disasm
```
  0007ddb4: movs r1,#0x0
  0007ddb6: movs r2,#0x0
  0007ddb8: ldr.w r3,[r0,#0x3d8]
  0007ddbc: cmp r2,r3
  0007ddbe: it cs
  0007ddc0: bx.cs lr
  0007ddc2: ldr.w r3,[r0,#0x3dc]
  0007ddc6: str.w r1,[r3,r2,lsl #0x2]
  0007ddca: adds r2,#0x1
  0007ddcc: b 0x0007ddb8
```
