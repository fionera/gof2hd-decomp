# PlayerStaticFar::render
elf 0x11c2da body 26
Sig: undefined __stdcall render(void)

## decompile
```c

/* PlayerStaticFar::render() */

void PlayerStaticFar::render(void)

{
                    /* WARNING: Could not recover jumptable at 0x001ac380. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_001ac384 + 0x1ac388))();
  return;
}

```

## target disasm
```
  00075a6c: adr r12,0x175a74
  00075a70: add r12,r12,#0xad000
  00075a74: ldr pc,[r12,#0x248]!
  0012c2da: b.w 0x001ac378
  001ac378: bx pc
  001ac37c: ldr r12,[0x1ac384]
  001ac380: add pc,r12,pc
```
