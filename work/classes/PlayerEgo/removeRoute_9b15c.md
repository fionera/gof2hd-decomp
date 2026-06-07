# PlayerEgo::removeRoute
elf 0x9b15c body 28
Sig: undefined __stdcall removeRoute(void)

## decompile
```c

/* PlayerEgo::removeRoute() */

void PlayerEgo::removeRoute(void)

{
  int in_r0;
  void *pvVar1;
  
  if (*(Route **)(in_r0 + 0xfc) != (Route *)0x0) {
    pvVar1 = (void *)Route::~Route(*(Route **)(in_r0 + 0xfc));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(in_r0 + 0xfc) = 0;
  return;
}

```

## target disasm
```
  000ab15c: push {r4,r6,r7,lr}
  000ab15e: add r7,sp,#0x8
  000ab160: mov r4,r0
  000ab162: ldr.w r0,[r0,#0xfc]
  000ab166: cbz r0,0x000ab170
  000ab168: blx 0x00072310
  000ab16c: blx 0x0006eb48
  000ab170: movs r0,#0x0
  000ab172: str.w r0,[r4,#0xfc]
  000ab176: pop {r4,r6,r7,pc}
```
