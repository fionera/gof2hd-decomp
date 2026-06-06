# PlayerEgo::ResumeEngineSound

ELF vaddr 0x9a5e2, body 8 bytes
Signature: undefined __stdcall ResumeEngineSound(void)

## Ghidra decompile
```c

/* PlayerEgo::ResumeEngineSound() */

void PlayerEgo::ResumeEngineSound(void)

{
  undefined4 *in_r0;
  
  (*(code *)(DAT_001aba34 + 0x1aba38))(*in_r0,0);
  return;
}

```

## Target disassembly (must match)
```
  000aa5e2: ldr r0,[r0,#0x0]
  000aa5e4: movs r1,#0x0
  000aa5e6: b.w 0x001aba28
```

Verify: tools/try.sh work/src/ResumeEngineSound.cpp ResumeEngineSound 9a5e2 32
