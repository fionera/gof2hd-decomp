# PlayerEgo::getHitpoints

ELF vaddr 0x9b190, body 8 bytes
Signature: undefined __stdcall getHitpoints(void)

## Ghidra decompile
```c

/* PlayerEgo::getHitpoints() */

void PlayerEgo::getHitpoints(void)

{
  undefined4 *in_r0;
  
  (*(code *)(DAT_001aba94 + 0x1aba98))(*in_r0);
  return;
}

```

## Target disassembly (must match)
```
  000ab190: ldr r0,[r0,#0x0]
  000ab192: b.w 0x001aba88
  001aba88: bx pc
```

Verify: tools/try.sh work/src/getHitpoints.cpp getHitpoints 9b190 32
