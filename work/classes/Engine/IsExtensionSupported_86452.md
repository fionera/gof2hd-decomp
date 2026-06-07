# Engine::IsExtensionSupported
elf 0x86452 body 110
Sig: undefined __thiscall IsExtensionSupported(Engine * this, char * param_1)

## decompile
```c

/* AbyssEngine::Engine::IsExtensionSupported(char const*) */

undefined4 __thiscall AbyssEngine::Engine::IsExtensionSupported(Engine *this,char *param_1)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  uint unaff_r5;
  
  iVar1 = glGetString(0x1f03);
  uVar5 = 0;
  do {
    uVar2 = uVar5;
    uVar5 = uVar2 + 1;
  } while (*(char *)(iVar1 + uVar2) != '\0');
  uVar5 = 0;
  do {
    uVar3 = uVar5;
    uVar5 = uVar3 + 1;
  } while (param_1[uVar3] != '\0');
  uVar5 = 0;
  do {
    if (uVar2 <= uVar5) {
      return 0;
    }
    if (*(char *)(iVar1 + uVar5) == *param_1) {
      uVar4 = 0;
      while( true ) {
        if (uVar3 > uVar4) {
          unaff_r5 = uVar5 + uVar4;
        }
        if ((uVar3 <= uVar4 || uVar2 <= unaff_r5) ||
           (unaff_r5 = (uint)*(byte *)(iVar1 + uVar5 + uVar4), (byte)param_1[uVar4] != unaff_r5))
        break;
        uVar4 = uVar4 + 1;
        if (uVar3 == uVar4) {
          return 1;
        }
      }
    }
    uVar5 = uVar5 + 1;
  } while( true );
}

```

## target disasm
```
  00096452: push {r4,r5,r6,r7,lr}
  00096454: add r7,sp,#0xc
  00096456: push.w r11
  0009645a: movw r0,#0x1f03
  0009645e: mov r4,r1
  00096460: blx 0x00070528
  00096464: movs r1,#0x0
  00096466: ldrb r2,[r0,r1]
  00096468: adds r1,#0x1
  0009646a: cmp r2,#0x0
  0009646c: bne 0x00096466
  0009646e: sub.w r12,r1,#0x1
  00096472: movs r1,#0x0
  00096474: ldrb r2,[r4,r1]
  00096476: adds r1,#0x1
  00096478: cmp r2,#0x0
  0009647a: bne 0x00096474
  0009647c: subs r2,r1,#0x1
  0009647e: movs r3,#0x0
  00096480: b 0x000964b0
  00096482: ldrb.w lr,[r0,r3]
  00096486: ldrb r1,[r4,#0x0]
  00096488: cmp lr,r1
  0009648a: bne 0x000964ae
  0009648c: add.w lr,r0,r3
  00096490: movs r1,#0x0
  00096492: cmp r1,r2
  00096494: itt cc
  00096496: add.cc r5,r3,r1
  00096498: cmp.cc r5,r12
  0009649a: bcs 0x000964ae
  0009649c: ldrb.w r5,[lr,r1]
  000964a0: ldrb r6,[r4,r1]
  000964a2: cmp r6,r5
  000964a4: bne 0x000964ae
  000964a6: adds r1,#0x1
  000964a8: cmp r2,r1
  000964aa: bne 0x00096492
  000964ac: b 0x000964b8
  000964ae: adds r3,#0x1
  000964b0: cmp r3,r12
  000964b2: bcc 0x00096482
  000964b4: movs r0,#0x0
  000964b6: b 0x000964ba
  000964b8: movs r0,#0x1
  000964ba: pop.w r11
  000964be: pop {r4,r5,r6,r7,pc}
```
