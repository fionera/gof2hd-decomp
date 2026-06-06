# Standing::applyMissionCompleted

ELF vaddr 0x11d992, body 8 bytes
Signature: undefined __thiscall applyMissionCompleted(Standing * this, int param_1)

## Ghidra decompile
```c

/* Standing::applyMissionCompleted(int) */

void __thiscall Standing::applyMissionCompleted(Standing *this,int param_1)

{
  (*(code *)(DAT_001ac3c4 + 0x1ac3c8))(this,param_1,0xfffffffb);
  return;
}

```

## Target disassembly (must match)
```
  0012d992: mvn r2,#0x4
  0012d996: b.w 0x001ac3b8
```

Verify: tools/try.sh work/src/applyMissionCompleted.cpp applyMissionCompleted 11d992 32
