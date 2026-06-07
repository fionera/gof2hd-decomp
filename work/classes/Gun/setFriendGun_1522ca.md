# Gun::setFriendGun
elf 0x1522ca body 6
Sig: undefined __thiscall setFriendGun(Gun * this, bool param_1)

## decompile
```c

/* Gun::setFriendGun(bool) */

void __thiscall Gun::setFriendGun(Gun *this,bool param_1)

{
  this[0xf9] = (Gun)param_1;
  return;
}

```

## target disasm
```
  001622ca: strb.w r1,[r0,#0xf9]
  001622ce: bx lr
```
