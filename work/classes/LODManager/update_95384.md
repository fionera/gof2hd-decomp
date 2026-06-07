# LODManager::update
elf 0x95384 body 24
Sig: undefined __thiscall update(LODManager * this, int param_1)

## decompile
```c

/* LODManager::update(int) */

void __thiscall LODManager::update(LODManager *this,int param_1)

{
  int iVar1;
  
  iVar1 = *(int *)(this + 0x10);
  *(int *)(this + 0x10) = param_1 + iVar1;
  if (1000 < param_1 + iVar1) {
    *(undefined4 *)(this + 0x10) = 0;
    (*(code *)(DAT_001ab8e4 + 0x1ab8e8))(this,0,0);
    return;
  }
  return;
}

```

## target disasm
```
  000a5384: ldr r2,[r0,#0x10]
  000a5386: add r1,r2
  000a5388: str r1,[r0,#0x10]
  000a538a: cmp.w r1,#0x3e8
  000a538e: it le
  000a5390: bx.le lr
  000a5392: movs r1,#0x0
  000a5394: movs r2,#0x0
  000a5396: str r1,[r0,#0x10]
  000a5398: b.w 0x001ab8d8
```
