# Generator::computerTradeGoods
elf 0x953a0 body 96
Sig: undefined __stdcall computerTradeGoods(Station * param_1)

## decompile
```c

/* Generator::computerTradeGoods(Station*) */

void Generator::computerTradeGoods(Station *param_1)

{
  int iVar1;
  uint *puVar2;
  int iVar3;
  Station *in_r1;
  uint uVar4;
  int *piVar5;
  
  iVar1 = Station::getIndex(in_r1);
  if ((iVar1 != 0x6c) && (puVar2 = (uint *)Station::getItems(), puVar2 != (uint *)0x0)) {
    piVar5 = *(int **)(DAT_000a5400 + 0xa53c6);
    for (uVar4 = 0; uVar4 < *puVar2; uVar4 = uVar4 + 1) {
      iVar1 = Item::getAmount();
      iVar3 = AbyssEngine::AERandom::nextInt(*piVar5);
      if (iVar3 < iVar1) {
        Item::changeAmount(*(Item **)(puVar2[1] + uVar4 * 4),-iVar3);
      }
    }
  }
  return;
}

```

## target disasm
```
  000a53a0: push {r4,r5,r6,r7,lr}
  000a53a2: add r7,sp,#0xc
  000a53a4: push.w r8
  000a53a8: mov r0,r1
  000a53aa: mov r4,r1
  000a53ac: blx 0x00071824
  000a53b0: cmp r0,#0x6c
  000a53b2: beq 0x000a53fa
  000a53b4: mov r0,r4
  000a53b6: blx 0x00071830
  000a53ba: mov r4,r0
  000a53bc: cbz r0,0x000a53fa
  000a53be: ldr r0,[0x000a5400]
  000a53c0: movs r6,#0x0
  000a53c2: add r0,pc
  000a53c4: ldr.w r8,[r0,#0x0]
  000a53c8: b 0x000a53f4
  000a53ca: ldr r0,[r4,#0x4]
  000a53cc: ldr.w r0,[r0,r6,lsl #0x2]
  000a53d0: blx 0x0007183c
  000a53d4: mov r5,r0
  000a53d6: ldr.w r0,[r8,#0x0]
  000a53da: movs r1,#0x3
  000a53dc: blx 0x00071848
  000a53e0: cmp r0,r5
  000a53e2: bge 0x000a53f2
  000a53e4: ldr r1,[r4,#0x4]
  000a53e6: ldr.w r2,[r1,r6,lsl #0x2]
  000a53ea: rsbs r1,r0
  000a53ec: mov r0,r2
  000a53ee: blx 0x00071854
  000a53f2: adds r6,#0x1
  000a53f4: ldr r0,[r4,#0x0]
  000a53f6: cmp r6,r0
  000a53f8: bcc 0x000a53ca
  000a53fa: pop.w r8
  000a53fe: pop {r4,r5,r6,r7,pc}
```
