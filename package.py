"""Assemble the release zips from the built DLLs, the mods' data and the release READMEs.

    python package.py      -> dist/sims-visitlots.zip, dist/sims-multistory.zip, dist/sims-eyedropper.zip

The names carry no version, so .../releases/latest/download/<name> always gets the newest release.

Run after the build.bat files. Entry names use forward slashes; files are added in sorted order with a fixed
timestamp, so the same inputs always give the same zip.
"""
import os
import sys
import zipfile

ROOT = os.path.dirname(os.path.abspath(__file__))
FIXED_TIME = (2000, 1, 1, 0, 0, 0)

PACKAGES = {
    'visitlots': ('visit_lots', 'visit_lots.dll', 'README-MOD.txt'),
    'multistory': ('multistory', 'multistory.dll', 'README-MULTISTORY.txt'),
    'eyedropper': ('eyedropper', 'eyedropper.dll', 'README-EYEDROPPER.txt'),
}


def collect(mod_dir, dll, readme):
    files = {
        'winmm.dll': os.path.join(ROOT, 'modloader', 'build', 'winmm.dll'),
        'mods/' + dll: os.path.join(ROOT, 'mods', mod_dir, 'build', dll),
        readme: os.path.join(ROOT, 'release', readme),
    }
    data = os.path.join(ROOT, 'mods', mod_dir, 'data')
    for base, _, names in os.walk(data):
        for name in names:
            if name.lower().startswith('orig_'):          # the game's own art, kept next to ours for reference only
                continue
            path = os.path.join(base, name)
            files[os.path.relpath(path, data).replace(os.sep, '/')] = path
    return files


def write_zip(out, files):
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


def main():
    packages = {name: collect(*spec) for name, spec in PACKAGES.items()}
    missing = [p for files in packages.values() for p in files.values() if not os.path.isfile(p)]
    if missing:
        sys.exit('missing (run the build.bat files first): ' + ', '.join(missing))
    os.makedirs(os.path.join(ROOT, 'dist'), exist_ok=True)
    for name, files in packages.items():
        write_zip(os.path.join(ROOT, 'dist', 'sims-%s.zip' % name), files)


if __name__ == '__main__':
    main()
