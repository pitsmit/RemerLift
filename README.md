# Remer

## Структура проекта

```text
Remer/
├── CMakeLists.txt
├── README.md
├── src/              # Исходный код приложения
├── tests/            # тесты
└── build/            # Файлы сборки (генерируются CMake)
```

## Сборка проекта

```bash
cmake -S . -B build
cmake --build build
```

## Запуск тестов

```bash
ctest --test-dir build --output-on-failure
```

## Запуск приложения

```bash
./build/Lift
```