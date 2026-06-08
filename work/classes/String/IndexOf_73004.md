# String::IndexOf
elf 0x73004 body 100
Sig: undefined __thiscall IndexOf(String * this, uint param_1, String * param_2)

## decompile
```c

/* AbyssEngine::String::IndexOf(unsigned int, AbyssEngine::String const&) */

uint __thiscall AbyssEngine::String::IndexOf(String *this,uint param_1,String *param_2)

{
  uint uVar1;
  short sVar2;
  
  while ((param_1 < *(uint *)(this + 8) && (*(uint *)(param_2 + 8) <= *(uint *)(this + 8) - param_1)
         )) {
    sVar2 = **(short **)(param_2 + 4);
    if (sVar2 == *(short *)(*(int *)(this + 4) + param_1 * 2)) {
      uVar1 = 0;
      while (sVar2 == *(short *)(*(int *)(this + 4) + param_1 * 2 + uVar1 * 2)) {
        if (*(uint *)(param_2 + 8) <= uVar1 + 1) {
          return param_1;
        }
        sVar2 = (*(short **)(param_2 + 4))[uVar1 + 1];
        uVar1 = uVar1 + 1;
      }
      param_1 = param_1 + uVar1;
    }
    else {
      param_1 = param_1 + 1;
    }
  }
  return 0xffffffff;
}

```

## target disasm
```
  00083004: push {r4,r5,r6,r7,lr}
  00083006: add r7,sp,#0xc
  00083008: push.w r8
  0008300c: ldrd r12,r3,[r2,#0x4]
  00083010: ldr.w lr,[r0,#0x8]
  00083014: add.w r8,r12,#0x2
  00083018: b 0x00083054
  0008301a: sub.w r4,lr,r1
  0008301e: cmp r3,r4
  00083020: bhi 0x00083060
  00083022: ldr r2,[r0,#0x4]
  00083024: ldrh.w r6,[r12,#0x0]
  00083028: ldrh.w r4,[r2,r1,lsl #0x1]
  0008302c: cmp r6,r4
  0008302e: bne 0x00083052
  00083030: add.w r2,r2,r1, lsl #0x1
  00083034: movs r4,#0x0
  00083036: b 0x00083044
  00083038: adds r5,r4,#0x1
  0008303a: cmp r5,r3
  0008303c: bcs 0x0008305e
  0008303e: ldrh.w r6,[r8,r4,lsl #0x1]
  00083042: mov r4,r5
  00083044: ldrh.w r5,[r2,r4,lsl #0x1]
  00083048: uxth r6,r6
  0008304a: cmp r6,r5
  0008304c: beq 0x00083038
  0008304e: add r1,r4
  00083050: b 0x00083054
  00083052: adds r1,#0x1
  00083054: mov.w r2,#0xffffffff
  00083058: cmp lr,r1
  0008305a: bhi 0x0008301a
  0008305c: b 0x00083060
  0008305e: mov r2,r1
  00083060: mov r0,r2
  00083062: pop.w r8
  00083066: pop {r4,r5,r6,r7,pc}
```
