# Waypoint::~Waypoint
elf 0x157d28 body 16
Sig: undefined __thiscall ~Waypoint(Waypoint * this)

## decompile
```c

/* Waypoint::~Waypoint() */

void __thiscall Waypoint::~Waypoint(Waypoint *this)

{
  KIPlayer::~KIPlayer((KIPlayer *)this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  00167d28: push {r7,lr}
  00167d2a: mov r7,sp
  00167d2c: blx 0x000732b8
  00167d30: pop.w {r7,lr}
  00167d34: b.w 0x001ab098
```
