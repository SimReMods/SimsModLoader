"""Assemble the release zips from the built DLLs, the mods' data and the release READMEs: one zip per mod per game.

    python package.py [--version 1.2.3]   -> dist/<mod>_<platform>.zip, e.g. eyedropper_steam.zip, eyedropper_ea.zip,
                                             eyedropper_cc.zip; seasons_<platform>-DO-NOT-DOWNLOAD.zip (for the Seasons
                                             installer, not for players)

platform: steam = The Sims Legacy Collection from Steam, ea = the same from the EA app, cc = The Sims Complete Collection.
Each zip holds the loader (winmm.dll), mods/<mod>_<platform>.dll, the mod's data and its README, filled in for that game
and with the version (--version, else the MOD_VERSION environment variable, else 0.0.0) on its first line.

The names carry no version, so .../releases/latest/download/<name> always gets the newest release.

Run after the build.bat files. Entry names use forward slashes; files are added in sorted order with a fixed
timestamp, so the same inputs always give the same zip.
"""
import argparse
import os
import sys
import zipfile

ROOT = os.path.dirname(os.path.abspath(__file__))
FIXED_TIME = (2000, 1, 1, 0, 0, 0)

# platform: (build subfolder of its DLL, the game in the README)
PLATFORMS = {
    'steam': ('', 'The Sims Legacy Collection from Steam (version 1.10.7.0)'),
    'ea': ('ea', 'The Sims Legacy Collection from the EA app (version 1.10.7)'),
    'cc': ('cc', 'The Sims Complete Collection (2003)'),
}

# mod folder: (README template in release/, platforms, zip name suffix)
PACKAGES = {
    'visit_lots': ('README-MOD.txt', ('steam', 'ea', 'cc'), ''),
    'multistory': ('README-MULTISTORY.txt', ('steam', 'ea'), ''),
    'eyedropper': ('README-EYEDROPPER.txt', ('steam', 'ea', 'cc'), ''),
    'all_cheats': ('README-ALLCHEATS.txt', ('steam', 'ea', 'cc'), ''),
    # the Seasons installer (github.com/SimReMods/Seasons) downloads these; players install that, not these zips
    'seasons': ('README-SEASONS.txt', ('steam', 'ea', 'cc'), '-DO-NOT-DOWNLOAD'),
}


def collect(mod, platform, readme):
    sub, _ = PLATFORMS[platform]
    dll = '%s_%s.dll' % (mod, platform)
    files = {
        'winmm.dll': os.path.join(ROOT, 'modloader', 'build', 'winmm.dll'),
        'mods/' + dll: os.path.join(ROOT, 'mods', mod, 'build', sub, dll),
    }
    data = os.path.join(ROOT, 'mods', mod, 'data')
    for base, _, names in os.walk(data):
        for name in names:
            if name.lower().startswith('orig_'):          # the game's own art, kept next to ours for reference only
                continue
            path = os.path.join(base, name)
            files[os.path.relpath(path, data).replace(os.sep, '/')] = path
    return files


def readme_text(template, mod, platform, version):
    game = PLATFORMS[platform][1]
    text = open(os.path.join(ROOT, 'release', template), encoding='utf-8').read()
    cracked = '' if platform == 'cc' else '; cracked versions are not supported'   # the Legacy builds only
    for key, value in (('{VERSION}', version), ('{GAME_UPPER}', game.upper()), ('{GAME}', game), ('{CRACKED}', cracked),
                       ('{DLL}', '%s_%s.dll' % (mod, platform))):
        text = text.replace(key, value)
    return text.replace('\r\n', '\n').replace('\n', '\r\n').encode('utf-8')


def write_zip(out, files, readme_name, readme):
    with zipfile.ZipFile(out, 'w', zipfile.ZIP_DEFLATED) as z:
        entries = dict(files)
        entries[readme_name] = None
        for arc in sorted(entries):
            info = zipfile.ZipInfo(arc, FIXED_TIME)
            info.compress_type = zipfile.ZIP_DEFLATED
            info.external_attr = 0o644 << 16
            if entries[arc] is None:
                content = readme
            else:
                with open(entries[arc], 'rb') as f:
                    content = f.read()
            z.writestr(info, content)
            print('%9d  %s' % (len(content), arc))
    print('wrote', out)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--version', default=os.environ.get('MOD_VERSION') or '0.0.0')
    a = ap.parse_args()
    version = a.version[1:] if a.version[:1] in 'vV' else a.version
    jobs = []
    for mod, (readme, platforms, suffix) in PACKAGES.items():
        for platform in platforms:
            jobs.append(('%s_%s%s.zip' % (mod, platform, suffix), collect(mod, platform, readme), readme,
                         readme_text(readme, mod, platform, version)))
    missing = [p for _, files, _, _ in jobs for p in files.values() if not os.path.isfile(p)]
    if missing:
        sys.exit('missing (run the build.bat files first): ' + ', '.join(missing))
    os.makedirs(os.path.join(ROOT, 'dist'), exist_ok=True)
    for name, files, readme, text in jobs:
        write_zip(os.path.join(ROOT, 'dist', name), files, readme, text)


if __name__ == '__main__':
    main()
