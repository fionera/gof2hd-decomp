# PlayerFixedObject::setPosition
elf 0x153fb4 body 28
Sig: undefined __thiscall setPosition(PlayerFixedObject * this, Vector * param_1)

## decompile
```c

/* PlayerFixedObject::setPosition(AbyssEngine::AEMath::Vector const&) */

void __thiscall PlayerFixedObject::setPosition(PlayerFixedObject *this,Vector *param_1)

{
                    /* WARNING: Could not recover jumptable at 0x00163fce. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(*(int *)this + 0x48))
            (this,*(undefined4 *)param_1,*(undefined4 *)(param_1 + 4),*(undefined4 *)(param_1 + 8));
  return;
}

```

## target disasm
```
  00163fb4: push {r7,lr}
  00163fb6: mov r7,sp
  00163fb8: ldr.w lr,[r0,#0x0]
  00163fbc: ldrd r12,r2,[r1,#0x0]
  00163fc0: ldr r3,[r1,#0x8]
  00163fc2: ldr.w lr,[lr,#0x48]
  00163fc6: mov r1,r12
  00163fc8: mov r12,lr
  00163fca: pop.w {r7,lr}
  00163fce: bx r12
```
