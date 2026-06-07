# AutoPilotList::getTargetString
elf 0x13139c body 44
Sig: undefined __stdcall getTargetString(void)

## decompile
```c

/* AutoPilotList::getTargetString() */

void AutoPilotList::getTargetString(void)

{
  String *in_r0;
  uint *in_r1;
  uint uVar1;
  
  uVar1 = *in_r1;
  if ((-1 < (int)uVar1) && (uVar1 < *(uint *)in_r1[4])) {
    AbyssEngine::String::String(in_r0,*(String **)(((uint *)in_r1[4])[1] + uVar1 * 4),false);
    return;
  }
  AbyssEngine::String::String(in_r0,(char *)(DAT_001413c8 + 0x1413c4),false);
  return;
}

```

## target disasm
```
  0014139c: push {r7,lr}
  0014139e: mov r7,sp
  001413a0: ldr r2,[r1,#0x0]
  001413a2: cmp r2,#0x0
  001413a4: blt 0x001413bc
  001413a6: ldr r1,[r1,#0x10]
  001413a8: ldr r3,[r1,#0x0]
  001413aa: cmp r2,r3
  001413ac: bcs 0x001413bc
  001413ae: ldr r1,[r1,#0x4]
  001413b0: ldr.w r1,[r1,r2,lsl #0x2]
  001413b4: movs r2,#0x0
  001413b6: blx 0x0006f028
  001413ba: pop {r7,pc}
  001413bc: ldr r1,[0x001413c8]
  001413be: movs r2,#0x0
  001413c0: add r1,pc
  001413c2: blx 0x0006ee18
  001413c6: pop {r7,pc}
```
