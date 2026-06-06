# Level::setPlayerRoute
elf 0xc0dc8 body 28
Sig: undefined __stdcall setPlayerRoute(Route * param_1)

## decompile
```c

/* Level::setPlayerRoute(Route*) */

void Level::setPlayerRoute(Route *param_1)

{
  void *pvVar1;
  undefined4 in_r1;
  
  if (*(Route **)(param_1 + 0x108) != (Route *)0x0) {
    pvVar1 = (void *)Route::~Route(*(Route **)(param_1 + 0x108));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(param_1 + 0x108) = in_r1;
  return;
}

```
## target disasm
```
  000d0dc8: push {r4,r5,r7,lr}
  000d0dca: add r7,sp,#0x8
  000d0dcc: mov r5,r0
  000d0dce: ldr.w r0,[r0,#0x108]
  000d0dd2: mov r4,r1
  000d0dd4: cbz r0,0x000d0dde
  000d0dd6: blx 0x00072310
  000d0dda: blx 0x0006eb48
  000d0dde: str.w r4,[r5,#0x108]
  000d0de2: pop {r4,r5,r7,pc}
```
