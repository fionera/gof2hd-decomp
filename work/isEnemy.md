# KIPlayer::isEnemy

ELF vaddr 0xa61e4, body 8 bytes
Signature: undefined __thiscall isEnemy(KIPlayer * this)

## Ghidra decompile
```c

/* KIPlayer::isEnemy() */

undefined1 __thiscall KIPlayer::isEnemy(KIPlayer *this)

{
  return *(undefined1 *)(*(int *)(this + 4) + 0x5c);
}

```

## Target disassembly (must match)
```
  000b61e4: ldr r0,[r0,#0x4]
  000b61e6: ldrb.w r0,[r0,#0x5c]
  000b61ea: bx lr
```

Verify: tools/try.sh work/src/isEnemy.cpp isEnemy a61e4 32
