#!/usr/bin/env python3
"""
aei.py — reader/converter for AbyssEngine .aei texture files (Galaxy on Fire 2).

Format reverse-engineered from AbyssEngine::ImageCreateFromFile (Android libgof2hdaa.so @0x7f54c):

  char[8]  magic   = "AEimage\0"
  u8       format_code
  u16      width            (LE)
  u16      height           (LE)
  u16      n_meta           (LE)   ; followed by n_meta*8 bytes that the loader skips
  <pixel data>:
    uncompressed RGBA8888  (codes 0x01,0x03,0x40,0x42,0x81): width*height*4 bytes
    compressed/sized       (other codes): u32 length (LE) + <length> bytes (GPU-compressed:
                                           PVRTC / ETC1 / etc., platform-specific)

Usage:
  aei.py info  <file.aei|dir> ...     # parse + summarize headers (validation)
  aei.py png   <file.aei> [out.png]   # convert uncompressed RGBA -> PNG
  aei.py raw   <file.aei> [out.bin]   # dump the raw pixel/compressed payload
"""
import sys, struct, zlib, os, glob

MAGIC = b"AEimage\x00"
UNCOMPRESSED = {0x01, 0x03, 0x40, 0x42, 0x81}   # width*height*4 RGBA8888

def parse(data):
    if data[:8] != MAGIC:
        raise ValueError("bad magic %r" % data[:8])
    fmt = data[8]
    w, h, n = struct.unpack_from('<HHH', data, 9)
    off = 15 + n * 8
    info = dict(fmt=fmt, w=w, h=h, n_meta=n, data_off=off, total=len(data))
    if fmt in UNCOMPRESSED:
        size = w * h * 4
        info.update(kind='rgba8888', payload_off=off, size=size,
                    end=off + size, trailing=len(data) - (off + size))
    else:
        if off + 4 <= len(data):
            ln = struct.unpack_from('<I', data, off)[0]
        else:
            ln = 0
        info.update(kind='compressed', payload_off=off + 4, size=ln,
                    end=off + 4 + ln, trailing=len(data) - (off + 4 + ln))
    return info

def write_png(path, w, h, rgba):
    def chunk(typ, body):
        c = typ + body
        return struct.pack('>I', len(body)) + c + struct.pack('>I', zlib.crc32(c) & 0xffffffff)
    raw = bytearray()
    stride = w * 4
    for y in range(h):
        raw.append(0)                       # filter: none
        raw += rgba[y*stride:(y+1)*stride]
    png = b'\x89PNG\r\n\x1a\n'
    png += chunk(b'IHDR', struct.pack('>IIBBBBB', w, h, 8, 6, 0, 0, 0))  # 8-bit RGBA
    png += chunk(b'IDAT', zlib.compress(bytes(raw), 9))
    png += chunk(b'IEND', b'')
    open(path, 'wb').write(png)

def cmd_info(args):
    files = []
    for a in args:
        files += glob.glob(os.path.join(a, '*.aei')) if os.path.isdir(a) else [a]
    fmts = {}
    bad = 0
    for f in files:
        try:
            i = parse(open(f, 'rb').read())
        except Exception as e:
            print("%-40s ERR %s" % (os.path.basename(f), e)); bad += 1; continue
        fmts[i['fmt']] = fmts.get(i['fmt'], 0) + 1
        if len(files) <= 20:
            print("%-34s fmt=0x%02x %4dx%-4d n_meta=%d off=%d size=%d trailing=%d kind=%s"
                  % (os.path.basename(f), i['fmt'], i['w'], i['h'], i['n_meta'],
                     i['data_off'], i['size'], i['trailing'], i['kind']))
    print("\n%d files, %d bad. format_code histogram:" % (len(files), bad))
    for k in sorted(fmts):
        print("  0x%02x : %d  (%s)" % (k, fmts[k], 'RGBA8888' if k in UNCOMPRESSED else 'compressed'))

def cmd_png(args):
    f = args[0]; out = args[1] if len(args) > 1 else f.rsplit('.', 1)[0] + '.png'
    i = parse(open(f, 'rb').read())
    if i['kind'] != 'rgba8888':
        print("not uncompressed (fmt=0x%02x, %s); use 'raw'" % (i['fmt'], i['kind'])); return 2
    data = open(f, 'rb').read()
    write_png(out, i['w'], i['h'], data[i['payload_off']:i['payload_off']+i['size']])
    print("wrote %s (%dx%d)" % (out, i['w'], i['h']))

def cmd_raw(args):
    f = args[0]; out = args[1] if len(args) > 1 else f.rsplit('.', 1)[0] + '.payload.bin'
    i = parse(open(f, 'rb').read()); data = open(f, 'rb').read()
    open(out, 'wb').write(data[i['payload_off']:i['payload_off']+i['size']])
    print("wrote %s (%d bytes, fmt=0x%02x %s)" % (out, i['size'], i['fmt'], i['kind']))

if __name__ == '__main__':
    if len(sys.argv) < 3:
        print(__doc__); sys.exit(1)
    {'info': cmd_info, 'png': cmd_png, 'raw': cmd_raw}[sys.argv[1]](sys.argv[2:])
