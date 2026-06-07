# CheatHandler::AddCheatCode
elf 0x74538 body 190
Sig: undefined __thiscall AddCheatCode(CheatHandler * this, String * param_1, int param_2)

## decompile
```c

/* AbyssEngine::CheatHandler::AddCheatCode(AbyssEngine::String const&, int) */

void __thiscall
AbyssEngine::CheatHandler::AddCheatCode(CheatHandler *this,String *param_1,int param_2)

{
  ushort uVar1;
  CheatCode *this_00;
  undefined2 *puVar2;
  Array *pAVar3;
  uint uVar4;
  
  this_00 = operator_new(0xc);
  CheatCode::CheatCode(this_00);
  *(int *)(this_00 + 4) = param_2;
  ArrayAdd<AbyssEngine::CheatCode*>(this_00,*(Array **)(this + 8));
  uVar4 = 0;
  do {
    if (*(uint *)(param_1 + 8) <= uVar4) {
      return;
    }
    puVar2 = (undefined2 *)String::operator[]((String *)param_1,uVar4);
    switch(*puVar2) {
    case 0x30:
      pAVar3 = *(Array **)this_00;
      uVar1 = **(ushort **)(this + 0xc);
      break;
    case 0x31:
      pAVar3 = *(Array **)this_00;
      uVar1 = *(ushort *)(*(int *)(this + 0xc) + 0x10);
      break;
    case 0x32:
      pAVar3 = *(Array **)this_00;
      uVar1 = *(ushort *)(*(int *)(this + 0xc) + 0x20);
      break;
    case 0x33:
      pAVar3 = *(Array **)this_00;
      uVar1 = *(ushort *)(*(int *)(this + 0xc) + 0x30);
      break;
    case 0x34:
      pAVar3 = *(Array **)this_00;
      uVar1 = *(ushort *)(*(int *)(this + 0xc) + 0x40);
      break;
    case 0x35:
      pAVar3 = *(Array **)this_00;
      uVar1 = *(ushort *)(*(int *)(this + 0xc) + 0x50);
      break;
    case 0x36:
      pAVar3 = *(Array **)this_00;
      uVar1 = *(ushort *)(*(int *)(this + 0xc) + 0x60);
      break;
    case 0x37:
      pAVar3 = *(Array **)this_00;
      uVar1 = *(ushort *)(*(int *)(this + 0xc) + 0x70);
      break;
    case 0x38:
      pAVar3 = *(Array **)this_00;
      uVar1 = *(ushort *)(*(int *)(this + 0xc) + 0x80);
      break;
    case 0x39:
      pAVar3 = *(Array **)this_00;
      uVar1 = *(ushort *)(*(int *)(this + 0xc) + 0x90);
      break;
    default:
      goto switchD_00084572_default;
    }
    ArrayAdd<unsigned_short>(uVar1,pAVar3);
switchD_00084572_default:
    uVar4 = uVar4 + 1;
  } while( true );
}

```

## target disasm
```
  00084538: push {r4,r5,r6,r7,lr}
  0008453a: add r7,sp,#0xc
  0008453c: push.w r8
  00084540: mov r8,r0
  00084542: movs r0,#0xc
  00084544: mov r5,r2
  00084546: mov r4,r1
  00084548: blx 0x0006eb24
  0008454c: mov r6,r0
  0008454e: blx 0x0006f6f4
  00084552: str r5,[r6,#0x4]
  00084554: mov r0,r6
  00084556: ldr.w r1,[r8,#0x8]
  0008455a: blx 0x0006f700
  0008455e: movs r5,#0x0
  00084560: b 0x000845f4
  00084562: mov r0,r4
  00084564: mov r1,r5
  00084566: blx 0x0006f37c
  0008456a: ldrh r0,[r0,#0x0]
  0008456c: subs r0,#0x30
  0008456e: cmp r0,#0x9
  00084570: bhi 0x000845f2
  00084572: tbb [pc,r0]
  00084580: ldr.w r0,[r8,#0xc]
  00084584: ldr r1,[r6,#0x0]
  00084586: ldrh r0,[r0,#0x0]
  00084588: b 0x000845ee
  0008458a: ldr.w r0,[r8,#0xc]
  0008458e: ldr r1,[r6,#0x0]
  00084590: ldrh r0,[r0,#0x20]
  00084592: b 0x000845ee
  00084594: ldr.w r0,[r8,#0xc]
  00084598: ldr r1,[r6,#0x0]
  0008459a: ldrh r0,[r0,#0x30]
  0008459c: b 0x000845ee
  0008459e: ldr.w r0,[r8,#0xc]
  000845a2: ldr r1,[r6,#0x0]
  000845a4: ldrh.w r0,[r0,#0x40]
  000845a8: b 0x000845ee
  000845aa: ldr.w r0,[r8,#0xc]
  000845ae: ldr r1,[r6,#0x0]
  000845b0: ldrh.w r0,[r0,#0x70]
  000845b4: b 0x000845ee
  000845b6: ldr.w r0,[r8,#0xc]
  000845ba: ldr r1,[r6,#0x0]
  000845bc: ldrh.w r0,[r0,#0x90]
  000845c0: b 0x000845ee
  000845c2: ldr.w r0,[r8,#0xc]
  000845c6: ldr r1,[r6,#0x0]
  000845c8: ldrh r0,[r0,#0x10]
  000845ca: b 0x000845ee
  000845cc: ldr.w r0,[r8,#0xc]
  000845d0: ldr r1,[r6,#0x0]
  000845d2: ldrh.w r0,[r0,#0x50]
  000845d6: b 0x000845ee
  000845d8: ldr.w r0,[r8,#0xc]
  000845dc: ldr r1,[r6,#0x0]
  000845de: ldrh.w r0,[r0,#0x60]
  000845e2: b 0x000845ee
  000845e4: ldr.w r0,[r8,#0xc]
  000845e8: ldr r1,[r6,#0x0]
  000845ea: ldrh.w r0,[r0,#0x80]
  000845ee: blx 0x0006f70c
  000845f2: adds r5,#0x1
  000845f4: ldr r0,[r4,#0x8]
  000845f6: cmp r5,r0
  000845f8: bcc 0x00084562
  000845fa: pop.w r8
  000845fe: pop {r4,r5,r6,r7,pc}
```
