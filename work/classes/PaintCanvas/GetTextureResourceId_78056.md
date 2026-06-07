# PaintCanvas::GetTextureResourceId
elf 0x78056 body 76
Sig: undefined __thiscall GetTextureResourceId(PaintCanvas * this, String * param_1)

## decompile
```c

/* AbyssEngine::PaintCanvas::GetTextureResourceId(AbyssEngine::String&) */

undefined2 __thiscall
AbyssEngine::PaintCanvas::GetTextureResourceId(PaintCanvas *this,String *param_1)

{
  int iVar1;
  uint uVar2;
  
  uVar2 = 0;
  while( true ) {
    if (*(uint *)(this + 0x134) <= uVar2) {
      return 0xffff;
    }
    iVar1 = *(int *)(*(int *)(this + 0x138) + uVar2 * 4);
    if (((iVar1 != 0) && (*(int *)(iVar1 + 4) == 2)) &&
       (iVar1 = String::Compare((String *)param_1,(char *)**(undefined4 **)(iVar1 + 0xc)),
       iVar1 == 0)) break;
    uVar2 = uVar2 + 1;
  }
  return **(undefined2 **)(*(int *)(this + 0x138) + uVar2 * 4);
}

```

## target disasm
```
  00088056: push {r4,r5,r6,r7,lr}
  00088058: add r7,sp,#0xc
  0008805a: push.w r11
  0008805e: mov r5,r1
  00088060: mov r4,r0
  00088062: movs r6,#0x0
  00088064: b 0x00088084
  00088066: ldr.w r0,[r4,#0x138]
  0008806a: ldr.w r0,[r0,r6,lsl #0x2]
  0008806e: cbz r0,0x00088082
  00088070: ldr r1,[r0,#0x4]
  00088072: cmp r1,#0x2
  00088074: bne 0x00088082
  00088076: ldr r0,[r0,#0xc]
  00088078: ldr r1,[r0,#0x0]
  0008807a: mov r0,r5
  0008807c: blx 0x0006fa3c
  00088080: cbz r0,0x00088092
  00088082: adds r6,#0x1
  00088084: ldr.w r0,[r4,#0x134]
  00088088: cmp r6,r0
  0008808a: bcc 0x00088066
  0008808c: movw r0,#0xffff
  00088090: b 0x0008809c
  00088092: ldr.w r0,[r4,#0x138]
  00088096: ldr.w r0,[r0,r6,lsl #0x2]
  0008809a: ldrh r0,[r0,#0x0]
  0008809c: pop.w r11
  000880a0: pop {r4,r5,r6,r7,pc}
```
