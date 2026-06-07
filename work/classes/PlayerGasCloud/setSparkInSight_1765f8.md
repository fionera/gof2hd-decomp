# PlayerGasCloud::setSparkInSight
elf 0x1765f8 body 22
Sig: undefined __thiscall setSparkInSight(PlayerGasCloud * this, int param_1, bool param_2)

## decompile
```c

/* PlayerGasCloud::setSparkInSight(int, bool) */

void __thiscall PlayerGasCloud::setSparkInSight(PlayerGasCloud *this,int param_1,bool param_2)

{
  uint uVar1;
  
  if (*(uint **)(this + 0x138) != (uint *)0x0) {
    uVar1 = **(uint **)(this + 0x138);
    if ((uint)param_1 < uVar1) {
      this = *(PlayerGasCloud **)(*(int *)(this + 0x150) + 4);
    }
    if ((uint)param_1 < uVar1) {
      this[param_1] = (PlayerGasCloud)param_2;
    }
  }
  return;
}

```

## target disasm
```
  001865f8: ldr.w r3,[r0,#0x138]
  001865fc: cbz r3,0x0018660c
  001865fe: ldr r3,[r3,#0x0]
  00186600: cmp r3,r1
  00186602: ittt hi
  00186604: ldr.hi.w r0,[r0,#0x150]
  00186608: ldr.hi r0,[r0,#0x4]
  0018660a: strb.hi r2,[r0,r1]
  0018660c: bx lr
```
