# Standing::applyMissionCompleted
elf 0x11d992 body 8
Sig: undefined __thiscall applyMissionCompleted(Standing * this, int param_1)

## decompile
```c

/* Standing::applyMissionCompleted(int) */

void __thiscall Standing::applyMissionCompleted(Standing *this,int param_1)

{
  (*(code *)(DAT_001ac3c4 + 0x1ac3c8))(this,param_1,0xfffffffb);
  return;
}

```

## target disasm
```
  0012d992: mvn r2,#0x4
  0012d996: b.w 0x001ac3b8
```
