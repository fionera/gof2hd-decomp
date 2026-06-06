# Agent::setSystemName

ELF vaddr 0x17772c, body 8 bytes
Signature: undefined __stdcall setSystemName(String param_1)

## Ghidra decompile
```c

/* Agent::setSystemName(AbyssEngine::String) */

void Agent::setSystemName(int param_1)

{
  (*(code *)(DAT_001ac554 + 0x1ac558))(param_1 + 0x18);
  return;
}

```

## Target disassembly (must match)
```
  0018772c: adds r0,#0x18
  0018772e: b.w 0x001ac548
  001ac548: bx pc
```

Verify: tools/try.sh work/src/setSystemName.cpp setSystemName 17772c 32
