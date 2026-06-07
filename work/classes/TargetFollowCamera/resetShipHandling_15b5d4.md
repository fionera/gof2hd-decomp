# TargetFollowCamera::resetShipHandling
elf 0x15b5d4 body 18
Sig: undefined __thiscall resetShipHandling(TargetFollowCamera * this)

## decompile
```c

/* TargetFollowCamera::resetShipHandling() */

void __thiscall TargetFollowCamera::resetShipHandling(TargetFollowCamera *this)

{
  int iVar1;
  
  iVar1 = DAT_0016b5e8;
  this[1].m_nPosZ = DAT_0016b5e4;
  this[1].field_14 = iVar1;
  (*(code *)(DAT_001ac5e4 + 0x1ac5e8))(this,0x3f800000);
  return;
}

```

## target disasm
```
  0016b5d4: ldr r2,[0x0016b5e8]
  0016b5d6: ldr r1,[0x0016b5e4]
  0016b5d8: strd r1,r2,[r0,#0x128]
  0016b5dc: mov.w r1,#0x3f800000
  0016b5e0: b.w 0x001ac5d8
  001ac5d8: bx pc
```
