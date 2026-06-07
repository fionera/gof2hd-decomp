# CutScene::update
elf 0x99664 body 14
Sig: undefined __thiscall update(CutScene * this)

## decompile
```c

/* CutScene::update() */

void __thiscall CutScene::update(CutScene *this)

{
  (*(code *)(DAT_001ab9c4 + 0x1ab9c8))(this);
  return;
}

```

## target disasm
```
  000a9664: b.w 0x001ab9b8
  001ab9b8: bx pc
  001ab9bc: ldr r12,[0x1ab9c4]
  001ab9c0: add pc,r12,pc
```
