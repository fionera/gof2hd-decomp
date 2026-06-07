# PlayerCreature::~PlayerCreature
elf 0x11ccd8 body 16
Sig: undefined __thiscall ~PlayerCreature(PlayerCreature * this)

## decompile
```c

/* PlayerCreature::~PlayerCreature() */

void __thiscall PlayerCreature::~PlayerCreature(PlayerCreature *this)

{
  KIPlayer::~KIPlayer((KIPlayer *)this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  0012ccd8: push {r7,lr}
  0012ccda: mov r7,sp
  0012ccdc: blx 0x000732b8
  0012cce0: pop.w {r7,lr}
  0012cce4: b.w 0x001ab098
```
