# Sprite::nextFrame

ELF vaddr 0xd1018, body 8 bytes
Signature: undefined __thiscall nextFrame(Sprite * this)

## Ghidra decompile
```c

/* Sprite::nextFrame() */

void __thiscall Sprite::nextFrame(Sprite *this)

{
  (*(code *)(DAT_001ac084 + 0x1ac088))(this,*(int *)(this + 0x38) + 1);
  return;
}

```

## Target disassembly (must match)
```
  000e1018: ldr r1,[r0,#0x38]
  000e101a: adds r1,#0x1
  000e101c: b.w 0x001ac078
```

Verify: tools/try.sh work/src/nextFrame.cpp nextFrame d1018 32
