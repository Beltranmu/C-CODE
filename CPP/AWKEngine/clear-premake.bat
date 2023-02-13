
attrib +r .\bin\HelloSound\fmod.dll
attrib +r .\bin\HelloSound\fmodL.dll
del ".\build\" /q /s /f
del ".\bin\" /q /s /f
attrib -r .\bin\HelloSound\fmod.dll
attrib -r .\bin\HelloSound\fmodL.dll
@echo delete done :D
pause