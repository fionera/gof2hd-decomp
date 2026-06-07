# KIPlayer::outerCollide
elf 0xa5224 body 28
Sig: undefined __thiscall outerCollide(KIPlayer * this, Vector * param_1)

## decompile
```c

/* KIPlayer::outerCollide(AbyssEngine::AEMath::Vector const&) */

void __thiscall KIPlayer::outerCollide(KIPlayer *this,Vector *param_1)

{
                    /* WARNING: Could not recover jumptable at 0x000b523e. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(*(int *)this + 0x3c))
            (this,*(undefined4 *)param_1,*(undefined4 *)(param_1 + 4),*(undefined4 *)(param_1 + 8));
  return;
}

```

## target disasm
```
  000b5224: push {r7,lr}
  000b5226: mov r7,sp
  000b5228: ldr.w lr,[r0,#0x0]
  000b522c: ldrd r12,r2,[r1,#0x0]
  000b5230: ldr r3,[r1,#0x8]
  000b5232: ldr.w lr,[lr,#0x3c]
  000b5236: mov r1,r12
  000b5238: mov r12,lr
  000b523a: pop.w {r7,lr}
  000b523e: bx r12
```
