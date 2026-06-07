# PlayerStation::~PlayerStation
elf 0x1225ec body 16
Sig: undefined __thiscall ~PlayerStation(PlayerStation * this)

## decompile
```c

/* PlayerStation::~PlayerStation() */

void __thiscall PlayerStation::~PlayerStation(PlayerStation *this)

{
  ~PlayerStation(this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  001325ec: push {r7,lr}
  001325ee: mov r7,sp
  001325f0: blx 0x000763fc
  001325f4: pop.w {r7,lr}
  001325f8: b.w 0x001ab098
```
