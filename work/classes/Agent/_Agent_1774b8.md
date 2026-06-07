# Agent::~Agent
elf 0x1774b8 body 68
Sig: undefined __thiscall ~Agent(Agent * this)

## decompile
```c

/* Agent::~Agent() */

void __thiscall Agent::~Agent(Agent *this)

{
  code *UNRECOVERED_JUMPTABLE;
  
  if (*(void **)(this + 0x88) != (void *)0x0) {
    operator_delete__(*(void **)(this + 0x88));
  }
  *(undefined4 *)(this + 0x88) = 0;
  if (*(int **)(this + 0xc) != (int *)0x0) {
    (**(code **)(**(int **)(this + 0xc) + 4))();
    *(undefined4 *)(this + 0xc) = 0;
  }
  UNRECOVERED_JUMPTABLE = *(code **)(DAT_001874fc + 0x1874e0);
  (*UNRECOVERED_JUMPTABLE)(this + 0x78);
  (*UNRECOVERED_JUMPTABLE)(this + 0x6c);
  (*UNRECOVERED_JUMPTABLE)(this + 0x18);
                    /* WARNING: Could not recover jumptable at 0x001874fa. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*UNRECOVERED_JUMPTABLE)(this);
  return;
}

```

## target disasm
```
  001874b8: push {r4,r5,r7,lr}
  001874ba: add r7,sp,#0x8
  001874bc: mov r4,r0
  001874be: ldr.w r0,[r0,#0x88]
  001874c2: cbz r0,0x001874c8
  001874c4: blx 0x0006ebfc
  001874c8: ldr r0,[r4,#0xc]
  001874ca: movs r5,#0x0
  001874cc: str.w r5,[r4,#0x88]
  001874d0: cbz r0,0x001874da
  001874d2: ldr r1,[r0,#0x0]
  001874d4: ldr r1,[r1,#0x4]
  001874d6: blx r1
  001874d8: str r5,[r4,#0xc]
  001874da: ldr r0,[0x001874fc]
  001874dc: add r0,pc
  001874de: ldr r5,[r0,#0x0]
  001874e0: add.w r0,r4,#0x78
  001874e4: blx r5
  001874e6: add.w r0,r4,#0x6c
  001874ea: blx r5
  001874ec: add.w r0,r4,#0x18
  001874f0: blx r5
  001874f2: mov r0,r4
  001874f4: mov r1,r5
  001874f6: pop.w {r4,r5,r7,lr}
  001874fa: bx r1
```
