# NewsItem::~NewsItem
elf 0xe09e6 body 20
Sig: undefined __thiscall ~NewsItem(NewsItem * this)

## decompile
```c

/* NewsItem::~NewsItem() */

NewsItem * __thiscall NewsItem::~NewsItem(NewsItem *this)

{
  operator_delete__(*(void **)(this + 8));
  *(undefined4 *)(this + 8) = 0;
  return this;
}

```

## target disasm
```
  000f09e6: push {r4,r6,r7,lr}
  000f09e8: add r7,sp,#0x8
  000f09ea: mov r4,r0
  000f09ec: ldr r0,[r0,#0x8]
  000f09ee: blx 0x0006ebfc
  000f09f2: movs r0,#0x0
  000f09f4: str r0,[r4,#0x8]
  000f09f6: mov r0,r4
  000f09f8: pop {r4,r6,r7,pc}
```
