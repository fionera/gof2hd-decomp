# PlayerGasCloud::isSparkAlive
elf 0x176610 body 42
Sig: undefined __thiscall isSparkAlive(PlayerGasCloud * this, int param_1)

## decompile
```c

/* PlayerGasCloud::isSparkAlive(int) */

bool __thiscall PlayerGasCloud::isSparkAlive(PlayerGasCloud *this,int param_1)

{
  if (*(uint **)(this + 0x138) == (uint *)0x0) {
    return false;
  }
  if (**(uint **)(this + 0x138) <= (uint)param_1) {
    return false;
  }
  return DAT_0018663c < *(int *)(*(int *)(*(int *)(this + 0x14c) + 4) + param_1 * 4);
}

```

## target disasm
```
  00186610: mov r2,r0
  00186612: ldr.w r0,[r0,#0x138]
  00186616: cbz r0,0x00186636
  00186618: ldr r3,[r0,#0x0]
  0018661a: movs r0,#0x0
  0018661c: cmp r3,r1
  0018661e: it ls
  00186620: bx.ls lr
  00186622: ldr.w r2,[r2,#0x14c]
  00186626: ldr r2,[r2,#0x4]
  00186628: ldr.w r1,[r2,r1,lsl #0x2]
  0018662c: ldr r2,[0x0018663c]
  0018662e: cmp r1,r2
  00186630: it gt
  00186632: mov.gt r0,#0x1
  00186634: bx lr
  00186636: movs r0,#0x0
  00186638: bx lr
```
