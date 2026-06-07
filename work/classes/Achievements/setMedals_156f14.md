# Achievements::setMedals
elf 0x156f14 body 56
Sig: undefined __thiscall setMedals(Achievements * this, int * param_1, int param_2)

## decompile
```c

/* Achievements::setMedals(int*, int) */

void __thiscall Achievements::setMedals(Achievements *this,int *param_1,int param_2)

{
  uint uVar1;
  int iVar2;
  
  for (iVar2 = 0; iVar2 < param_2; iVar2 = iVar2 + 1) {
    uVar1 = param_1[iVar2];
    if (3 < uVar1) {
      uVar1 = 0;
    }
    *(uint *)(*(int *)this + iVar2 * 4) = uVar1;
  }
  for (; param_2 < 0x2d; param_2 = param_2 + 1) {
    *(undefined4 *)(*(int *)this + param_2 * 4) = 0;
  }
  return;
}

```

## target disasm
```
  00166f14: push {r4,r6,r7,lr}
  00166f16: add r7,sp,#0x8
  00166f18: mov.w r12,#0x0
  00166f1c: mov.w lr,#0x0
  00166f20: b 0x00166f36
  00166f22: ldr.w r3,[r1,lr,lsl #0x2]
  00166f26: ldr r4,[r0,#0x0]
  00166f28: cmp r3,#0x4
  00166f2a: it cs
  00166f2c: mov.cs r3,r12
  00166f2e: str.w r3,[r4,lr,lsl #0x2]
  00166f32: add.w lr,lr,#0x1
  00166f36: cmp lr,r2
  00166f38: blt 0x00166f22
  00166f3a: movs r1,#0x0
  00166f3c: b 0x00166f46
  00166f3e: ldr r3,[r0,#0x0]
  00166f40: str.w r1,[r3,r2,lsl #0x2]
  00166f44: adds r2,#0x1
  00166f46: cmp r2,#0x2d
  00166f48: blt 0x00166f3e
  00166f4a: pop {r4,r6,r7,pc}
```
