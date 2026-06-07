# Engine::SetFrameBufferTexture
elf 0x84aa0 body 102
Sig: undefined __thiscall SetFrameBufferTexture(Engine * this, int param_1, int param_2)

## decompile
```c

/* AbyssEngine::Engine::SetFrameBufferTexture(int, int) */

void __thiscall AbyssEngine::Engine::SetFrameBufferTexture(Engine *this,int param_1,int param_2)

{
  Engine *pEVar1;
  bool bVar2;
  
  if (*(int *)(this + param_1 * 4 + 0x48c) != -1) {
    glActiveTexture(0x84c0);
    glBindTexture(0xde1,*(undefined4 *)(this + param_1 * 4 + 0x48c));
  }
  bVar2 = param_2 == -1;
  pEVar1 = (Engine *)0x0;
  if (!bVar2) {
    pEVar1 = this + param_2 * 4;
    bVar2 = *(int *)(pEVar1 + 0x48c) == -1;
  }
  if (bVar2) {
    return;
  }
  glActiveTexture(0x84c1);
  (*(code *)(DAT_001ab484 + 0x1ab488))(0xde1,*(undefined4 *)(pEVar1 + 0x48c));
  return;
}

```

## target disasm
```
  00094aa0: push {r4,r5,r6,r7,lr}
  00094aa2: add r7,sp,#0xc
  00094aa4: push.w r11
  00094aa8: mov r5,r0
  00094aaa: add.w r0,r0,r1, lsl #0x2
  00094aae: mov r4,r2
  00094ab0: ldr.w r1,[r0,#0x48c]
  00094ab4: adds r1,#0x1
  00094ab6: beq 0x00094ace
  00094ab8: addw r6,r0,#0x48c
  00094abc: movw r0,#0x84c0
  00094ac0: blx 0x0006fb8c
  00094ac4: ldr r1,[r6,#0x0]
  00094ac6: movw r0,#0xde1
  00094aca: blx 0x0006f244
  00094ace: adds r0,r4,#0x1
  00094ad0: ittt ne
  00094ad2: add.ne.w r0,r5,r4, lsl #0x2
  00094ad6: ldr.ne.w r1,[r0,#0x48c]
  00094ada: adds.ne.w r1,r1,#0x1
  00094ade: bne 0x00094ae6
  00094ae0: pop.w r11
  00094ae4: pop {r4,r5,r6,r7,pc}
  00094ae6: addw r4,r0,#0x48c
  00094aea: movw r0,#0x84c1
  00094aee: blx 0x0006fb8c
  00094af2: ldr r1,[r4,#0x0]
  00094af4: movw r0,#0xde1
  00094af8: pop.w r11
  00094afc: pop.w {r4,r5,r6,r7,lr}
  00094b00: b.w 0x001ab478
  001ab478: bx pc
```
