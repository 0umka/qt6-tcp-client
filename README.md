qt6-tcp-server

Небольшое Qt 6 консольное приложение-эмулятор для спама json данными по TCP-соединению.

Требования:

Qt 6.5+

CMake 3.19+

C++ компилятор

Сборка:
```
cmake -S . -B build
cmake --build build
cmake --install build
```

После установки готовые файлы будут в папке build/qt6-tcp-client. Запускать нужно qt6-tcp-client.exe из этой папки.
