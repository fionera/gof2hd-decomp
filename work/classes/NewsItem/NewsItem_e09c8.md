# NewsItem::NewsItem
elf 0xe09c8 body 30
Sig: undefined __thiscall NewsItem(NewsItem * this, int param_1, bool param_2, bool * param_3, int param_4, int param_5, int param_6)

## decompile
```c

/* NewsItem::NewsItem(int, bool, bool*, int, int, int) */

void __thiscall
NewsItem::NewsItem(NewsItem *this,int param_1,bool param_2,bool *param_3,int param_4,int param_5,
                  int param_6)

{
  this[4] = (NewsItem)param_2;
  *(int *)this = param_1;
  *(bool **)(this + 8) = param_3;
  *(int *)(this + 0xc) = param_4;
  *(int *)(this + 0x10) = param_5;
  *(int *)(this + 0x14) = param_6;
  this[0x18] = (NewsItem)0x0;
  return;
}

```

## target disasm
```
  000f09c8: push {r4,r6,r7,lr}
  000f09ca: add r7,sp,#0x8
  000f09cc: add.w lr,r7,#0x8
  000f09d0: ldm.w lr,{r4,r12,lr}
  000f09d4: strb r2,[r0,#0x4]
  000f09d6: str r1,[r0,#0x0]
  000f09d8: add.w r1,r0,#0x8
  000f09dc: stm r1,{r3,r4,r12,lr}
  000f09e0: movs r1,#0x0
  000f09e2: strb r1,[r0,#0x18]
  000f09e4: pop {r4,r6,r7,pc}
```
