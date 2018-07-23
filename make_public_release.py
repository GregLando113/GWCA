import os
import sys
import shutil
import datetime as d

# call it from the parent repository i.e
# - you are there
# -- GWCA
# --- GWCA
# ---- Context
# ---- ...
# -- GWCA-Public (optional)

pub_dir = 'GWCA-Public'
include_dirs = [
    'GWCA\\Constants',
    'GWCA\\Context',
    'GWCA\\GameContainers',
    'GWCA\\GameEntities',
    'GWCA\\Managers',
    'GWCA\\Packets',
    'GWCA\\Utilities'
]

os.system('GWCA\\make.bat')

for folder in include_dirs:
    folder_path = os.path.join('GWCA', folder)
    for root, dirs, files in os.walk(folder_path):
        for file in files:
            dest = os.path.join(pub_dir, folder)
            if not os.path.exists(dest):
                os.makedirs(dest)
            print('Copying', file, 'to', dest, '...')
            shutil.copy(os.path.join(root, file), os.path.join(dest, file))

shutil.copy('GWCA\\GWCA.lib', os.path.join(pub_dir, 'GWCA.lib'))

os.chdir(pub_dir)
os.system('git commit -a -m "Updated to commit at %s"' % d.datetime.now().isoformat(timespec='seconds').replace(':','-'))
os.system('git push origin master')