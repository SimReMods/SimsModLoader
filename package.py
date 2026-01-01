"""Assemble the release zip from the built DLLs, the visit_lots data and release/README-MOD.txt.

    python package.py [version]      -> dist/sims-visitlots-<version>.zip

Run after the three build.bat files. Entry names use forward slashes; files are added in sorted order with a fixed
timestamp, so the same inputs always give the same zip.
"""
import os
import sys
import zipfile

ROOT = os.path.dirname(os.path.abspath(__file__))
FIXED_TIME = (2000, 1, 1, 0, 0, 0)


def main():
    version = sys.argv[1] if len(sys.argv) > 1 else 'dev'
    files = {
        'winmm.dll': os.path.join(ROOT, 'modloader', 'build', 'winmm.dll'),
        'mods/visit_lots.dll': os.path.join(ROOT, 'mods', 'visit_lots', 'build', 'visit_lots.dll'),
        'README-MOD.txt': os.path.join(ROOT, 'release', 'README-MOD.txt'),
    }
    data = os.path.join(ROOT, 'mods', 'visit_lots', 'data')
    for base, _, names in os.walk(data):
        for name in names:
            path = os.path.join(base, name)
            files[os.path.relpath(path, data).replace(os.sep, '/')] = path
    missing = [p for p in files.values() if not os.path.isfile(p)]
    if missing:
        sys.exit('missing (run the build.bat files first): ' + ', '.join(missing))

    os.makedirs(os.path.join(ROOT, 'dist'), exist_ok=True)
    out = os.path.join(ROOT, 'dist', 'sims-visitlots-%s.zip' % version)
    with zipfile.ZipFile(out, 'w', zipfile.ZIP_DEFLATED) as z:
        for arc in sorted(files):
            info = zipfile.ZipInfo(arc, FIXED_TIME)
            info.compress_type = zipfile.ZIP_DEFLATED
            info.external_attr = 0o644 << 16
            with open(files[arc], 'rb') as f:
                content = f.read()
            if arc.endswith('.txt'):
                content = content.replace(b'\r\n', b'\n').replace(b'\n', b'\r\n')
            z.writestr(info, content)
    for arc in sorted(files):
        print('%9d  %s' % (os.path.getsize(files[arc]), arc))
    print('wrote', out)


if __name__ == '__main__':
    main()
