@echo off
:: ============================================================
::  graphics.h (WinBGIm) автомат суулгагч
::  Dev-C++ дээр graphics.h ашиглахад бэлэн болгоно
:: ============================================================

echo.
echo  *** graphics.h (WinBGIm) суулгагч ***
echo.

:: --- Dev-C++ байгаа газрыг олох ---
set DEVCPP=
if exist "C:\Program Files (x86)\Dev-Cpp\MinGW64\include" (
    set DEVCPP=C:\Program Files (x86)\Dev-Cpp\MinGW64
)
if exist "C:\Program Files\Dev-Cpp\MinGW64\include" (
    set DEVCPP=C:\Program Files\Dev-Cpp\MinGW64
)
if exist "C:\Dev-Cpp\MinGW64\include" (
    set DEVCPP=C:\Dev-Cpp\MinGW64
)

if "%DEVCPP%"=="" (
    echo  [АЛДАА] Dev-C++ олдсонгүй!
    echo  Гараар замыг оруулна уу:
    set /p DEVCPP=  Dev-C++ MinGW64 зам:
)

echo.
echo  Dev-C++ зам: %DEVCPP%
echo.

:: --- Файлуудыг хуулах ---
echo  [1/3] graphics.h хуулж байна...
copy /Y "%~dp0graphics.h" "%DEVCPP%\include\graphics.h" >nul
if errorlevel 1 ( echo  [АЛДАА] graphics.h хуулж чадсангүй! Администратороор ажиллуулна уу. & pause & exit )
echo       OK

echo  [2/3] winbgim.h хуулж байна...
copy /Y "%~dp0winbgim.h" "%DEVCPP%\include\winbgim.h" >nul
if errorlevel 1 ( echo  [АЛДАА] winbgim.h хуулж чадсангүй! & pause & exit )
echo       OK

echo  [3/3] libbgi.a хуулж байна...
copy /Y "%~dp0libbgi.a" "%DEVCPP%\lib\libbgi.a" >nul
if errorlevel 1 ( echo  [АЛДАА] libbgi.a хуулж чадсангүй! & pause & exit )
echo       OK

echo.
echo  ============================================================
echo   Суулгалт амжилттай!
echo  ============================================================
echo.
echo   Одоо Dev-C++ дээр:
echo   Tools ^> Compiler Options ^> Linker талбарт нэмнэ:
echo.
echo     -lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32
echo.
echo   Тест хийхийн тулд test_graphics.cpp файлыг нээгээд F11.
echo.
pause
