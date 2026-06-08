# String::ToLowerCase
elf 0x73128 body 112
Sig: undefined __thiscall ToLowerCase(String * this)

## decompile
```c

/* AbyssEngine::String::ToLowerCase() */

void __thiscall AbyssEngine::String::ToLowerCase(String *this)

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
    uVar2 = sVar1 - 0x41;
    bVar7 = 0x19 < uVar2;
    bVar6 = uVar2 == 0x1a;
    if (bVar7) {
      uVar2 = sVar1 - 0xc0;
      bVar6 = uVar2 == 0x1e;
    }
    if ((!bVar7 || uVar2 < 0x1e) || bVar6) {
      sVar3 = sVar1 + 0x20;
      goto switchD_00083168_caseD_9a;
    }
    sVar3 = 0x81;
    switch(sVar1) {
    case 0x8e:
      sVar3 = 0x84;
      break;
    case 0x8f:
      sVar3 = 0x86;
      break;
    case 0x90:
      sVar3 = 0x82;
      break;
    case 0x91:
    case 0x93:
    case 0x94:
    case 0x95:
    case 0x96:
    case 0x97:
    case 0x98:
      goto switchD_00083168_caseD_91;
    case 0x92:
      sVar3 = 0x91;
      break;
    case 0x99:
      sVar3 = 0x94;
      break;
    case 0x9a:
      break;
    default:
      if (sVar1 == 0xa5) {
        sVar3 = 0xa4;
        break;
      }
      goto switchD_00083168_caseD_91;
    }
switchD_00083168_caseD_9a:
    *psVar5 = sVar3;
switchD_00083168_caseD_91:
    psVar5 = psVar5 + 1;
    iVar4 = iVar4 + -1;
  } while( true );
}

```

## target disasm
```
  00083128: push {r7,lr}
  0008312a: mov r7,sp
  0008312c: ldr.w lr,[r0,#0x4]
  00083130: cmp.w lr,#0x0
  00083134: it eq
  00083136: pop.eq {r7,pc}
  00083138: ldr.w r12,[r0,#0x8]
  0008313c: cmp.w r12,#0x0
  00083140: beq 0x000831a4
  00083142: ldrh.w r3,[lr,#0x0]
  00083146: sub.w r2,r3,#0x41
  0008314a: uxth r2,r2
  0008314c: cmp r2,#0x1a
  0008314e: ittt cs
  00083150: sub.cs.w r2,r3,#0xc0
  00083154: uxth.cs r2,r2
  00083156: cmp.cs r2,#0x1e
  00083158: bhi 0x0008315e
  0008315a: adds r3,#0x20
  0008315c: b 0x0008317a
  0008315e: sub.w r2,r3,#0x8e
  00083162: cmp r2,#0xc
  00083164: bhi 0x0008318c
  00083166: movs r3,#0x81
  00083168: tbb [pc,r2]
  0008317a: strh.w r3,[lr,#0x0]
  0008317e: add.w lr,lr,#0x2
  00083182: sub.w r12,r12,#0x1
  00083186: b 0x0008313c
  00083188: movs r3,#0x84
  0008318a: b 0x0008317a
  0008318c: cmp r3,#0xa5
  0008318e: bne 0x0008317e
  00083190: movs r3,#0xa4
  00083192: b 0x0008317a
  00083194: movs r3,#0x86
  00083196: b 0x0008317a
  00083198: movs r3,#0x82
  0008319a: b 0x0008317a
  0008319c: movs r3,#0x91
  0008319e: b 0x0008317a
  000831a0: movs r3,#0x94
  000831a2: b 0x0008317a
  000831a4: pop {r7,pc}
```
