# PaintCanvas::CheckNUseRefractFBO
elf 0x7d504 body 48
Sig: undefined __stdcall CheckNUseRefractFBO(bool param_1)

## decompile
```c

/* AbyssEngine::PaintCanvas::CheckNUseRefractFBO(bool) */

void AbyssEngine::PaintCanvas::CheckNUseRefractFBO(bool param_1)

{
  int iVar1;
  
  if (((**(char **)(DAT_0008d534 + 0x8d510) != '\0') &&
      (**(char **)(DAT_0008d538 + 0x8d51a) != '\0')) &&
     (iVar1 = Engine::IsPostEffectActivated(*(Engine **)(param_1 + 0x34)), iVar1 == 0)) {
    (*(code *)(DAT_001ab414 + 0x1ab418))(*(undefined4 *)(param_1 + 0x34));
    return;
  }
  return;
}

```

## target disasm
```
  0008d504: push {r4,r6,r7,lr}
  0008d506: add r7,sp,#0x8
  0008d508: mov r4,r0
  0008d50a: ldr r0,[0x0008d534]
  0008d50c: add r0,pc
  0008d50e: ldr r0,[r0,#0x0]
  0008d510: ldrb r0,[r0,#0x0]
  0008d512: cbz r0,0x0008d526
  0008d514: ldr r0,[0x0008d538]
  0008d516: add r0,pc
  0008d518: ldr r0,[r0,#0x0]
  0008d51a: ldrb r0,[r0,#0x0]
  0008d51c: cbz r0,0x0008d526
  0008d51e: ldr r0,[r4,#0x34]
  0008d520: blx 0x0006ee78
  0008d524: cbz r0,0x0008d528
  0008d526: pop {r4,r6,r7,pc}
  0008d528: ldr r0,[r4,#0x34]
  0008d52a: pop.w {r4,r6,r7,lr}
  0008d52e: b.w 0x001ab408
  001ab408: bx pc
```
