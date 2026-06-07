# ImageFactory::createChar
elf 0x11c6cc body 34
Sig: undefined __stdcall createChar(int param_1)

## decompile
```c

/* ImageFactory::createChar(int) */

void ImageFactory::createChar(int param_1)

{
  int iVar1;
  
  iVar1 = AbyssEngine::AERandom::nextInt(**(int **)(DAT_0012c6f0 + 0x12c6da));
  createChar(SUB41(LZCOUNT(iVar1),0),(uint)(iVar1 == 0));
  return;
}

```

## target disasm
```
  0012c6cc: push {r4,r6,r7,lr}
  0012c6ce: add r7,sp,#0x8
  0012c6d0: ldr r0,[0x0012c6f0]
  0012c6d2: mov r4,r1
  0012c6d4: movs r1,#0x2
  0012c6d6: add r0,pc
  0012c6d8: ldr r0,[r0,#0x0]
  0012c6da: ldr r0,[r0,#0x0]
  0012c6dc: blx 0x00071848
  0012c6e0: clz r0,r0
  0012c6e4: mov r2,r4
  0012c6e6: lsrs r1,r0,#0x5
  0012c6e8: blx 0x00071b48
  0012c6ec: pop {r4,r6,r7,pc}
```
