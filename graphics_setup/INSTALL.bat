@echo off
echo.
echo  *** graphics.h (WinBGIm) суулгагч ***
echo.
set DEVCPP=
if exist "C:\Program Files (x86)\Dev-Cpp\MinGW64\include" set DEVCPP=C:\Program Files (x86)\Dev-Cpp\MinGW64
if exist "C:\Program Files\Dev-Cpp\MinGW64\include" set DEVCPP=C:\Program Files\Dev-Cpp\MinGW64
if exist "C:\Dev-Cpp\MinGW64\include" set DEVCPP=C:\Dev-Cpp\MinGW64
if "%DEVCPP%"=="" (echo  [АЛДАА] Dev-C++ олдсонгүй! & set /p DEVCPP=  Dev-C++ MinGW64 зам: )
echo  Dev-C++ зам: %DEVCPP%
echo  [1/3] graphics.h...
copy /Y "%~dp0graphics.h" "%DEVCPP%\include\graphics.h" >nul
if errorlevel 1 (echo  [АЛДАА] Администратороор ажиллуулна уу! & pause & exit)
echo       OK
echo  [2/3] winbgim.h...
copy /Y "%~dp0winbgim.h" "%DEVCPP%\include\winbgim.h" >nul
if errorlevel 1 (echo  [АЛДАА] & pause & exit)
echo       OK
echo  [3/3] libbgi.a...
copy /Y "%~dp0libbgi.a" "%DEVCPP%\lib\libbgi.a" >nul
if errorlevel 1 (echo  [АЛДАА] & pause & exit)
echo       OK
echo.
echo  Суулгалт амжилттай!
echo  Tools > Compiler Options > Linker талбарт нэмнэ:
echo  -lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32
pause
