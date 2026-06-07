# Globals::releaseResources
elf 0xe479c body 34
Sig: undefined __stdcall releaseResources(void)

## decompile
```c

/* Globals::releaseResources() */

void Globals::releaseResources(void)

{
  AbyssEngine::PaintCanvas::ReleaseAllResources();
  (*(code *)(DAT_001ac2c4 + 0x1ac2c8))(**(undefined4 **)(DAT_000f47c0 + 0xf47b2));
  return;
}

```

## target disasm
```
  000f479c: push {r7,lr}
  000f479e: mov r7,sp
  000f47a0: ldr r0,[0x000f47bc]
  000f47a2: add r0,pc
  000f47a4: ldr r0,[r0,#0x0]
  000f47a6: ldr r0,[r0,#0x0]
  000f47a8: blx 0x0006f964
  000f47ac: ldr r0,[0x000f47c0]
  000f47ae: add r0,pc
  000f47b0: ldr r0,[r0,#0x0]
  000f47b2: ldr r0,[r0,#0x0]
  000f47b4: pop.w {r7,lr}
  000f47b8: b.w 0x001ac2b8
  001ac2b8: bx pc
```
