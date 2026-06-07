# PaintCanvas::GetMeshResourceId
elf 0x7800a body 76
Sig: undefined __thiscall GetMeshResourceId(PaintCanvas * this, String * param_1)

## decompile
```c

/* AbyssEngine::PaintCanvas::GetMeshResourceId(AbyssEngine::String&) */

undefined2 __thiscall AbyssEngine::PaintCanvas::GetMeshResourceId(PaintCanvas *this,String *param_1)

{
  int iVar1;
  uint uVar2;
  
  uVar2 = 0;
  while( true ) {
    if (*(uint *)(this + 0x134) <= uVar2) {
      return 0xffff;
    }
    iVar1 = *(int *)(*(int *)(this + 0x138) + uVar2 * 4);
    if (((iVar1 != 0) && (*(int *)(iVar1 + 4) == 4)) &&
       (iVar1 = String::Compare((String *)param_1,(char *)**(undefined4 **)(iVar1 + 0xc)),
       iVar1 == 0)) break;
    uVar2 = uVar2 + 1;
  }
  return **(undefined2 **)(*(int *)(this + 0x138) + uVar2 * 4);
}

```

## target disasm
```
  0008800a: push {r4,r5,r6,r7,lr}
  0008800c: add r7,sp,#0xc
  0008800e: push.w r11
  00088012: mov r5,r1
  00088014: mov r4,r0
  00088016: movs r6,#0x0
  00088018: b 0x00088038
  0008801a: ldr.w r0,[r4,#0x138]
  0008801e: ldr.w r0,[r0,r6,lsl #0x2]
  00088022: cbz r0,0x00088036
  00088024: ldr r1,[r0,#0x4]
  00088026: cmp r1,#0x4
  00088028: bne 0x00088036
  0008802a: ldr r0,[r0,#0xc]
  0008802c: ldr r1,[r0,#0x0]
  0008802e: mov r0,r5
  00088030: blx 0x0006fa3c
  00088034: cbz r0,0x00088046
  00088036: adds r6,#0x1
  00088038: ldr.w r0,[r4,#0x134]
  0008803c: cmp r6,r0
  0008803e: bcc 0x0008801a
  00088040: movw r0,#0xffff
  00088044: b 0x00088050
  00088046: ldr.w r0,[r4,#0x138]
  0008804a: ldr.w r0,[r0,r6,lsl #0x2]
  0008804e: ldrh r0,[r0,#0x0]
  00088050: pop.w r11
  00088054: pop {r4,r5,r6,r7,pc}
```
