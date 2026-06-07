# Layout::initTip
elf 0xd495c body 148
Sig: undefined __stdcall initTip(void)

## decompile
```c

/* Layout::initTip() */

void Layout::initTip(void)

{
  int in_r0;
  void *pvVar1;
  uint *puVar2;
  String *pSVar3;
  undefined4 *puVar4;
  undefined4 *puVar5;
  undefined4 *puVar6;
  int iVar7;
  PaintCanvas *this;
  uint uVar8;
  
  if (*(Array **)(in_r0 + 0x3c8) != (Array *)0x0) {
    ArrayReleaseClasses<AbyssEngine::String*>(*(Array **)(in_r0 + 0x3c8));
    if (*(Array<AbyssEngine::String*> **)(in_r0 + 0x3c8) != (Array<AbyssEngine::String*> *)0x0) {
      pvVar1 = (void *)Array<AbyssEngine::String*>::~Array
                                 (*(Array<AbyssEngine::String*> **)(in_r0 + 0x3c8));
      operator_delete(pvVar1);
    }
    *(undefined4 *)(in_r0 + 0x3c8) = 0;
  }
  pvVar1 = operator_new(0xc);
  Array<AbyssEngine::String*>::Array();
  puVar4 = (undefined4 *)(DAT_000e4a04 + 0xe49a0);
  puVar5 = (undefined4 *)(DAT_000e4a08 + 0xe49a2);
  puVar2 = *(uint **)(DAT_000e4a00 + 0xe499c);
  puVar6 = (undefined4 *)(DAT_000e4a0c + 0xe49a6);
  *(void **)(in_r0 + 0x3c8) = pvVar1;
  uVar8 = *puVar2;
  iVar7 = *(int *)*puVar4;
  this = *(PaintCanvas **)*puVar6;
  AbyssEngine::AERandom::nextInt(*(int *)*puVar5);
  pSVar3 = (String *)GameText::getText(iVar7);
  AbyssEngine::PaintCanvas::GetLineArray
            (this,uVar8,pSVar3,
             *(int *)(**(int **)(DAT_000e4a10 + 0xe49d4) + 0x78) +
             *(int *)(**(int **)(DAT_000e4a10 + 0xe49d4) + 0x4c) * -2,*(Array **)(in_r0 + 0x3c8));
  return;
}

```

## target disasm
```
  000e495c: push {r4,r5,r6,r7,lr}
  000e495e: add r7,sp,#0xc
  000e4960: push.w r8
  000e4964: sub sp,#0x8
  000e4966: mov r4,r0
  000e4968: ldr.w r0,[r0,#0x3c8]
  000e496c: cbz r0,0x000e4986
  000e496e: blx 0x0006facc
  000e4972: ldr.w r0,[r4,#0x3c8]
  000e4976: cbz r0,0x000e4980
  000e4978: blx 0x0006f64c
  000e497c: blx 0x0006eb48
  000e4980: movs r0,#0x0
  000e4982: str.w r0,[r4,#0x3c8]
  000e4986: movs r0,#0xc
  000e4988: blx 0x0006eb24
  000e498c: mov r5,r0
  000e498e: blx 0x0006f628
  000e4992: ldr r0,[0x000e4a00]
  000e4994: ldr r1,[0x000e4a04]
  000e4996: ldr r2,[0x000e4a08]
  000e4998: add r0,pc
  000e499a: ldr r3,[0x000e4a0c]
  000e499c: add r1,pc
  000e499e: add r2,pc
  000e49a0: ldr r0,[r0,#0x0]
  000e49a2: add r3,pc
  000e49a4: str.w r5,[r4,#0x3c8]
  000e49a8: ldr r2,[r2,#0x0]
  000e49aa: ldr r1,[r1,#0x0]
  000e49ac: ldr r3,[r3,#0x0]
  000e49ae: ldr.w r8,[r0,#0x0]
  000e49b2: ldr r0,[r2,#0x0]
  000e49b4: ldr r5,[r1,#0x0]
  000e49b6: movs r1,#0xd
  000e49b8: ldr r6,[r3,#0x0]
  000e49ba: blx 0x00071848
  000e49be: add.w r1,r0,#0x154
  000e49c2: mov r0,r5
  000e49c4: blx 0x00072f70
  000e49c8: mov r2,r0
  000e49ca: ldr r0,[0x000e4a10]
  000e49cc: ldr.w r1,[r4,#0x3c8]
  000e49d0: add r0,pc
  000e49d2: ldr r0,[r0,#0x0]
  000e49d4: ldr r0,[r0,#0x0]
  000e49d6: ldr r3,[r0,#0x4c]
  000e49d8: ldr r0,[r0,#0x78]
  000e49da: str r1,[sp,#0x0]
  000e49dc: mov r1,r8
  000e49de: sub.w r3,r0,r3, lsl #0x1
  000e49e2: mov r0,r6
  000e49e4: blx 0x00074e90
  000e49e8: add sp,#0x8
  000e49ea: pop.w r8
  000e49ee: pop {r4,r5,r6,r7,pc}
```
