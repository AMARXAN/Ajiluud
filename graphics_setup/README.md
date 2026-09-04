# Dev-C++ дээр graphics.h суулгах заавар

## Алхам 1 — WinBGIm татах

**graphics.h**, **winbgim.h**, **libbgi.a** файлуудыг доорх холбоосоос татна:

🔗 https://winbgim.codecutter.org/

эсвэл шууд татах:

🔗 https://github.com/krist7599555/winbgim/archive/refs/heads/master.zip

## Алхам 2 — Файлуудыг хуулах

| Файл | Хуулах газар |
|------|-------------|
| `graphics.h` | `C:\Program Files (x86)\Dev-Cpp\MinGW64\include\` |
| `winbgim.h`  | `C:\Program Files (x86)\Dev-Cpp\MinGW64\include\` |
| `libbgi.a`   | `C:\Program Files (x86)\Dev-Cpp\MinGW64\lib\` |

> 💡 **INSTALL.bat** файлыг **Администратороор ажиллуулвал** автоматаар хуулна.

## Алхам 3 — Dev-C++ тохиргоо

1. Dev-C++ нээнэ
2. **Tools → Compiler Options** цонх нээнэ
3. **"Add the following commands when calling linker"** талбарт:

```
-lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32
```

4. **OK** дарна

## Алхам 4 — Тест

`test_graphics.cpp` файлыг Dev-C++ дээр нээгээд **F11** дарна.  
Цонх гарч 4 өнгийн тойрог харагдвал амжилттай суусан!

## graphics.h Ludo тоглоом

Суулгасны дараа `ludo_graphics.cpp` файлыг нээгээд **F11** дарна.
