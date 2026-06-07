# Hud::releaseAllKeys
elf 0x161440 body 52
Sig: undefined __stdcall releaseAllKeys(void)

## decompile
```c

/* Hud::releaseAllKeys() */

void Hud::releaseAllKeys(void)

{
  int in_r0;
  int iVar1;
  int iVar2;
  
  *(undefined4 *)(in_r0 + 0x284) = 0;
  for (iVar1 = 0; iVar1 != 0x19; iVar1 = iVar1 + 1) {
    if ((*(int *)(in_r0 + 0x28c) != 0) &&
       (iVar2 = *(int *)(*(int *)(in_r0 + 0x28c) + 4), *(int *)(iVar2 + iVar1 * 4) != 0)) {
      *(undefined4 *)(iVar2 + iVar1 * 4) = 0;
    }
    *(undefined4 *)(*(int *)(in_r0 + 0x290) + iVar1 * 4) = 0;
  }
  *(undefined4 *)(in_r0 + 0x288) = 0;
  return;
}

```

## target disasm
```
  00171440: mov.w r12,#0x0
  00171444: movs r2,#0x0
  00171446: str.w r12,[r0,#0x284]
  0017144a: b 0x00171468
  0017144c: ldr.w r3,[r0,#0x28c]
  00171450: cbz r3,0x0017145e
  00171452: ldr r3,[r3,#0x4]
  00171454: ldr.w r1,[r3,r2,lsl #0x2]
  00171458: cbz r1,0x0017145e
  0017145a: str.w r12,[r3,r2,lsl #0x2]
  0017145e: ldr.w r1,[r0,#0x290]
  00171462: str.w r12,[r1,r2,lsl #0x2]
  00171466: adds r2,#0x1
  00171468: cmp r2,#0x19
  0017146a: bne 0x0017144c
  0017146c: movs r1,#0x0
  0017146e: str.w r1,[r0,#0x288]
  00171472: bx lr
```
