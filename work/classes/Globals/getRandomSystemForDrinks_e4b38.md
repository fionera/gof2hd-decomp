# Globals::getRandomSystemForDrinks
elf 0xe4b38 body 40
Sig: undefined __stdcall getRandomSystemForDrinks(void)

## decompile
```c

/* Globals::getRandomSystemForDrinks() */

void Globals::getRandomSystemForDrinks(void)

{
  undefined4 uVar1;
  undefined4 uVar2;
  
  uVar2 = **(undefined4 **)(DAT_000f4b60 + 0xf4b44);
  uVar1 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_000f4b64 + 0xf4b46));
  (*(code *)(DAT_001ac2e4 + 0x1ac2e8))(uVar2,uVar1);
  return;
}

```

## target disasm
```
  000f4b38: push {r4,r6,r7,lr}
  000f4b3a: add r7,sp,#0x8
  000f4b3c: ldr r0,[0x000f4b60]
  000f4b3e: ldr r1,[0x000f4b64]
  000f4b40: add r0,pc
  000f4b42: add r1,pc
  000f4b44: ldr r0,[r0,#0x0]
  000f4b46: ldr r1,[r1,#0x0]
  000f4b48: ldr r4,[r0,#0x0]
  000f4b4a: ldr r0,[r1,#0x0]
  000f4b4c: movs r1,#0x16
  000f4b4e: blx 0x00071848
  000f4b52: mov r1,r0
  000f4b54: mov r0,r4
  000f4b56: pop.w {r4,r6,r7,lr}
  000f4b5a: b.w 0x001ac2d8
  001ac2d8: bx pc
```
