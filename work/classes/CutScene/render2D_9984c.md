# CutScene::render2D
elf 0x9984c body 8
Sig: undefined __stdcall render2D(void)

## decompile
```c

/* CutScene::render2D() */

void CutScene::render2D(void)

{
  undefined4 *in_r0;
  
  (*(code *)(DAT_001ab9e4 + 0x1ab9e8))(*in_r0);
  return;
}

```

## target disasm
```
  000a984c: ldr r0,[r0,#0x0]
  000a984e: b.w 0x001ab9d8
  001ab9d8: bx pc
```
