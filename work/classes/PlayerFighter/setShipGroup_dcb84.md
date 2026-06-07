# PlayerFighter::setShipGroup
elf 0xdcb84 body 26
Sig: undefined __stdcall setShipGroup(AEGeometry * param_1, int param_2, bool param_3)

## decompile
```c

/* PlayerFighter::setShipGroup(AEGeometry*, int, bool) */

void PlayerFighter::setShipGroup(AEGeometry *param_1,int param_2,bool param_3)

{
                    /* WARNING: Could not recover jumptable at 0x001ac1c0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_001ac1c4 + 0x1ac1c8))(param_1,param_2,param_3);
  return;
}

```

## target disasm
```
  00073114: adr r12,0x17311c
  00073118: add r12,r12,#0xae000
  0007311c: ldr pc,[r12,#0xdd8]!
  000ecb84: b.w 0x001ac1b8
  001ac1b8: bx pc
  001ac1bc: ldr r12,[0x1ac1c4]
  001ac1c0: add pc,r12,pc
```
