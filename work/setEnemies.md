# KIPlayer::setEnemies

ELF vaddr 0xa6178, body 8 bytes
Signature: undefined __stdcall setEnemies(Array * param_1)

## Ghidra decompile
```c

/* KIPlayer::setEnemies(Array<Player*>*) */

void KIPlayer::setEnemies(Array *param_1)

{
  (*(code *)(DAT_001abcc4 + 0x1abcc8))(*(undefined4 *)(param_1 + 4));
  return;
}

```

## Target disassembly (must match)
```
  000b6178: ldr r0,[r0,#0x4]
  000b617a: b.w 0x001abcb8
  001abcb8: bx pc
```

Verify: tools/try.sh work/src/setEnemies.cpp setEnemies a6178 32
