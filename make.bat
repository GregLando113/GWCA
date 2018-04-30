@echo off

if not defined DevEnvDir (
call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x86
)

for /R GWCA\src\ %%G in (*.cpp) do cl /nologo /O2 /EHsc /I.\ /DUNICODE /c %%G
for /R Dependencies\minhook\src\ %%G in (*.c) do cl /nologo /O2 /EHsc /I.\ /DUNICODE /c %%G
for /R Dependencies\minhook\src\hde\ %%G in (*.c) do cl /nologo /O2 /EHsc /I.\ /DUNICODE /c %%G
lib -nologo -out:GWCA.lib *.obj
del *.obj