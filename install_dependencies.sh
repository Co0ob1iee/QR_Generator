#!/bin/bash

# Skrypt automatycznej instalacji zależności dla Generator Kodów QR
# Wspiera Ubuntu/Debian, Arch Linux, Fedora/CentOS

set -e  # Zatrzymaj skrypt w przypadku błędu

echo "=================================="
echo "Generator Kodów QR - Instalator zależności"
echo "=================================="

# Funkcja wykrywania dystrybucji
detect_distro() {
    if [ -f /etc/os-release ]; then
        . /etc/os-release
        DISTRO=$ID
        VERSION=$VERSION_ID
    elif type lsb_release >/dev/null 2>&1; then
        DISTRO=$(lsb_release -si | tr '[:upper:]' '[:lower:]')
        VERSION=$(lsb_release -sr)
    elif [ -f /etc/redhat-release ]; then
        DISTRO="centos"
    else
        echo "❌ Nie można wykryć dystrybucji Linuxa"
        exit 1
    fi
    
    echo "🔍 Wykryto dystrybucję: $DISTRO $VERSION"
}

# Instalacja dla Ubuntu/Debian
install_debian() {
    echo "📦 Instalacja zależności dla Ubuntu/Debian..."
    
    sudo apt update
    
    # Podstawowe narzędzia budowania
    sudo apt install -y build-essential cmake pkg-config git
    
    # Qt6
    echo "🔧 Instalacja Qt6..."
    sudo apt install -y qt6-base-dev qt6-tools-dev
    
    # libqrencode
    echo "📱 Instalacja libqrencode..."
    sudo apt install -y libqrencode-dev
    
    # OpenCV
    echo "👁 Instalacja OpenCV..."
    sudo apt install -y libopencv-dev
    
    echo "✅ Instalacja zależności zakończona pomyślnie!"
}

# Instalacja dla Arch Linux/Manjaro
install_arch() {
    echo "📦 Instalacja zależności dla Arch Linux..."
    
    # Aktualizacja systemu
    sudo pacman -Syu --noconfirm
    
    # Podstawowe narzędzia
    sudo pacman -S --noconfirm base-devel cmake pkgconf git
    
    # Qt6
    echo "🔧 Instalacja Qt6..."
    sudo pacman -S --noconfirm qt6-base qt6-tools
    
    # libqrencode
    echo "📱 Instalacja qrencode..."
    sudo pacman -S --noconfirm qrencode
    
    # OpenCV
    echo "👁 Instalacja OpenCV..."
    sudo pacman -S --noconfirm opencv
    
    echo "✅ Instalacja zależności zakończona pomyślnie!"
}

# Instalacja dla Fedora/CentOS
install_fedora() {
    echo "📦 Instalacja zależności dla Fedora/CentOS..."
    
    # Wybór menedżera pakietów
    if command -v dnf &> /dev/null; then
        PKG_MGR="dnf"
    elif command -v yum &> /dev/null; then
        PKG_MGR="yum"
    else
        echo "❌ Nie znaleziono dnf ani yum"
        exit 1
    fi
    
    # Podstawowe narzędzia
    sudo $PKG_MGR install -y gcc-c++ cmake pkgconfig git
    sudo $PKG_MGR groupinstall -y "Development Tools"
    
    # Qt6
    echo "🔧 Instalacja Qt6..."
    sudo $PKG_MGR install -y qt6-qtbase-devel
    
    # libqrencode
    echo "📱 Instalacja qrencode..."
    sudo $PKG_MGR install -y qrencode-devel
    
    # OpenCV
    echo "👁 Instalacja OpenCV..."
    sudo $PKG_MGR install -y opencv-devel
    
    echo "✅ Instalacja zależności zakończona pomyślnie!"
}

# Instalacja dla openSUSE
install_opensuse() {
    echo "📦 Instalacja zależności dla openSUSE..."
    
    sudo zypper refresh
    
    # Podstawowe narzędzia
    sudo zypper install -y gcc-c++ cmake pkg-config git
    sudo zypper install -y -t pattern devel_basis
    
    # Qt6
    echo "🔧 Instalacja Qt6..."
    sudo zypper install -y qt6-base-devel
    
    # libqrencode
    echo "📱 Instalacja qrencode..."
    sudo zypper install -y libqrencode-devel
    
    # OpenCV
    echo "👁 Instalacja OpenCV..."
    sudo zypper install -y opencv-devel
    
    echo "✅ Instalacja zależności zakończona pomyślnie!"
}

# Weryfikacja instalacji
verify_installation() {
    echo ""
    echo "🔍 Weryfikacja instalacji..."
    
    # Sprawdź cmake
    if command -v cmake &> /dev/null; then
        echo "✅ CMake: $(cmake --version | head -n1)"
    else
        echo "❌ CMake nie został zainstalowany"
    fi
    
    # Sprawdź gcc
    if command -v g++ &> /dev/null; then
        echo "✅ G++: $(g++ --version | head -n1)"
    else
        echo "❌ G++ nie został zainstalowany"
    fi
    
    # Sprawdź pkg-config
    if command -v pkg-config &> /dev/null; then
        echo "✅ pkg-config: $(pkg-config --version)"
    else
        echo "❌ pkg-config nie został zainstalowany"
    fi
    
    # Sprawdź qrencode
    if pkg-config --exists libqrencode; then
        echo "✅ libqrencode: $(pkg-config --modversion libqrencode)"
    else
        echo "❌ libqrencode nie został zainstalowany"
    fi
    
    # Sprawdź OpenCV
    if pkg-config --exists opencv4; then
        echo "✅ OpenCV: $(pkg-config --modversion opencv4)"
    elif pkg-config --exists opencv; then
        echo "✅ OpenCV: $(pkg-config --modversion opencv)"
    else
        echo "❌ OpenCV nie został zainstalowany"
    fi
}

# Główna funkcja
main() {
    detect_distro
    
    case $DISTRO in
        ubuntu|debian|linuxmint|pop)
            install_debian
            ;;
        arch|manjaro|cachyos|endeavouros)
            install_arch
            ;;
        fedora|centos|rhel)
            install_fedora
            ;;
        opensuse*|sled|sles)
            install_opensuse
            ;;
        *)
            echo "❌ Nieobsługiwana dystrybucja: $DISTRO"
            echo "📖 Sprawdź README.md dla instrukcji instalacji ręcznej"
            exit 1
            ;;
    esac
    
    verify_installation
    
    echo ""
    echo "🎉 Instalacja zakończona!"
    echo "📁 Możesz teraz skompilować projekt:"
    echo "   mkdir build && cd build"
    echo "   cmake .."
    echo "   make"
    echo "   ./bin/qr-generator"
}

# Sprawdź czy skrypt jest uruchamiany jako root
if [[ $EUID -eq 0 ]]; then
   echo "❌ Nie uruchamiaj tego skryptu jako root!"
   echo "   Skrypt automatycznie użyje sudo gdy będzie potrzebne"
   exit 1
fi

# Uruchom główną funkcję
main "$@"
