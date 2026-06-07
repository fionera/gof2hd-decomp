# Mission::getName
elf 0x15baec body 54
Sig: undefined __stdcall getName(void)

## decompile
```c

/* Mission::getName() */

void Mission::getName(void)

{
  String *in_r0;
  String *pSVar1;
  int in_r1;
  
  if (*(int *)(in_r1 + 100) != 0) {
    AbyssEngine::String::String(in_r0,(char *)(DAT_0016bb24 + 0x16bb00),false);
    return;
  }
  pSVar1 = (String *)GameText::getText(**(int **)(DAT_0016bb28 + 0x16bb0c));
  AbyssEngine::String::String(in_r0,pSVar1,false);
  return;
}

```

## target disasm
```
  0016baec: push {r4,r6,r7,lr}
  0016baee: add r7,sp,#0x8
  0016baf0: mov r4,r0
  0016baf2: ldr r0,[r1,#0x64]
  0016baf4: cbz r0,0x0016bb04
  0016baf6: ldr r1,[0x0016bb24]
  0016baf8: mov r0,r4
  0016bafa: movs r2,#0x0
  0016bafc: add r1,pc
  0016bafe: blx 0x0006ee18
  0016bb02: pop {r4,r6,r7,pc}
  0016bb04: ldr r0,[0x0016bb28]
  0016bb06: ldr r1,[r1,#0xc]
  0016bb08: add r0,pc
  0016bb0a: add.w r1,r1,#0x162
  0016bb0e: ldr r0,[r0,#0x0]
  0016bb10: ldr r0,[r0,#0x0]
  0016bb12: blx 0x00072f70
  0016bb16: mov r1,r0
  0016bb18: mov r0,r4
  0016bb1a: movs r2,#0x0
  0016bb1c: blx 0x0006f028
  0016bb20: pop {r4,r6,r7,pc}
```
