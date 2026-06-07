# ApplicationManager::SoundSet
elf 0x82148 body 52
Sig: undefined __thiscall SoundSet(ApplicationManager * this, AESoundInfo * param_1, int param_2)

## decompile
```c

/* AbyssEngine::ApplicationManager::SoundSet(AbyssEngine::AESoundInfo const*, int) */

void __thiscall
AbyssEngine::ApplicationManager::SoundSet(ApplicationManager *this,AESoundInfo *param_1,int param_2)

{
  EVP_PKEY_CTX *ctx;
  
  if ((param_1 != (AESoundInfo *)0x0) &&
     (*(AESoundRessource **)(this + 0xac) != (AESoundRessource *)0x0)) {
    AESoundRessource::SetSound(*(AESoundRessource **)(this + 0xac),param_1,param_2);
    for (ctx = (EVP_PKEY_CTX *)0x0; (int)ctx < param_2; ctx = ctx + 1) {
      AESoundRessource::init(*(AESoundRessource **)(this + 0xac),ctx);
    }
  }
  return;
}

```

## target disasm
```
  00092148: push {r4,r5,r6,r7,lr}
  0009214a: add r7,sp,#0xc
  0009214c: push.w r11
  00092150: mov r4,r2
  00092152: mov r5,r0
  00092154: cbz r1,0x00092176
  00092156: ldr.w r0,[r5,#0xac]
  0009215a: cbz r0,0x00092176
  0009215c: mov r2,r4
  0009215e: blx 0x000702f4
  00092162: movs r6,#0x0
  00092164: b 0x00092172
  00092166: ldr.w r0,[r5,#0xac]
  0009216a: mov r1,r6
  0009216c: blx 0x000700d8
  00092170: adds r6,#0x1
  00092172: cmp r6,r4
  00092174: blt 0x00092166
  00092176: pop.w r11
  0009217a: pop {r4,r5,r6,r7,pc}
```
