# Hud::sameHudEventAsBeforeAggregate
elf 0x163200 body 62
Sig: undefined __stdcall sameHudEventAsBeforeAggregate(String param_1)

## decompile
```c

/* Hud::sameHudEventAsBeforeAggregate(AbyssEngine::String) */

int Hud::sameHudEventAsBeforeAggregate(int param_1,String *param_2)

{
  int iVar1;
  int iVar2;
  
  iVar1 = **(int **)(param_1 + 0x264);
  do {
    iVar1 = iVar1 + -1;
    if (iVar1 < 1) {
      return -1;
    }
    iVar2 = *(int *)(*(int *)(*(int *)(param_1 + 0x264) + 4) + iVar1 * 4);
  } while ((iVar2 == 0) ||
          (iVar2 = AbyssEngine::String::Compare(*(String **)(iVar2 + 0x1c),param_2), iVar2 != 0));
  return iVar1;
}

```

## target disasm
```
  00173200: push {r4,r5,r6,r7,lr}
  00173202: add r7,sp,#0xc
  00173204: push.w r11
  00173208: mov r6,r0
  0017320a: ldr.w r0,[r0,#0x264]
  0017320e: mov r5,r1
  00173210: ldr r0,[r0,#0x0]
  00173212: subs r4,r0,#0x1
  00173214: b 0x0017322e
  00173216: ldr.w r0,[r6,#0x264]
  0017321a: ldr r0,[r0,#0x4]
  0017321c: ldr.w r0,[r0,r4,lsl #0x2]
  00173220: cbz r0,0x0017322c
  00173222: ldr r0,[r0,#0x1c]
  00173224: mov r1,r5
  00173226: blx 0x0006f664
  0017322a: cbz r0,0x00173236
  0017322c: subs r4,#0x1
  0017322e: cmp r4,#0x1
  00173230: bge 0x00173216
  00173232: mov.w r4,#0xffffffff
  00173236: mov r0,r4
  00173238: pop.w r11
  0017323c: pop {r4,r5,r6,r7,pc}
```
