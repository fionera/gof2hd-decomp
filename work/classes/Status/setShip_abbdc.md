# Status::setShip
elf 0xabbdc body 34
Sig: undefined __stdcall setShip(Ship * param_1)

## decompile
```c

/* Status::setShip(Ship*) */

void Status::setShip(Ship *param_1)

{
  void *pvVar1;
  undefined4 in_r1;
  
  if (*(Ship **)(param_1 + 400) != (Ship *)0x0) {
    pvVar1 = (void *)Ship::~Ship(*(Ship **)(param_1 + 400));
    operator_delete(pvVar1);
    *(undefined4 *)(param_1 + 400) = 0;
  }
  *(undefined4 *)(param_1 + 400) = in_r1;
  return;
}

```
## target disasm
```
  000bbbdc: push {r4,r5,r7,lr}
  000bbbde: add r7,sp,#0x8
  000bbbe0: mov r5,r0
  000bbbe2: ldr.w r0,[r0,#0x190]
  000bbbe6: mov r4,r1
  000bbbe8: cbz r0,0x000bbbf8
  000bbbea: blx 0x00073468
  000bbbee: blx 0x0006eb48
  000bbbf2: movs r0,#0x0
  000bbbf4: str.w r0,[r5,#0x190]
  000bbbf8: str.w r4,[r5,#0x190]
  000bbbfc: pop {r4,r5,r7,pc}
```
