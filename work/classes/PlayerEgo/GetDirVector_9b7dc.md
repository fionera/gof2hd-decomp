# PlayerEgo::GetDirVector
elf 0x9b7dc body 12
Sig: undefined __stdcall GetDirVector(void)

## decompile
```c

/* PlayerEgo::GetDirVector() */

void PlayerEgo::GetDirVector(void)

{
  AEGeometry::getDirection();
  return;
}

```

## target disasm
```
  000ab7dc: push {r7,lr}
  000ab7de: mov r7,sp
  000ab7e0: ldr r1,[r1,#0x8]
  000ab7e2: blx 0x00072574
  000ab7e6: pop {r7,pc}
```
