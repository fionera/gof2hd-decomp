# Status::hardCoreMode
elf 0xabe40 body 30
Sig: undefined __stdcall hardCoreMode(void)

## decompile
```c

/* Status::hardCoreMode() */

bool Status::hardCoreMode(void)

{
  return *(float *)(*(int *)(DAT_000bbe60 + 0xbbe4a) + 0x2c) == 1.5;
}

```
## target disasm
```
  000bbe40: ldr r0,[0x000bbe60]
  000bbe42: vmov.f32 s0,0x3fc00000
  000bbe46: add r0,pc
  000bbe48: ldr r0,[r0,#0x0]
  000bbe4a: vldr.32 s2,[r0,#0x2c]
  000bbe4e: movs r0,#0x0
  000bbe50: vcmpe.f32 s2,s0
  000bbe54: vmrs apsr,fpscr
  000bbe58: it eq
  000bbe5a: mov.eq r0,#0x1
  000bbe5c: bx lr
```
