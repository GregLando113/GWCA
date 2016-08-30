@echo off
call vcvarsall

cl /c /Zi Dependencies\disasm\ld32.c
for /R GWCA\src\ %%G in (*.cpp) do cl /Zi /EHsc /DUNICODE /c %%G
lib -nologo -out:GWCAd.lib *.obj
del *.obj