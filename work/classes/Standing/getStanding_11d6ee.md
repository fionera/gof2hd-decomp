# Standing::getStanding
elf 0x11d6ee body 56
Sig: undefined __stdcall getStanding(int param_1)

## decompile
```c

/* Standing::getStanding(int) */

undefined4 Standing::getStanding(int param_1)

{
  undefined4 uVar1;
  int in_r1;
  int iVar2;
  
  iVar2 = *(int *)(param_1 + 4);
  if (-1 < iVar2) {
    if (in_r1 == 1) {
      uVar1 = 0x46;
      if (iVar2 == 2) {
        uVar1 = 100;
      }
      if (iVar2 == 3) {
        uVar1 = 0xffffff9c;
      }
      return uVar1;
    }
    if (in_r1 == 0) {
      uVar1 = 0x46;
      if (iVar2 == 0) {
        uVar1 = 100;
      }
      if (iVar2 == 1) {
        uVar1 = 0xffffff9c;
      }
      return uVar1;
    }
  }
  return *(undefined4 *)(*(int *)param_1 + in_r1 * 4);
}

```

## target disasm
```
  0012d6ee: ldr r2,[r0,#0x4]
  0012d6f0: cmp r2,#0x0
  0012d6f2: blt 0x0012d70c
  0012d6f4: cmp r1,#0x1
  0012d6f6: beq 0x0012d714
  0012d6f8: cbnz r1,0x0012d70c
  0012d6fa: movs r0,#0x46
  0012d6fc: cmp r2,#0x0
  0012d6fe: it eq
  0012d700: mov.eq r0,#0x64
  0012d702: cmp r2,#0x1
  0012d704: it eq
  0012d706: mvn.eq r0,#0x63
  0012d70a: bx lr
  0012d70c: ldr r0,[r0,#0x0]
  0012d70e: ldr.w r0,[r0,r1,lsl #0x2]
  0012d712: bx lr
  0012d714: movs r0,#0x46
  0012d716: cmp r2,#0x2
  0012d718: it eq
  0012d71a: mov.eq r0,#0x64
  0012d71c: cmp r2,#0x3
  0012d71e: it eq
  0012d720: mvn.eq r0,#0x63
  0012d724: bx lr
```
