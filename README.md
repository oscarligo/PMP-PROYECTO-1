# CC3086 - Programación de Microprocesadores
## Proyecto 1 del curso
## PMP-PROYECTO-1
### Descripción:
Este proyecto es un videojuego hecho en **C++** 

---
### Dependencias:
- **CMake**  
- **ncurses** (Linux/macOS/WSL)  

### Instalación:

**Linux**
```
sudo apt install build-essential cmake libncurses5-dev libncursesw5-dev
```
**macOS**

```
brew install cmake ncurses
```
**WSL**

```
sudo apt install build-essential cmake libncurses5-dev libncursesw5-dev
```

### Pasos para ejecutar el programa:

1. Configurar el proyecto:

    ```sh
    ./configure.sh
    ```

    Si no se tiene permisos por defecto
    ```
    chmod +x configure.sh
    ./configure.sh
    ```

2. Compilar:

    ```sh
    cmake --build build
    ```
3. Ejecutar:  

    ```sh
    ./build/PMP-PROYECTO-1
    ```
---
## Estructura del proyecto
```
PMP-PROYECTO-1/
│
├── CMakeLists.txt     <- Archivo de CMake que define cómo compilar el proyecto
├── configure.sh       <- Script para configurar el proyecto y generar build/
├── README.md          <- Este archivo con instrucciones
│
├── include/           <- Headers (.h)
│
├── src/               <- Código fuente (.cpp)
│   ├── main.cpp
│   └── ...
│
└── build/             <- Carpeta generada por CMake
    └── PMP-PROYECTO-1 <- Ejecutable final
```