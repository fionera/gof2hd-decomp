# Player::getPosition
elf 0xa2858 body 12
Sig: undefined __stdcall getPosition(void)

## decompile
```c

/* Player::getPosition() */

void Player::getPosition(void)

{
  Matrix *in_r0;
  
  AbyssEngine::AEMath::MatrixGetPosition(in_r0);
  return;
}

```
## target disasm
```
  000b2858: push {r7,lr}
  000b285a: mov r7,sp
  000b285c: adds r1,#0x4
  000b285e: blx 0x0006f16c
  000b2862: pop {r7,pc}
```
