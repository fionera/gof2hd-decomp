# SolarSystem::getWarpGateEnumIndex

ELF vaddr 0x1557f0, body 8 bytes
Signature: undefined __thiscall getWarpGateEnumIndex(SolarSystem * this)

## Ghidra decompile
```c

/* SolarSystem::getWarpGateEnumIndex() */

void __thiscall SolarSystem::getWarpGateEnumIndex(SolarSystem *this)

{
  (*(code *)(DAT_001ac574 + 0x1ac578))(this,*(undefined4 *)(this + 0x30));
  return;
}

```

## Target disassembly (must match)
```
  001657f0: ldr r1,[r0,#0x30]
  001657f2: b.w 0x001ac568
  001ac568: bx pc
```

Verify: tools/try.sh work/src/getWarpGateEnumIndex.cpp getWarpGateEnumIndex 1557f0 32
