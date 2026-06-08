# AbyssEngine::SpriteSystemSetAllSize
elf 0x86db8 body 34
Sig: undefined __stdcall SpriteSystemSetAllSize(short param_1, SpriteSystem * param_2)

## decompile
```c

/* AbyssEngine::SpriteSystemSetAllSize(short, AbyssEngine::SpriteSystem*) */

void AbyssEngine::SpriteSystemSetAllSize(short param_1,SpriteSystem *param_2)

{
  uint uVar1;
  short *psVar2;
  
  if (param_2 != (SpriteSystem *)0x0) {
    psVar2 = *(short **)(param_2 + 8);
    if (param_2[0xc] != (SpriteSystem)0x0) {
      *psVar2 = param_1;
      return;
    }
    for (uVar1 = 0; uVar1 < *(ushort *)param_2; uVar1 = uVar1 + 1) {
      psVar2[uVar1] = param_1;
    }
  }
  return;
}

```

## target disasm
```
  00096db8: cbz r1,0x00096dd8
  00096dba: ldrb r3,[r1,#0xc]
  00096dbc: ldr.w r12,[r1,#0x8]
  00096dc0: cbz r3,0x00096dc8
  00096dc2: strh.w r0,[r12,#0x0]
  00096dc6: bx lr
  00096dc8: movs r3,#0x0
  00096dca: b 0x00096dd2
  00096dcc: strh.w r0,[r12,r3,lsl #0x1]
  00096dd0: adds r3,#0x1
  00096dd2: ldrh r2,[r1,#0x0]
  00096dd4: cmp r3,r2
  00096dd6: bcc 0x00096dcc
  00096dd8: bx lr
```
