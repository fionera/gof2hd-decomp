# PlayerEgo::pitchAllPrimaryGuns

ELF vaddr 0xa2208, body 8 bytes
Signature: undefined __stdcall pitchAllPrimaryGuns(float param_1)

## Ghidra decompile
```c

/* PlayerEgo::pitchAllPrimaryGuns(float) */

void PlayerEgo::pitchAllPrimaryGuns(float param_1)

{
  undefined4 *in_r0;
  
  (*(code *)(DAT_001abca4 + 0x1abca8))(*in_r0);
  return;
}

```

## Target disassembly (must match)
```
  000b2208: ldr r0,[r0,#0x0]
  000b220a: b.w 0x001abc98
  001abc98: bx pc
```

Verify: tools/try.sh work/src/pitchAllPrimaryGuns.cpp pitchAllPrimaryGuns a2208 32
