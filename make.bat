@echo off

if not defined DevEnvDir (

call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" x86

)

cl /nologo /c /Zi Dependencies\disasm\ld32.c
for /R GWCA\src\ %%G in (*.cpp) do cl /nologo /EHsc /I.\ /DUNICODE /c %%G
lib -nologo -out:GWCA.lib *.obj
del *.obj