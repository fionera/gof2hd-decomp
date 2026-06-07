# PlayerEgo::isDead
elf 0x9b17c body 20
Sig: undefined __thiscall isDead(PlayerEgo * this)

## decompile
```c

/* PlayerEgo::isDead() */

bool __thiscall PlayerEgo::isDead(PlayerEgo *this)

{
  int iVar1;
  
  iVar1 = getHitpoints();
  return iVar1 < 1;
}

```

## target disasm
```
  000ab17c: push {r7,lr}
  000ab17e: mov r7,sp
  000ab180: blx 0x000724e4
  000ab184: movs r1,#0x0
  000ab186: cmp r0,#0x1
  000ab188: it lt
  000ab18a: mov.lt r1,#0x1
  000ab18c: mov r0,r1
  000ab18e: pop {r7,pc}
```
