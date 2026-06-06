# Player::pitchAllPrimaryGuns
elf 0xa433a body 38
Sig: undefined __thiscall pitchAllPrimaryGuns(Player * this, float param_1)

## decompile
```c

/* Player::pitchAllPrimaryGuns(float) */

void __thiscall Player::pitchAllPrimaryGuns(Player *this,float param_1)

{
  int *piVar1;
  undefined4 in_r1;
  int iVar2;
  int iVar3;
  
  if ((*(int *)this != 0) && (piVar1 = (int *)**(int **)(*(int *)this + 4), piVar1 != (int *)0x0)) {
    iVar2 = *piVar1;
    for (iVar3 = 0; iVar2 != iVar3; iVar3 = iVar3 + 1) {
      *(undefined4 *)(*(int *)(piVar1[1] + iVar3 * 4) + 0xb0) = in_r1;
    }
  }
  return;
}

```
## target disasm
```
  000b433a: ldr r0,[r0,#0x0]
  000b433c: cbz r0,0x000b435e
  000b433e: ldr r0,[r0,#0x4]
  000b4340: ldr r0,[r0,#0x0]
  000b4342: cbz r0,0x000b435e
  000b4344: vmov s0,r1
  000b4348: ldr r1,[r0,#0x0]
  000b434a: movs r2,#0x0
  000b434c: b 0x000b435a
  000b434e: ldr r3,[r0,#0x4]
  000b4350: ldr.w r3,[r3,r2,lsl #0x2]
  000b4354: adds r2,#0x1
  000b4356: vstr.32 s0,[r3,#0xb0]
  000b435a: cmp r1,r2
  000b435c: bne 0x000b434e
  000b435e: bx lr
```
