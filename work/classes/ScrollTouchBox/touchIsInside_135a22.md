# ScrollTouchBox::touchIsInside
elf 0x135a22 body 50
Sig: undefined __thiscall touchIsInside(ScrollTouchBox * this, int param_1, int param_2)

## decompile
```c

/* ScrollTouchBox::touchIsInside(int, int) */

bool __thiscall ScrollTouchBox::touchIsInside(ScrollTouchBox *this,int param_1,int param_2)

{
  int iVar1;
  int in_r12;
  int iVar2;
  bool bVar3;
  bool bVar4;
  
  iVar2 = *(int *)(this + 4);
  bVar4 = SBORROW4(iVar2,param_1);
  iVar1 = iVar2 - param_1;
  bVar3 = iVar2 == param_1;
  if (iVar2 <= param_1) {
    in_r12 = *(int *)(this + 8);
    bVar4 = SBORROW4(in_r12,param_2);
    iVar1 = in_r12 - param_2;
    bVar3 = in_r12 == param_2;
  }
  if (bVar3 || iVar1 < 0 != bVar4) {
    if (param_1 < iVar2 + *(int *)(this + 0xc)) {
      return param_2 < *(int *)(this + 0x10) + in_r12;
    }
    return false;
  }
  return false;
}

```

## target disasm
```
  00145a22: push {r7,lr}
  00145a24: mov r7,sp
  00145a26: ldr.w lr,[r0,#0x4]
  00145a2a: mov r3,r0
  00145a2c: cmp lr,r1
  00145a2e: itt le
  00145a30: ldr.le.w r12,[r3,#0x8]
  00145a34: cmp.le r12,r2
  00145a36: ble 0x00145a3c
  00145a38: movs r0,#0x0
  00145a3a: pop {r7,pc}
  00145a3c: ldr r0,[r3,#0xc]
  00145a3e: add lr,r0
  00145a40: movs r0,#0x0
  00145a42: cmp lr,r1
  00145a44: it le
  00145a46: pop.le {r7,pc}
  00145a48: ldr r1,[r3,#0x10]
  00145a4a: add r1,r12
  00145a4c: cmp r1,r2
  00145a4e: it gt
  00145a50: mov.gt r0,#0x1
  00145a52: pop {r7,pc}
```
