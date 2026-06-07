# PaintCanvas::GetMeshResourceId
elf 0x77fb4 body 86
Sig: undefined __thiscall GetMeshResourceId(PaintCanvas * this, String * param_1, ushort param_2)

## decompile
```c

/* AbyssEngine::PaintCanvas::GetMeshResourceId(AbyssEngine::String&, unsigned short) */

undefined2 __thiscall
AbyssEngine::PaintCanvas::GetMeshResourceId(PaintCanvas *this,String *param_1,ushort param_2)

{
  int iVar1;
  undefined2 *puVar2;
  uint uVar3;
  
  uVar3 = 0;
  while( true ) {
    if (*(uint *)(this + 0x134) <= uVar3) {
      return 0xffff;
    }
    iVar1 = *(int *)(*(int *)(this + 0x138) + uVar3 * 4);
    if ((((iVar1 != 0) && (*(int *)(iVar1 + 4) == 4)) &&
        (iVar1 = String::Compare((String *)param_1,(char *)**(undefined4 **)(iVar1 + 0xc)),
        iVar1 == 0)) &&
       (puVar2 = *(undefined2 **)(*(int *)(this + 0x138) + uVar3 * 4),
       *(ushort *)(*(int *)(puVar2 + 6) + 4) == param_2)) break;
    uVar3 = uVar3 + 1;
  }
  return *puVar2;
}

```

## target disasm
```
  00087fb4: push {r4,r5,r6,r7,lr}
  00087fb6: add r7,sp,#0xc
  00087fb8: push.w r8
  00087fbc: mov r8,r2
  00087fbe: mov r5,r1
  00087fc0: mov r6,r0
  00087fc2: movs r4,#0x0
  00087fc4: b 0x00087ff4
  00087fc6: ldr.w r0,[r6,#0x138]
  00087fca: ldr.w r0,[r0,r4,lsl #0x2]
  00087fce: cbz r0,0x00087ff2
  00087fd0: ldr r1,[r0,#0x4]
  00087fd2: cmp r1,#0x4
  00087fd4: bne 0x00087ff2
  00087fd6: ldr r0,[r0,#0xc]
  00087fd8: ldr r1,[r0,#0x0]
  00087fda: mov r0,r5
  00087fdc: blx 0x0006fa3c
  00087fe0: cbnz r0,0x00087ff2
  00087fe2: ldr.w r0,[r6,#0x138]
  00087fe6: ldr.w r0,[r0,r4,lsl #0x2]
  00087fea: ldr r1,[r0,#0xc]
  00087fec: ldrh r1,[r1,#0x4]
  00087fee: cmp r1,r8
  00087ff0: beq 0x00088006
  00087ff2: adds r4,#0x1
  00087ff4: ldr.w r0,[r6,#0x134]
  00087ff8: cmp r4,r0
  00087ffa: bcc 0x00087fc6
  00087ffc: movw r0,#0xffff
  00088000: pop.w r8
  00088004: pop {r4,r5,r6,r7,pc}
  00088006: ldrh r0,[r0,#0x0]
  00088008: b 0x00088000
```
