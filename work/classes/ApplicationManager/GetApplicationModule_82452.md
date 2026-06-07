# ApplicationManager::GetApplicationModule
elf 0x82452 body 36
Sig: undefined __stdcall GetApplicationModule(uint param_1)

## decompile
```c

/* AbyssEngine::ApplicationManager::GetApplicationModule(unsigned int) */

undefined4 AbyssEngine::ApplicationManager::GetApplicationModule(uint param_1)

{
  int in_r1;
  uint uVar1;
  
  uVar1 = 0;
  while( true ) {
    if (*(uint *)(param_1 + 0x50) <= uVar1) {
      return 0;
    }
    if (*(int *)(*(int *)(param_1 + 0x54) + uVar1 * 4) == in_r1) break;
    uVar1 = uVar1 + 1;
  }
  return *(undefined4 *)(*(int *)(param_1 + 0x48) + uVar1 * 4);
}

```

## target disasm
```
  00092452: ldr.w r12,[r0,#0x50]
  00092456: movs r2,#0x0
  00092458: b 0x0009245c
  0009245a: adds r2,#0x1
  0009245c: cmp r2,r12
  0009245e: itt cs
  00092460: mov.cs r0,#0x0
  00092462: bx.cs lr
  00092464: ldr r3,[r0,#0x54]
  00092466: ldr.w r3,[r3,r2,lsl #0x2]
  0009246a: cmp r3,r1
  0009246c: bne 0x0009245a
  0009246e: ldr r0,[r0,#0x48]
  00092470: ldr.w r0,[r0,r2,lsl #0x2]
  00092474: bx lr
```
