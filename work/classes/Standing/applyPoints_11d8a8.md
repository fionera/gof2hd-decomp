# Standing::applyPoints
elf 0x11d8a8 body 38
Sig: undefined __thiscall applyPoints(Standing * this, int param_1, int param_2)

## decompile
```c

/* Standing::applyPoints(int, int) */

void __thiscall Standing::applyPoints(Standing *this,int param_1,int param_2)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  
  iVar1 = *(int *)this;
  iVar2 = param_2 + *(int *)(iVar1 + param_1 * 4);
  *(int *)(iVar1 + param_1 * 4) = iVar2;
  if (iVar2 < 0x65) {
    if (-0x65 < iVar2) {
      return;
    }
    uVar3 = 0xffffff9c;
  }
  else {
    uVar3 = 100;
  }
  *(undefined4 *)(iVar1 + param_1 * 4) = uVar3;
  return;
}

```

## target disasm
```
  0012d8a8: ldr r0,[r0,#0x0]
  0012d8aa: ldr.w r3,[r0,r1,lsl #0x2]
  0012d8ae: add r2,r3
  0012d8b0: str.w r2,[r0,r1,lsl #0x2]
  0012d8b4: cmp r2,#0x64
  0012d8b6: ble 0x0012d8bc
  0012d8b8: movs r2,#0x64
  0012d8ba: b 0x0012d8c8
  0012d8bc: cmn r2,#0x65
  0012d8c0: it gt
  0012d8c2: bx.gt lr
  0012d8c4: mvn r2,#0x63
  0012d8c8: str.w r2,[r0,r1,lsl #0x2]
  0012d8cc: bx lr
```
