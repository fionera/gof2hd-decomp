# CutScene::render2D

ELF vaddr 0x9984c, body 8 bytes
Signature: undefined __stdcall render2D(void)

## Ghidra decompile
```c

/* CutScene::render2D() */

void CutScene::render2D(void)

{
  undefined4 *in_r0;
  
  (*(code *)(DAT_001ab9e4 + 0x1ab9e8))(*in_r0);
  return;
}

```

## Target disassembly (must match)
```
  000a984c: ldr r0,[r0,#0x0]
  000a984e: b.w 0x001ab9d8
  001ab9d8: bx pc
```

Verify: tools/try.sh work/src/render2D.cpp render2D 9984c 32
