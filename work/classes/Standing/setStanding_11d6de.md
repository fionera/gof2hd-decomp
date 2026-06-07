# Standing::setStanding
elf 0x11d6de body 8
Sig: undefined __thiscall setStanding(Standing * this, int param_1, int param_2)

## decompile
```c

/* Standing::setStanding(int, int) */

void __thiscall Standing::setStanding(Standing *this,int param_1,int param_2)

{
  *(int *)(*(int *)this + param_1 * 4) = param_2;
  return;
}

```

## target disasm
```
  0012d6de: ldr r0,[r0,#0x0]
  0012d6e0: str.w r2,[r0,r1,lsl #0x2]
  0012d6e4: bx lr
```
