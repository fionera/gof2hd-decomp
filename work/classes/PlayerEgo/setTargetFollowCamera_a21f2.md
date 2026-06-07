# PlayerEgo::setTargetFollowCamera
elf 0xa21f2 body 18
Sig: undefined __thiscall setTargetFollowCamera(PlayerEgo * this, TargetFollowCamera * param_1)

## decompile
```c

/* PlayerEgo::setTargetFollowCamera(TargetFollowCamera*) */

void __thiscall PlayerEgo::setTargetFollowCamera(PlayerEgo *this,TargetFollowCamera *param_1)

{
  int *piVar1;
  
  piVar1 = this[9].field_10;
  this[3].field_1C = (int)param_1;
  (*(code *)(DAT_001abc94 + 0x1abc98))(param_1,piVar1);
  return;
}

```

## target disasm
```
  000b21f2: mov r2,r1
  000b21f4: ldr.w r1,[r0,#0x154]
  000b21f8: str.w r2,[r0,#0x88]
  000b21fc: mov r0,r2
  000b21fe: b.w 0x001abc88
  001abc88: bx pc
```
