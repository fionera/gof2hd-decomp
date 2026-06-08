# String::ToUpperCase
elf 0x730a4 body 124
Sig: undefined __thiscall ToUpperCase(String * this)

## decompile
```c

/* AbyssEngine::String::ToUpperCase() */

void __thiscall AbyssEngine::String::ToUpperCase(String *this)

{
  short sVar1;
  ushort uVar2;
  short sVar3;
  int iVar4;
  short *psVar5;
  bool bVar6;
  bool bVar7;
  
  psVar5 = *(short **)(this + 4);
  if (psVar5 == (short *)0x0) {
    return;
  }
  iVar4 = *(int *)(this + 8);
  do {
    if (iVar4 == 0) {
      return;
    }
    sVar1 = *psVar5;
    uVar2 = sVar1 - 0x61;
    bVar7 = 0x19 < uVar2;
    bVar6 = uVar2 == 0x1a;
    if (bVar7) {
      uVar2 = sVar1 - 0xe0;
      bVar6 = uVar2 == 0x1e;
    }
    if ((!bVar7 || uVar2 < 0x1e) || bVar6) {
      sVar3 = sVar1 + -0x20;
      goto switchD_000830e4_caseD_81;
    }
    sVar3 = 0x9a;
    switch(sVar1) {
    case 0x81:
      break;
    case 0x82:
      sVar3 = 0x90;
      break;
    case 0x83:
    case 0x85:
      goto LAB_000830f0;
    case 0x84:
      sVar3 = 0x8e;
      break;
    case 0x86:
      sVar3 = 0x8f;
      break;
    default:
      if (sVar1 == 0x91) {
        sVar3 = 0x92;
      }
      else if (sVar1 == 0x94) {
        sVar3 = 0x99;
      }
      else {
        if (sVar1 != 0xa4) goto LAB_000830f0;
        sVar3 = 0xa5;
      }
    }
switchD_000830e4_caseD_81:
    *psVar5 = sVar3;
LAB_000830f0:
    psVar5 = psVar5 + 1;
    iVar4 = iVar4 + -1;
  } while( true );
}

```

## target disasm
```
  000830a4: push {r7,lr}
  000830a6: mov r7,sp
  000830a8: ldr.w lr,[r0,#0x4]
  000830ac: cmp.w lr,#0x0
  000830b0: it eq
  000830b2: pop.eq {r7,pc}
  000830b4: ldr.w r12,[r0,#0x8]
  000830b8: cmp.w r12,#0x0
  000830bc: beq 0x00083124
  000830be: ldrh.w r3,[lr,#0x0]
  000830c2: sub.w r2,r3,#0x61
  000830c6: uxth r2,r2
  000830c8: cmp r2,#0x1a
  000830ca: ittt cs
  000830cc: sub.cs.w r2,r3,#0xe0
  000830d0: uxth.cs r2,r2
  000830d2: cmp.cs r2,#0x1e
  000830d4: bhi 0x000830da
  000830d6: subs r3,#0x20
  000830d8: b 0x000830fa
  000830da: sub.w r2,r3,#0x81
  000830de: cmp r2,#0x5
  000830e0: bhi 0x00083104
  000830e2: movs r3,#0x9a
  000830e4: tbb [pc,r2]
  000830ee: b 0x000830f0
  000830f0: add.w lr,lr,#0x2
  000830f4: sub.w r12,r12,#0x1
  000830f8: b 0x000830b8
  000830fa: strh.w r3,[lr,#0x0]
  000830fe: b 0x000830f0
  00083100: movs r3,#0x90
  00083102: b 0x000830fa
  00083104: cmp r3,#0x91
  00083106: beq 0x0008311c
  00083108: cmp r3,#0x94
  0008310a: beq 0x00083120
  0008310c: cmp r3,#0xa4
  0008310e: bne 0x000830f0
  00083110: movs r3,#0xa5
  00083112: b 0x000830fa
  00083114: movs r3,#0x8e
  00083116: b 0x000830fa
  00083118: movs r3,#0x8f
  0008311a: b 0x000830fa
  0008311c: movs r3,#0x92
  0008311e: b 0x000830fa
  00083120: movs r3,#0x99
  00083122: b 0x000830fa
  00083124: pop {r7,pc}
```
