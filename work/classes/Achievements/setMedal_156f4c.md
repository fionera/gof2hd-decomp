# Achievements::setMedal
elf 0x156f4c body 8
Sig: undefined __thiscall setMedal(Achievements * this, int param_1, int param_2)

## decompile
```c

/* Achievements::setMedal(int, int) */

void __thiscall Achievements::setMedal(Achievements *this,int param_1,int param_2)

{
  *(int *)(*(int *)this + param_1 * 4) = param_2;
  return;
}

```

## target disasm
```
  00166f4c: ldr r0,[r0,#0x0]
  00166f4e: str.w r2,[r0,r1,lsl #0x2]
  00166f52: bx lr
```
