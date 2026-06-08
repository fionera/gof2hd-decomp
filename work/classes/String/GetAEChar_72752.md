# String::GetAEChar
elf 0x72752 body 50
Sig: undefined __stdcall GetAEChar(void)

## decompile
```c

/* AbyssEngine::String::GetAEChar() const */

void AbyssEngine::String::GetAEChar(void)

{
  int in_r0;
  void *pvVar1;
  undefined1 *puVar2;
  uint uVar3;
  uint uVar4;
  
  if (*(int *)(in_r0 + 8) != 0) {
    uVar4 = *(int *)(in_r0 + 8) + 1;
    pvVar1 = operator_new__(uVar4);
    for (uVar3 = 0; uVar4 != uVar3; uVar3 = uVar3 + 1) {
      *(undefined1 *)((int)pvVar1 + uVar3) = *(undefined1 *)(*(int *)(in_r0 + 4) + uVar3 * 2);
    }
    return;
  }
  puVar2 = operator_new__(1);
  *puVar2 = 0;
  return;
}

```

## target disasm
```
  00082752: push {r4,r5,r7,lr}
  00082754: add r7,sp,#0x8
  00082756: mov r4,r0
  00082758: ldr r0,[r0,#0x8]
  0008275a: cbz r0,0x00082778
  0008275c: adds r5,r0,#0x1
  0008275e: mov r0,r5
  00082760: blx 0x0006ec08
  00082764: movs r1,#0x0
  00082766: b 0x00082772
  00082768: ldr r2,[r4,#0x4]
  0008276a: ldrb.w r2,[r2,r1,lsl #0x1]
  0008276e: strb r2,[r0,r1]
  00082770: adds r1,#0x1
  00082772: cmp r5,r1
  00082774: bne 0x00082768
  00082776: pop {r4,r5,r7,pc}
  00082778: movs r0,#0x1
  0008277a: blx 0x0006ec08
  0008277e: movs r1,#0x0
  00082780: strb r1,[r0,#0x0]
  00082782: pop {r4,r5,r7,pc}
```
