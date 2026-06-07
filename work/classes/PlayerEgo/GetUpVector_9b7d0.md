# PlayerEgo::GetUpVector
elf 0x9b7d0 body 12
Sig: undefined __stdcall GetUpVector(void)

## decompile
```c

/* PlayerEgo::GetUpVector() */

void PlayerEgo::GetUpVector(void)

{
  AEGeometry::getUpVector();
  return;
}

```

## target disasm
```
  000ab7d0: push {r7,lr}
  000ab7d2: mov r7,sp
  000ab7d4: ldr r1,[r1,#0x8]
  000ab7d6: blx 0x00072568
  000ab7da: pop {r7,pc}
```
