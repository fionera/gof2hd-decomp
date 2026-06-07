# KIPlayer::setRoute
elf 0xa6150 body 36
Sig: undefined __thiscall setRoute(KIPlayer * this, Route * param_1)

## decompile
```c

/* KIPlayer::setRoute(Route*) */

void __thiscall KIPlayer::setRoute(KIPlayer *this,Route *param_1)

{
  void *pvVar1;
  
  if (*(Route **)(this + 0x6c) != (Route *)0x0) {
    pvVar1 = (void *)Route::~Route(*(Route **)(this + 0x6c));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x6c) = 0;
  if (param_1 != (Route *)0x0) {
    *(Route **)(this + 0xb4) = param_1;
  }
  *(Route **)(this + 0x6c) = param_1;
  return;
}

```

## target disasm
```
  000b6150: push {r4,r5,r7,lr}
  000b6152: add r7,sp,#0x8
  000b6154: mov r5,r0
  000b6156: ldr r0,[r0,#0x6c]
  000b6158: mov r4,r1
  000b615a: cbz r0,0x000b6164
  000b615c: blx 0x00072310
  000b6160: blx 0x0006eb48
  000b6164: movs r0,#0x0
  000b6166: cmp r4,#0x0
  000b6168: str r0,[r5,#0x6c]
  000b616a: beq 0x000b6170
  000b616c: str.w r4,[r5,#0xb4]
  000b6170: str r4,[r5,#0x6c]
  000b6172: pop {r4,r5,r7,pc}
```
