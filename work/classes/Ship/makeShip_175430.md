# Ship::makeShip
elf 0x175430 body 18
Sig: undefined __stdcall makeShip(int param_1)

## decompile
```c

/* Ship::makeShip(int) */

void Ship::makeShip(int param_1)

{
  int iVar1;
  __fn *in_r1;
  void *in_r2;
  int in_r3;
  void *unaff_r4;
  
  iVar1 = clone((Ship *)param_1,in_r1,in_r2,in_r3,unaff_r4);
  if (-1 < (int)in_r1) {
    *(__fn **)(iVar1 + 0x14) = in_r1;
  }
  return;
}

```
## target disasm
```
  00185430: push {r4,r6,r7,lr}
  00185432: add r7,sp,#0x8
  00185434: mov r4,r1
  00185436: blx 0x000734bc
  0018543a: cmp r4,#0x0
  0018543c: blt 0x00185440
  0018543e: str r4,[r0,#0x14]
  00185440: pop {r4,r6,r7,pc}
```
