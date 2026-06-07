# PlayerEgo::driveReady
elf 0x9b03a body 14
Sig: undefined __thiscall driveReady(PlayerEgo * this)

## decompile
```c

/* PlayerEgo::driveReady() */

bool __thiscall PlayerEgo::driveReady(PlayerEgo *this)

{
  return (int)this[0xe].field_8 <= (int)this[0xe].field_4;
}

```

## target disasm
```
  000ab03a: ldrd r1,r2,[r0,#0x1fc]
  000ab03e: movs r0,#0x0
  000ab040: cmp r1,r2
  000ab042: it ge
  000ab044: mov.ge r0,#0x1
  000ab046: bx lr
```
