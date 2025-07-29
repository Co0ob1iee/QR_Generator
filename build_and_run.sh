#!/bin/bash

# Skrypt automatycznej kompilacji i uruchomienia Generator Kodów QR

set -e  # Zatrzymaj skrypt w przypadku błędu

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_NAME="Generator Kodów QR"

echo "========================================"
echo "$PROJECT_NAME - Kompilacja i uruchomienie"
echo "========================================"

# Funkcja sprawdzania zależności
check_dependencies() {
    echo "🔍 Sprawdzanie zależności..."
    
    local missing_deps=0
    
    # Sprawdź cmake
    if ! command -v cmake &> /dev/null; then
        echo "❌ cmake nie jest zainstalowany"
        missing_deps=1
    else
        echo "✅ cmake: $(cmake --version | head -n1 | cut -d' ' -f3)"
    fi
    
    # Sprawdź kompilator
    if ! command -v g++ &> /dev/null; then
        echo "❌ g++ nie jest zainstalowany"
        missing_deps=1
    else
        echo "✅ g++: $(g++ --version | head -n1 | cut -d' ' -f4)"
    fi
    
    # Sprawdź Qt6
    if ! pkg-config --exists Qt6Core Qt6Widgets Qt6Gui; then
        echo "❌ Qt6 nie jest zainstalowane lub niepełne"
        missing_deps=1
    else
        echo "✅ Qt6: $(pkg-config --modversion Qt6Core)"
    fi
    
    # Sprawdź OpenCV
    if ! pkg-config --exists opencv4 && ! pkg-config --exists opencv; then
        echo "❌ OpenCV nie jest zainstalowane"
        missing_deps=1
    else
        if pkg-config --exists opencv4; then
            echo "✅ OpenCV: $(pkg-config --modversion opencv4)"
        else
            echo "✅ OpenCV: $(pkg-config --modversion opencv)"
        fi
    fi
    
    # Sprawdź qrencode
    if ! pkg-config --exists libqrencode; then
        echo "❌ libqrencode nie jest zainstalowane"
        missing_deps=1
    else
        echo "✅ qrencode: $(pkg-config --modversion libqrencode)"
    fi
    
    if [ $missing_deps -eq 1 ]; then
        echo ""
        echo "❌ Brakuje wymaganych zależności!"
        echo "🔧 Uruchom skrypt instalacji zależności:"
        echo "   ./install_dependencies.sh"
        echo ""
        echo "📖 Lub zainstaluj ręcznie zgodnie z README.md"
        exit 1
    fi
    
    echo "✅ Wszystkie zależności są dostępne!"
}

# Funkcja kompilacji CMake
build_cmake() {
    echo ""
    echo "🔨 Kompilacja za pomocą CMake..."
    
    # Przejdź do katalogu projektu
    cd "$SCRIPT_DIR"
    
    # Stwórz katalog build
    if [ -d "build" ]; then
        echo "🧹 Czyszczenie poprzedniej kompilacji..."
        rm -rf build
    fi
    
    mkdir build
    cd build
    
    # Konfiguracja CMake
    echo "⚙️ Konfiguracja CMake..."
    cmake .. -DCMAKE_BUILD_TYPE=Release
    
    # Kompilacja
    echo "🔧 Kompilacja..."
    make -j$(nproc) || make  # Fallback do single-threaded jeśli parallel nie działa
    
    echo "✅ Kompilacja zakończona pomyślnie!"
    
    # Sprawdź czy plik wykonywalny został utworzony
    if [ -f "bin/qr-generator" ]; then
        echo "📁 Plik wykonywalny: $SCRIPT_DIR/build/bin/qr-generator"
    else
        echo "❌ Nie znaleziono pliku wykonywalnego!"
        exit 1
    fi
}

# Funkcja kompilacji Makefile
build_makefile() {
    echo ""
    echo "🔨 Kompilacja za pomocą Makefile..."
    
    # Przejdź do katalogu projektu
    cd "$SCRIPT_DIR"
    
    # Wyczyść poprzednią kompilację
    make clean > /dev/null 2>&1 || true
    
    # Kompilacja
    make -j$(nproc) || make  # Fallback do single-threaded
    
    echo "✅ Kompilacja Makefile zakończona pomyślnie!"
}

# Funkcja uruchomienia aplikacji
run_application() {
    echo ""
    echo "🚀 Uruchamianie aplikacji..."
    
    cd "$SCRIPT_DIR"
    
    if [ -f "build/bin/qr-generator" ]; then
        echo "📱 Uruchamianie Generator Kodów QR..."
        ./build/bin/qr-generator
    elif [ -f "build/bin/qrgenerator" ]; then
        echo "📱 Uruchamianie Generator Kodów QR..."
        ./build/bin/qrgenerator
    else
        echo "❌ Nie znaleziono pliku wykonywalnego!"
        echo "   Sprawdź czy kompilacja przebiegła pomyślnie"
        exit 1
    fi
}

# Funkcja instalacji
install_application() {
    echo ""
    echo "📦 Instalacja aplikacji do systemu..."
    
    cd "$SCRIPT_DIR/build"
    
    if [ -f "bin/qr-generator" ]; then
        sudo cp bin/qr-generator /usr/local/bin/
        echo "✅ Aplikacja zainstalowana do /usr/local/bin/qr-generator"
        echo "   Możesz teraz uruchomić aplikację komendą: qr-generator"
    else
        echo "❌ Nie znaleziono pliku do instalacji!"
        exit 1
    fi
}

# Funkcja pomocy
show_help() {
    echo "Generator Kodów QR - Skrypt kompilacji"
    echo ""
    echo "Użycie: $0 [OPCJE]"
    echo ""
    echo "Opcje:"
    echo "  -h, --help        Pokaż tę pomoc"
    echo "  -c, --check       Sprawdź tylko zależności"
    echo "  -b, --build       Tylko skompiluj (nie uruchamiaj)"
    echo "  -r, --run         Tylko uruchom (jeśli już skompilowane)"
    echo "  -i, --install     Zainstaluj do systemu po kompilacji"
    echo "  -m, --makefile    Użyj Makefile zamiast CMake"
    echo "  --clean           Wyczyść pliki kompilacji"
    echo ""
    echo "Przykłady:"
    echo "  $0                # Skompiluj i uruchom"
    echo "  $0 --check        # Sprawdź zależności"
    echo "  $0 --build        # Tylko kompilacja"
    echo "  $0 --install      # Kompiluj i zainstaluj"
}

# Funkcja czyszczenia
clean_build() {
    echo "🧹 Czyszczenie plików kompilacji..."
    cd "$SCRIPT_DIR"
    
    if [ -d "build" ]; then
        rm -rf build
        echo "✅ Usunięto katalog build/"
    fi
    
    # Wyczyść pliki Makefile
    make clean > /dev/null 2>&1 || true
    
    echo "✅ Czyszczenie zakończone!"
}

# Parsowanie argumentów
BUILD_ONLY=false
RUN_ONLY=false
CHECK_ONLY=false
INSTALL=false
USE_MAKEFILE=false
CLEAN_ONLY=false

while [[ $# -gt 0 ]]; do
    case $1 in
        -h|--help)
            show_help
            exit 0
            ;;
        -c|--check)
            CHECK_ONLY=true
            shift
            ;;
        -b|--build)
            BUILD_ONLY=true
            shift
            ;;
        -r|--run)
            RUN_ONLY=true
            shift
            ;;
        -i|--install)
            INSTALL=true
            shift
            ;;
        -m|--makefile)
            USE_MAKEFILE=true
            shift
            ;;
        --clean)
            CLEAN_ONLY=true
            shift
            ;;
        *)
            echo "❌ Nieznana opcja: $1"
            echo "   Użyj --help aby zobaczyć dostępne opcje"
            exit 1
            ;;
    esac
done

# Główna logika
main() {
    # Czyszczenie
    if [ "$CLEAN_ONLY" = true ]; then
        clean_build
        exit 0
    fi
    
    # Sprawdzenie zależności
    if [ "$CHECK_ONLY" = true ]; then
        check_dependencies
        exit 0
    fi
    
    # Sprawdź zależności (zawsze)
    check_dependencies
    
    # Tylko uruchomienie
    if [ "$RUN_ONLY" = true ]; then
        run_application
        exit 0
    fi
    
    # Kompilacja
    if [ "$USE_MAKEFILE" = true ]; then
        build_makefile
    else
        build_cmake
    fi
    
    # Instalacja
    if [ "$INSTALL" = true ]; then
        install_application
    fi
    
    # Uruchomienie (jeśli nie tylko kompilacja)
    if [ "$BUILD_ONLY" = false ]; then
        run_application
    fi
}

# Uruchom główną funkcję
main "$@"
