#!/bin/bash
set -e

if [ ! -d "build" ]; then
    mkdir build
fi

cd build
cmake ..

echo "Configuración completada."
echo "Compila con: cmake --build ."
echo "Ejecuta con: ./PMP-PROYECTO-1"
