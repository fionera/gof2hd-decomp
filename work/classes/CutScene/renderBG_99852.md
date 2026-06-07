# CutScene::renderBG
elf 0x99852 body 12
Sig: undefined __stdcall renderBG(void)

## decompile
```c

/* CutScene::renderBG() */

void CutScene::renderBG(void)

{
  undefined4 *in_r0;
  
  (*(code *)(DAT_001ab9f4 + 0x1ab9f8))(*in_r0,in_r0[0x16]);
  return;
}

```

## target disasm
```
  000a9852: ldr r2,[r0,#0x0]
  000a9854: ldr r1,[r0,#0x58]
  000a9856: mov r0,r2
  000a9858: b.w 0x001ab9e8
  001ab9e8: bx pc
```
