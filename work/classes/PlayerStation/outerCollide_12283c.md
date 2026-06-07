# PlayerStation::outerCollide
elf 0x12283c body 28
Sig: undefined __thiscall outerCollide(PlayerStation * this, Vector * param_1)

## decompile
```c

/* PlayerStation::outerCollide(AbyssEngine::AEMath::Vector const&) */

void __thiscall PlayerStation::outerCollide(PlayerStation *this,Vector *param_1)

{
                    /* WARNING: Could not recover jumptable at 0x00132856. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(*(int *)this + 0x3c))
            (this,*(undefined4 *)param_1,*(undefined4 *)(param_1 + 4),*(undefined4 *)(param_1 + 8));
  return;
}

```

## target disasm
```
  0013283c: push {r7,lr}
  0013283e: mov r7,sp
  00132840: ldr.w lr,[r0,#0x0]
  00132844: ldrd r12,r2,[r1,#0x0]
  00132848: ldr r3,[r1,#0x8]
  0013284a: ldr.w lr,[lr,#0x3c]
  0013284e: mov r1,r12
  00132850: mov r12,lr
  00132852: pop.w {r7,lr}
  00132856: bx r12
```
