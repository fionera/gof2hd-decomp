# PlayerEgo::aboutToReachAutoTarget
elf 0xa15ee body 6
Sig: undefined __thiscall aboutToReachAutoTarget(PlayerEgo * this)

## decompile
```c

/* PlayerEgo::aboutToReachAutoTarget() */

undefined1 __thiscall PlayerEgo::aboutToReachAutoTarget(PlayerEgo *this)

{
  return *(undefined1 *)&this[0x16].field_18;
}

```

## target disasm
```
  000b15ee: ldrb.w r0,[r0,#0x330]
  000b15f2: bx lr
```
