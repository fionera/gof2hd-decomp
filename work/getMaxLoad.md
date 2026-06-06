# Ship::getMaxLoad

ELF vaddr 0x174c40, body 8 bytes
Signature: undefined __stdcall getMaxLoad(void)

## Ghidra decompile
```c

/* Ship::getMaxLoad() */

int Ship::getMaxLoad(void)

{
  int in_r0;
  
  return *(int *)(in_r0 + 0x28) + *(int *)(in_r0 + 0xc);
}

```

## Target disassembly (must match)
```
  00184c40: ldr r1,[r0,#0xc]
  00184c42: ldr r0,[r0,#0x28]
  00184c44: add r0,r1
  00184c46: bx lr
```

Verify: tools/try.sh work/src/getMaxLoad.cpp getMaxLoad 174c40 32
