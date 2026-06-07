# Objective::Objective
elf 0x97c0e body 28
Sig: undefined __thiscall Objective(Objective * this, int param_1, int param_2, int param_3, Level * param_4)

## decompile
```c

/* Objective::Objective(int, int, int, Level*) */

void __thiscall
Objective::Objective(Objective *this,int param_1,int param_2,int param_3,Level *param_4)

{
  *(int *)this = param_1;
  *(int *)(this + 4) = param_2;
  *(int *)(this + 8) = param_3;
  *(Level **)(this + 0xc) = param_4;
  *(undefined4 *)(this + 0x10) = 0;
  *(undefined4 *)(this + 0x14) = 0;
  if (param_1 == 0xd) {
    *(int *)(this + 0x18) = param_3;
  }
  return;
}

```

## target disasm
```
  000a7c0e: push {r7,lr}
  000a7c10: mov r7,sp
  000a7c12: ldr.w r12,[r7,#0x8]
  000a7c16: mov.w lr,#0x0
  000a7c1a: cmp r1,#0xd
  000a7c1c: stm r0,{r1,r2,r3,r12,lr}
  000a7c20: str.w lr,[r0,#0x14]
  000a7c24: it eq
  000a7c26: str.eq r3,[r0,#0x18]
  000a7c28: pop {r7,pc}
```
