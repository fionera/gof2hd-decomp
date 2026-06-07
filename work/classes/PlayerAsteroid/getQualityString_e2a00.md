# PlayerAsteroid::getQualityString
elf 0xe2a00 body 46
Sig: undefined __stdcall getQualityString(void)

## decompile
```c

/* PlayerAsteroid::getQualityString() */

void PlayerAsteroid::getQualityString(void)

{
  String *in_r0;
  int in_r1;
  uint uVar1;
  char *pcVar2;
  
  uVar1 = *(int *)(in_r1 + 0x150) - 5;
  if (uVar1 < 3) {
    pcVar2 = *(char **)(DAT_000f2a38 + 0xf2a10 + uVar1 * 4);
  }
  else {
    pcVar2 = (char *)(DAT_000f2a34 + 0xf2a20);
    if (*(int *)(in_r1 + 0x150) != 4) {
      pcVar2 = (char *)(DAT_000f2a30 + 0xf2a1e);
    }
  }
  AbyssEngine::String::String(in_r0,pcVar2,false);
  return;
}

```

## target disasm
```
  000f2a00: ldr.w r2,[r1,#0x150]
  000f2a04: subs r1,r2,#0x5
  000f2a06: cmp r1,#0x3
  000f2a08: bcs 0x000f2a14
  000f2a0a: ldr r2,[0x000f2a38]
  000f2a0c: add r2,pc
  000f2a0e: ldr.w r1,[r2,r1,lsl #0x2]
  000f2a12: b 0x000f2a22
  000f2a14: ldr r3,[0x000f2a30]
  000f2a16: cmp r2,#0x4
  000f2a18: ldr r1,[0x000f2a34]
  000f2a1a: add r3,pc
  000f2a1c: add r1,pc
  000f2a1e: it ne
  000f2a20: mov.ne r1,r3
  000f2a22: push {r7,lr}
  000f2a24: mov r7,sp
  000f2a26: movs r2,#0x0
  000f2a28: blx 0x0006ee18
  000f2a2c: pop {r7,pc}
```
