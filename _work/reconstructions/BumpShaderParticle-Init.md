# `BumpShaderParticle::Init` exclusive reconstruction

## Target

- TU: `src/game/core/BumpShaderParticle.cpp`
- `_ZN11AbyssEngine18BumpShaderParticle4InitEPNS_6EngineE`
- original `0x87dc0`, current `0x9bb98`
- 97.0%, 388/388 bytes

## Recovered lookup layout

The original performs six attribute lookups followed by thirteen uniform
lookups. The reconstruction performs five plus fourteen; both make nineteen
calls and retain the same size.

| Object offset | API | Original literal |
| ---: | --- | --- |
| +32 | `glGetAttribLocation` | `a_position` |
| +36 | `glGetAttribLocation` | `a_texCoord` |
| +40 | `glGetAttribLocation` | `a_normal` |
| +44 | `glGetAttribLocation` | `a_tangent` |
| +48 | `glGetAttribLocation` | `a_bitangent` |
| +52 | `glGetAttribLocation` | `a_VertexColor` |
| +56 | `glGetUniformLocation` | `u_ModelViewProjectionMatrix` |
| +60 | `glGetUniformLocation` | `u_ModelMatrix` |
| +64 | `glGetUniformLocation` | `u_lightdirmodel` |
| +68 | `glGetUniformLocation` | `u_eyeposmodel` |
| +72 | `glGetUniformLocation` | `s_texture[0]` |
| +76 | `glGetUniformLocation` | `s_texture[1]` |
| +80 | `glGetUniformLocation` | `glColor` |
| +84 | `glGetUniformLocation` | `u_AmbientColor` |
| +88 | `glGetUniformLocation` | `u_DiffuseColor` |
| +92 | `glGetUniformLocation` | `u_SpecularColor` |
| +96 | `glGetUniformLocation` | `u_SpecularPower` |
| +100 | `glGetUniformLocation` | `u_TexBiasDiffuse` |
| +104 | `glGetUniformLocation` | `u_TexBiasNormal` |

The attribute and uniform function-pointer pools resolve respectively to the
original GOT slots `0x21000c` and `0x210010`. The sampler pair is the
contiguous object fields at `+72/+76`, not a global table.

The original literal addresses, in call order, are:

```text
0x1ca74a 0x1ca75e 0x1ca755 0x1ca82f 0x1ca839 0x1ca845
0x1ca853 0x1ca775 0x1ca86d 0x1ca783 0x1ca87d 0x1ca88a
0x1ca897 0x1ca7c2 0x1ca7d1 0x1ca7e0 0x1ca7f0 0x1ca89f
0x1ca8b0
```

`ES2LoadProgram` receives inline GLSL rather than the current `.vsh/.fsh`
filenames:

- vertex blob `0x1e86d0`, 1223 bytes,
  SHA-256 `b3111eac32e811247b3a993fbd7af5755e3b0e81a501c6f69645061dfc0807e9`;
- fragment blob `0x1e8b98`, 1181 bytes,
  SHA-256 `89dc30ae67ef2d4c5fc79861e53691b2cb6db643fef90c35655206c785bb6391`.

## Exclusive requeue

Drain the fleet. Change the header to six attribute fields and thirteen
uniform fields, add `a_VertexColor`, remove `uniformU13`, and start the sampler
pair at `uniformU4`. Preserve behavior throughout the TU by remapping current
`uniformU0` color/attribute uses to the sixth attribute and decrementing the
subsequent true uniform indices:

```text
U1->U0 U2->U1 U3->U2 U4->U3
U7->U6 U8->U7 U9->U8 U10->U9 U11->U10 U12->U11 U13->U12
```

Extract and add the two verified inline GLSL blobs in the same pass. Gate the
entire class, layout, shader call sites, exports, and parity before landing.
Confidence is high on the call/field/literal mapping.
