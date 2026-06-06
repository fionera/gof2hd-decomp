# Level::wingmanDied
elf 0xc4484 body 104
Sig: undefined __stdcall wingmanDied(String * param_1)

## decompile
```c

/* Level::wingmanDied(AbyssEngine::String const&) */

void Level::wingmanDied(String *param_1)

{
  uint *puVar1;
  uint uVar2;
  int iVar3;
  String *in_r1;
  undefined4 *puVar4;
  uint uVar5;
  
  puVar4 = *(undefined4 **)(DAT_000d44e8 + 0xd4494);
  puVar1 = (uint *)Status::getWingmen();
  if (puVar1 != (uint *)0x0) {
    uVar2 = *puVar1;
    if (uVar2 < 2) {
      (*(code *)(DAT_001abfb4 + 0x1abfb8))(*puVar4,0);
      return;
    }
    for (uVar5 = 0; uVar5 < uVar2; uVar5 = uVar5 + 1) {
      iVar3 = AbyssEngine::String::Compare(*(String **)(puVar1[1] + uVar5 * 4),in_r1);
      if (iVar3 == 0) {
        (*(code *)(DAT_001abfc4 + 0x1abfc8))(*(undefined4 *)(puVar1[1] + uVar5 * 4),puVar1);
        return;
      }
      uVar2 = *puVar1;
    }
  }
  return;
}

```
## target disasm
```
  000d4484: push {r4,r5,r6,r7,lr}
  000d4486: add r7,sp,#0xc
  000d4488: push.w r11
  000d448c: ldr r0,[0x000d44e8]
  000d448e: mov r4,r1
  000d4490: add r0,pc
  000d4492: ldr r6,[r0,#0x0]
  000d4494: ldr r0,[r6,#0x0]
  000d4496: blx 0x000741dc
  000d449a: mov r5,r0
  000d449c: cbz r0,0x000d44be
  000d449e: ldr r0,[r5,#0x0]
  000d44a0: cmp r0,#0x2
  000d44a2: bcc 0x000d44c4
  000d44a4: movs r6,#0x0
  000d44a6: b 0x000d44ba
  000d44a8: ldr r0,[r5,#0x4]
  000d44aa: mov r1,r4
  000d44ac: ldr.w r0,[r0,r6,lsl #0x2]
  000d44b0: blx 0x0006f664
  000d44b4: cbz r0,0x000d44d4
  000d44b6: ldr r0,[r5,#0x0]
  000d44b8: adds r6,#0x1
  000d44ba: cmp r6,r0
  000d44bc: bcc 0x000d44a8
  000d44be: pop.w r11
  000d44c2: pop {r4,r5,r6,r7,pc}
  000d44c4: ldr r0,[r6,#0x0]
  000d44c6: movs r1,#0x0
  000d44c8: pop.w r11
  000d44cc: pop.w {r4,r5,r6,r7,lr}
  000d44d0: b.w 0x001abfa8
  000d44d4: ldr r0,[r5,#0x4]
  000d44d6: mov r1,r5
  000d44d8: ldr.w r0,[r0,r6,lsl #0x2]
  000d44dc: pop.w r11
  000d44e0: pop.w {r4,r5,r6,r7,lr}
  000d44e4: b.w 0x001abfb8
  001abfa8: bx pc
  001abfb8: bx pc
```
