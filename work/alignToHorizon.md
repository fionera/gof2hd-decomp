# PlayerEgo::alignToHorizon

ELF vaddr 0xa12f2, body 8 bytes
Signature: undefined __stdcall alignToHorizon(void)

## Ghidra decompile
```c

/* PlayerEgo::alignToHorizon() */

void PlayerEgo::alignToHorizon(void)

{
  int in_r0;
  
  *(undefined1 *)(in_r0 + 0x2f4) = 1;
  return;
}

```

## Target disassembly (must match)
```
  000b12f2: movs r1,#0x1
  000b12f4: strb.w r1,[r0,#0x2f4]
  000b12f8: bx lr
```

Verify: tools/try.sh work/src/alignToHorizon.cpp alignToHorizon a12f2 32
