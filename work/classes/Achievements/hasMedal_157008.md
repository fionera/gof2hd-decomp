# Achievements::hasMedal
elf 0x157008 body 16
Sig: undefined __thiscall hasMedal(Achievements * this, int param_1, int param_2)

## decompile
```c

/* Achievements::hasMedal(int, int) */

bool __thiscall Achievements::hasMedal(Achievements *this,int param_1,int param_2)

{
  return *(int *)(*(int *)this + param_1 * 4) == param_2;
}

```

## target disasm
```
  00167008: ldr r0,[r0,#0x0]
  0016700a: ldr.w r0,[r0,r1,lsl #0x2]
  0016700e: subs r0,r0,r2
  00167010: clz r0,r0
  00167014: lsrs r0,r0,#0x5
  00167016: bx lr
```
