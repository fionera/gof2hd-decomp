# Player::setEmpData
elf 0xa2a34 body 34
Sig: undefined __thiscall setEmpData(Player * this, int param_1, int param_2)

## decompile
```c

/* Player::setEmpData(int, int) */

void __thiscall Player::setEmpData(Player *this,int param_1,int param_2)

{
  *(int *)(this + 0x7c) = param_1;
  if (*(int *)(this + 0x80) < param_1) {
    *(int *)(this + 0x80) = param_1;
  }
  updateDamageRate(this);
  *(int *)(this + 0xe4) = param_2;
  return;
}

```
## target disasm
```
  000b2a34: push {r4,r5,r7,lr}
  000b2a36: add r7,sp,#0x8
  000b2a38: mov r5,r0
  000b2a3a: ldr.w r0,[r0,#0x80]
  000b2a3e: mov r4,r2
  000b2a40: str r1,[r5,#0x7c]
  000b2a42: cmp r0,r1
  000b2a44: mov r0,r5
  000b2a46: it lt
  000b2a48: str.w.lt r1,[r5,#0x80]
  000b2a4c: blx 0x00072df0
  000b2a50: str.w r4,[r5,#0xe4]
  000b2a54: pop {r4,r5,r7,pc}
```
