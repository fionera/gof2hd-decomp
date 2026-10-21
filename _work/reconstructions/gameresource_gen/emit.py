"""C++ spelling of one recovered resource entry (shared by the gen_*.py generators)."""

HEADER_INCLUDES = '''#include "engine/render/Resource.h"
#include "engine/render/ResourceImage.h"
#include "engine/render/ResourceTexture.h"
#include "engine/render/ResourceMaterial.h"
#include "engine/render/ResourceMesh.h"
#include "engine/render/Engine.h"
#include "engine/core/ApplicationManager.h"
#include "engine/render/PaintCanvas.h"
#include "game/core/Globals.h"'''

KIND = {1: 'RESOURCE_FONT', 2: 'RESOURCE_TEXTURE', 3: 'RESOURCE_IMAGE', 4: 'RESOURCE_MESH',
        6: 'RESOURCE_MATERIAL'}
BLEND = {1: 'BlendMode_1', 2: 'BlendMode_2', 8: 'BlendMode_8', 0x15: 'BlendMode_0x15'}


def cstr_lit(s):
    return '"' + s.replace('\\', '\\\\').replace('"', '\\"') + '"'


def flt(f):
    if f == 0.0:
        return '0.0f'
    r = repr(f)
    if '.' not in r and 'e' not in r:
        r += '.0'
    return r + 'f'


def blend(b):
    return BLEND.get(b, 'static_cast<BlendMode>(%d)' % b)


def payload_expr(p):
    t = p[0]
    if t == 'tex':
        return 'new ResourceTexture(%s, %s)' % (cstr_lit(p[1]), flt(p[2]))
    if t == 'mesh':
        return 'new ResourceMesh(%s, %d, %s)' % (cstr_lit(p[1]), p[2], 'true' if p[3] else 'false')
    if t == 'mat1':
        return 'new ResourceMaterial(%d, %s)' % (p[1], blend(p[2]))
    if t == 'mat2':
        return 'new ResourceMaterial(%d, %d, %s)' % (p[1], p[2], blend(p[3]))
    if t == 'image':
        return 'new ResourceImage(%d, %d)' % (p[1], p[2])
    raise ValueError(p)


def entry_expr(r):
    rid, kind, unused, p = r
    assert unused == 0xffffffff, r
    return 'new Resource(%d, %s, %s)' % (rid, KIND[kind], payload_expr(p))
