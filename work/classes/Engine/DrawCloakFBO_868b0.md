# Engine::DrawCloakFBO
elf 0x868b0 body 36
Sig: undefined __stdcall DrawCloakFBO(FBOContainer * param_1)

## decompile
```c

/* AbyssEngine::Engine::DrawCloakFBO(AbyssEngine::FBOContainer*) */

void AbyssEngine::Engine::DrawCloakFBO(FBOContainer *param_1)

{
  if (**(char **)(DAT_000968d4 + 0x968b8) != '\0') {
                    /* WARNING: Could not recover jumptable at 0x000968d0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (**(code **)(**(int **)(*(int *)(param_1 + 0x514) + **(int **)(DAT_000968d8 + 0x968c6) * 4) +
                0x14))();
    return;
  }
  return;
}

```

## target disasm
```
  000968b0: mov r2,r0
  000968b2: ldr r0,[0x000968d4]
  000968b4: add r0,pc
  000968b6: ldr r0,[r0,#0x0]
  000968b8: ldrb r0,[r0,#0x0]
  000968ba: cbz r0,0x000968d2
  000968bc: ldr r0,[0x000968d8]
  000968be: ldr.w r3,[r2,#0x514]
  000968c2: add r0,pc
  000968c4: ldr r0,[r0,#0x0]
  000968c6: ldr r0,[r0,#0x0]
  000968c8: ldr.w r0,[r3,r0,lsl #0x2]
  000968cc: ldr r3,[r0,#0x0]
  000968ce: ldr r3,[r3,#0x14]
  000968d0: bx r3
  000968d2: bx lr
```
