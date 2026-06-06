# Player::GetEngineEvent
elf 0xa4108 body 6
Sig: undefined __thiscall GetEngineEvent(Player * this)

## decompile
```c

/* Player::GetEngineEvent() */

undefined4 __thiscall Player::GetEngineEvent(Player *this)

{
  return *(undefined4 *)(this + 0xf0);
}

```
## target disasm
```
  000b4108: ldr.w r0,[r0,#0xf0]
  000b410c: bx lr
```
