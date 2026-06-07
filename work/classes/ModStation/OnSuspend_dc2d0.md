# ModStation::OnSuspend
elf 0xdc2d0 body 24
Sig: undefined __thiscall OnSuspend(ModStation * this)

## decompile
```c

/* ModStation::OnSuspend() */

void __thiscall ModStation::OnSuspend(ModStation *this)

{
  int iVar1;
  
  iVar1 = DAT_000ec2e8;
  this[2].field_8 = (int *)0x0;
  if (**(int **)(iVar1 + 0xec2dc) == 0) {
    return;
  }
  (*(code *)(DAT_001ac194 + 0x1ac198))();
  return;
}

```

## target disasm
```
  000ec2d0: ldr r1,[0x000ec2e8]
  000ec2d2: movs r2,#0x0
  000ec2d4: str.w r2,[r0,#0x128]
  000ec2d8: add r1,pc
  000ec2da: ldr r1,[r1,#0x0]
  000ec2dc: ldr r0,[r1,#0x0]
  000ec2de: cmp r0,#0x0
  000ec2e0: it ne
  000ec2e2: b.ne.w 0x001ac188
  000ec2e6: bx lr
```
