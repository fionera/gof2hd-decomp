# Achievements::setMedal

ELF vaddr 0x156f4c, body 8 bytes
Signature: undefined __thiscall setMedal(Achievements * this, int param_1, int param_2)

## Ghidra decompile
```c

/* Achievements::setMedal(int, int) */

void __thiscall Achievements::setMedal(Achievements *this,int param_1,int param_2)

{
  *(int *)(*(int *)this + param_1 * 4) = param_2;
  return;
}

```

## Target disassembly (must match)
```
  00166f4c: ldr r0,[r0,#0x0]
  00166f4e: str.w r2,[r0,r1,lsl #0x2]
  00166f52: bx lr
```

Verify: tools/try.sh work/src/setMedal.cpp setMedal 156f4c 32
