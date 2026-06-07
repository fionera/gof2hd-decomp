# Transform::SetVisible
elf 0x77630 body 6
Sig: undefined __thiscall SetVisible(Transform * this, bool param_1)

## decompile
```c

/* AbyssEngine::Transform::SetVisible(bool) */

void __thiscall AbyssEngine::Transform::SetVisible(Transform *this,bool param_1)

{
  this[0xec] = (Transform)param_1;
  return;
}

```

## target disasm
```
  00087630: strb.w r1,[r0,#0xec]
  00087634: bx lr
```
