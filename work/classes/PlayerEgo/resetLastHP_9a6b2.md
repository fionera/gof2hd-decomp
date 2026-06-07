# PlayerEgo::resetLastHP
elf 0x9a6b2 body 18
Sig: undefined __stdcall resetLastHP(void)

## decompile
```c

/* PlayerEgo::resetLastHP() */

void PlayerEgo::resetLastHP(void)

{
  undefined4 *in_r0;
  undefined4 uVar1;
  
  uVar1 = Player::getCombinedHP((Player *)*in_r0);
  in_r0[0x4c] = uVar1;
  return;
}

```

## target disasm
```
  000aa6b2: push {r4,r6,r7,lr}
  000aa6b4: add r7,sp,#0x8
  000aa6b6: mov r4,r0
  000aa6b8: ldr r0,[r0,#0x0]
  000aa6ba: blx 0x0007225c
  000aa6be: str.w r0,[r4,#0x130]
  000aa6c2: pop {r4,r6,r7,pc}
```
