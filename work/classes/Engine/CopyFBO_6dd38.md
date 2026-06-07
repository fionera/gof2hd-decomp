# Engine::CopyFBO
elf 0x6dd38 body 118
Sig: undefined __thiscall CopyFBO(Engine * this)

## decompile
```c

/* AbyssEngine::Engine::CopyFBO() */

void __thiscall AbyssEngine::Engine::CopyFBO(Engine *this)

{
  int iVar1;
  
  if (**(char **)(DAT_0007ddb0 + 0x7dd44) == '\0') {
    return;
  }
  iVar1 = IsPostEffectActivated(this);
  if (iVar1 == 0) {
    DeactivateRender2FracFBO();
    DrawCloakFBO((FBOContainer *)this);
    ActivateViewBuffer(this);
  }
  else {
    DeactivateRender2TextureFBO(this);
    DrawCloakFBO((FBOContainer *)this);
    ActivateRender2TextureFBO(this);
  }
  glEnable(0xb71);
  glDepthMask(1);
  glDisable(0xbe2);
  glClearColor(0,0,0,0);
  (*(code *)(DAT_001ab094 + 0x1ab098))(0x100);
  return;
}

```

## target disasm
```
  0007dd38: push {r4,r6,r7,lr}
  0007dd3a: add r7,sp,#0x8
  0007dd3c: mov r4,r0
  0007dd3e: ldr r0,[0x0007ddb0]
  0007dd40: add r0,pc
  0007dd42: ldr r0,[r0,#0x0]
  0007dd44: ldrb r0,[r0,#0x0]
  0007dd46: cbz r0,0x0007dd68
  0007dd48: mov r0,r4
  0007dd4a: blx 0x0006ee78
  0007dd4e: cbz r0,0x0007dd6a
  0007dd50: mov r0,r4
  0007dd52: blx 0x0006ee84
  0007dd56: ldr.w r1,[r4,#0x414]
  0007dd5a: mov r0,r4
  0007dd5c: blx 0x0006ee90
  0007dd60: mov r0,r4
  0007dd62: blx 0x0006ee9c
  0007dd66: b 0x0007dd80
  0007dd68: pop {r4,r6,r7,pc}
  0007dd6a: mov r0,r4
  0007dd6c: blx 0x0006eea8
  0007dd70: ldr.w r1,[r4,#0x418]
  0007dd74: mov r0,r4
  0007dd76: blx 0x0006ee90
  0007dd7a: mov r0,r4
  0007dd7c: blx 0x0006eeb4
  0007dd80: movw r0,#0xb71
  0007dd84: blx 0x0006edb8
  0007dd88: movs r0,#0x1
  0007dd8a: blx 0x0006eec0
  0007dd8e: movw r0,#0xbe2
  0007dd92: blx 0x0006edc4
  0007dd96: movs r0,#0x0
  0007dd98: movs r1,#0x0
  0007dd9a: movs r2,#0x0
  0007dd9c: movs r3,#0x0
  0007dd9e: blx 0x0006eecc
  0007dda2: mov.w r0,#0x100
  0007dda6: pop.w {r4,r6,r7,lr}
  0007ddaa: b.w 0x001ab088
```
