# Item::transaction
elf 0xe01e8 body 78
Sig: undefined __thiscall transaction(Item * this, bool param_1, int param_2, bool param_3)

## decompile
```c

/* Item::transaction(bool, int, bool) */

int __thiscall Item::transaction(Item *this,bool param_1,int param_2,bool param_3)

{
  int iVar1;
  int iVar2;
  
  if (param_1) {
    iVar1 = *(int *)(this + 0x38);
    if (0 < iVar1) {
      if (param_3) {
        iVar2 = *(int *)(this + 0x18);
      }
      else {
        iVar1 = Status::getCredits();
        iVar2 = *(int *)(this + 0x18);
        if (iVar1 < iVar2) {
          return 0;
        }
        iVar1 = *(int *)(this + 0x38);
      }
      *(int *)(this + 0x34) = *(int *)(this + 0x34) + 1;
      *(int *)(this + 0x38) = iVar1 + -1;
      return -iVar2;
    }
  }
  else if (0 < *(int *)(this + 0x34)) {
    *(int *)(this + 0x34) = *(int *)(this + 0x34) + -1;
    *(int *)(this + 0x38) = *(int *)(this + 0x38) + 1;
    return *(int *)(this + 0x18);
  }
  return 0;
}

```
## target disasm
```
  000f01e8: push {r4,r6,r7,lr}
  000f01ea: add r7,sp,#0x8
  000f01ec: mov r4,r0
  000f01ee: cbz r1,0x000f01fc
  000f01f0: ldr r0,[r4,#0x38]
  000f01f2: cmp r0,#0x1
  000f01f4: blt 0x000f0222
  000f01f6: cbz r3,0x000f0210
  000f01f8: ldr r1,[r4,#0x18]
  000f01fa: b 0x000f0228
  000f01fc: ldr r1,[r4,#0x34]
  000f01fe: cmp r1,#0x1
  000f0200: blt 0x000f0222
  000f0202: ldr r2,[r4,#0x38]
  000f0204: subs r1,#0x1
  000f0206: ldr r0,[r4,#0x18]
  000f0208: adds r2,#0x1
  000f020a: strd r1,r2,[r4,#0x34]
  000f020e: pop {r4,r6,r7,pc}
  000f0210: ldr r0,[0x000f0238]
  000f0212: add r0,pc
  000f0214: ldr r0,[r0,#0x0]
  000f0216: ldr r0,[r0,#0x0]
  000f0218: blx 0x000733d8
  000f021c: ldr r1,[r4,#0x18]
  000f021e: cmp r0,r1
  000f0220: bge 0x000f0226
  000f0222: movs r0,#0x0
  000f0224: pop {r4,r6,r7,pc}
  000f0226: ldr r0,[r4,#0x38]
  000f0228: ldr r2,[r4,#0x34]
  000f022a: subs r0,#0x1
  000f022c: adds r2,#0x1
  000f022e: strd r2,r0,[r4,#0x34]
  000f0232: rsbs r0,r1
  000f0234: pop {r4,r6,r7,pc}
```
