# RadioMessage::RadioMessage
elf 0x15162a body 38
Sig: undefined __thiscall RadioMessage(RadioMessage * this, int param_1, int param_2, Objective * param_3)

## decompile
```c

/* RadioMessage::RadioMessage(int, int, Objective*) */

void __thiscall
RadioMessage::RadioMessage(RadioMessage *this,int param_1,int param_2,Objective *param_3)

{
  *(undefined4 *)(this + 0x1c) = 0;
  *(undefined4 *)this = 0;
  *(Objective **)(this + 4) = param_3;
  *(int *)(this + 8) = param_1;
  *(int *)(this + 0xc) = param_2;
  *(undefined4 *)(this + 0x10) = 0xb;
  *(undefined2 *)(this + 0x20) = 0;
  *(undefined4 *)(this + 0x24) = 0;
  return;
}

```

## target disasm
```
  0016162a: push {r7,lr}
  0016162c: mov r7,sp
  0016162e: mov.w r12,#0x0
  00161632: mov.w lr,#0xb
  00161636: str.w r12,[r0,#0x1c]
  0016163a: strd r12,r3,[r0,#0x0]
  0016163e: add.w r3,r0,#0x8
  00161642: stm.w r3!,{r1,r2,lr}
  00161646: strh.w r12,[r0,#0x20]
  0016164a: str.w r12,[r0,#0x24]
  0016164e: pop {r7,pc}
```
