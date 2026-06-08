# String::String
elf 0x72398 body 18
Sig: undefined __thiscall String(String * this)

## decompile
```c

/* AbyssEngine::String::String() */

void __thiscall AbyssEngine::String::String(String *this)

{
  *(int *)this = *(int *)(DAT_000823ac + 0x823a0) + 8;
  *(undefined4 *)(this + 4) = 0;
  *(undefined4 *)(this + 8) = 0;
  return;
}

```

## target disasm
```
  00082398: ldr r1,[0x000823ac]
  0008239a: movs r2,#0x0
  0008239c: add r1,pc
  0008239e: ldr r1,[r1,#0x0]
  000823a0: adds r1,#0x8
  000823a2: strd r1,r2,[r0,#0x0]
  000823a6: str r2,[r0,#0x8]
  000823a8: bx lr
```
