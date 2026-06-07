# Globals::getItemName
elf 0xe4b68 body 34
Sig: undefined __stdcall getItemName(int param_1)

## decompile
```c

/* Globals::getItemName(int) */

void Globals::getItemName(int param_1)

{
  String *pSVar1;
  
  pSVar1 = (String *)GameText::getText(**(int **)(DAT_000f4b8c + 0xf4b78));
  AbyssEngine::String::String((String *)param_1,pSVar1,false);
  return;
}

```

## target disasm
```
  000f4b68: push {r4,r6,r7,lr}
  000f4b6a: add r7,sp,#0x8
  000f4b6c: mov r4,r0
  000f4b6e: ldr r0,[0x000f4b8c]
  000f4b70: addw r1,r2,#0x4fa
  000f4b74: add r0,pc
  000f4b76: ldr r0,[r0,#0x0]
  000f4b78: ldr r0,[r0,#0x0]
  000f4b7a: blx 0x00072f70
  000f4b7e: mov r1,r0
  000f4b80: mov r0,r4
  000f4b82: movs r2,#0x0
  000f4b84: blx 0x0006f028
  000f4b88: pop {r4,r6,r7,pc}
```
