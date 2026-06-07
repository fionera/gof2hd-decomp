# KIPlayer::setInitActive
elf 0xa5e4c body 18
Sig: undefined __stdcall setInitActive(bool param_1)

## decompile
```c

/* KIPlayer::setInitActive(bool) */

void KIPlayer::setInitActive(bool param_1)

{
  setActive(param_1);
  *(undefined1 *)(param_1 + 0xb2) = 0;
  return;
}

```

## target disasm
```
  000b5e4c: push {r4,r6,r7,lr}
  000b5e4e: add r7,sp,#0x8
  000b5e50: mov r4,r0
  000b5e52: blx 0x000732f4
  000b5e56: movs r0,#0x0
  000b5e58: strb.w r0,[r4,#0xb2]
  000b5e5c: pop {r4,r6,r7,pc}
```
