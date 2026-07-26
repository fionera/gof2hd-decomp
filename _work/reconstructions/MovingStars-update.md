# `MovingStars::update` stack-order blocker

## Target

- `_ZN11MovingStars6updateEiN11AbyssEngine6AEMath6MatrixEbf`
- 49.5%, original 1356 bytes / current 1352 bytes
- TU: `src/engine/render/MovingStars.cpp`

The pass restores the real delta parameter, direct signed integer-to-float
conversions, canvas reload pattern, literal-pool constants, bounded random
calls, per-star transform handle, matrix movement fields, and respawn
transform sequence. The six invented `VectorSignedToFloat(int, int)` calls
in this method are gone.

Verified constants include:

- spawn base `-500`
- depth scale `4500.0f`
- sprite bounds `-70.0f`, `500.0f`, `-500.0f`, `70.0f`
- speed and velocity scales `1000.0f`
- respawn depth `20000.0f`
- random bounds `20000`, `18000`, and `500`

The remaining frame mismatch is stack-object ordering:

| Object | Original | Current |
| --- | ---: | ---: |
| shared Matrix / hidden-return scratch | `sp+0x18` | `sp+0x28` |
| respawn Vector | `sp+0x54` | `sp+0x1c` |
| copied by-value Matrix argument | `sp+0x60` | `sp+0x68` |
| stack canary | `sp+0x9c` | `sp+0xa4` |
| total frame | 160 bytes | 168 bytes |

The branch-local Matrix, `MatrixTransformVector` result, and discarded
`MatrixSetTranslation` result already share one 60-byte slot. Two retries
found no honest typed C++ form that reverses the scratch/Vector allocation
order without adding another Matrix lifetime, default construction and
assignment, volatility, placement-new plumbing, or an artificial use.

Requeue only with new compiler-allocation evidence. Do not replace the typed
objects with lifetime tricks solely to force stack placement.
