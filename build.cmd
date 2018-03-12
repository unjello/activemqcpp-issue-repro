@ECHO OFF
GOTO:%~1 2>NUL
IF ERRORLEVEL 1 (
	ECHO Invalid argument: %1
	ECHO.
	ECHO Usage:  %~n0  BuildType
	ECHO.
	ECHO Where:  BuildType may be Debug, Release or Clean
	GOTO:EOF
)

:Debug
DEL /F *.ilk *.pdb *.obj
cl.exe  /nologo /TP -D_WIN32 -I.\opt\activemq-cpp\inc -I.\opt\activemq-cpp\apr\inc\apr /DWIN32 /D_WINDOWS /W3 /GR /EHsc /MDd /Zi /Ob0 /Od /RTC1 -std:c++17 /showIncludes /FS -c main.cpp
link.exe /nologo main.obj  /out:activemq_issued.exe /version:0.0  /machine:X86 /debug /INCREMENTAL /subsystem:console  .\opt\activemq-cpp\lib\Debug\activemq-cppd.lib kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comdlg32.lib advapi32.lib
GOTO:EOF

:Release
DEL /F *.ilk *.pdb *.obj
cl.exe  /nologo /TP -D_WIN32 -I.\opt\activemq-cpp\inc -I.\opt\activemq-cpp\apr\inc\apr /DWIN32 /D_WINDOWS /W3 /GR /EHsc /MD /Zi /O2 /Ob1 /DNDEBUG -std:c++17 /showIncludes /FS -c main.cpp
link.exe /nologo main.obj  /out:activemq_issue.exe /version:0.0  /machine:X86 /debug /INCREMENTAL /subsystem:console  .\opt\activemq-cpp\lib\Release\activemq-cpp.lib kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comdlg32.lib advapi32.lib
GOTO:EOF

:Clean
DEL /F *.ilk *.pdb *.obj
GOTO:EOF