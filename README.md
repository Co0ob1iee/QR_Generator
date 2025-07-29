# Generator Kodów QR - C++ Qt

Zaawansowana aplikacja do generowania i odczytywania kodów QR napisana w C++ z użyciem biblioteki Qt.

## Funkcjonalności

### Generowanie kodów QR:
- **URL** - generowanie kodów QR dla adresów internetowych
- **Tekst** - kodowanie dowolnego tekstu w kodzie QR
- **Kontakt** - tworzenie vCard z danymi kontaktowymi
- **WiFi** - generowanie kodów QR dla sieci WiFi z zarządzaniem zapisanymi sieciami

### Odczytywanie kodów QR:
- Wczytywanie z plików obrazów
- Skanowanie przy użyciu kamery
- Odczyt z zrzutu ekranu
- Analiza i formatowanie wyników

### Dodatkowe funkcje:
- Zapisywanie kodów QR jako obrazy PNG
- Kopiowanie danych do schowka
- Szyfrowane przechowywanie haseł WiFi
- Intuicyjny interfejs użytkownika

## Wymagania systemowe

### Biblioteki wymagane:
- **Qt6** (Core, Widgets, Gui)
- **libqrencode** - do generowania kodów QR
- **OpenCV** - do odczytywania kodów QR z obrazów i kamery
- **cmake** - system budowania
- **g++** lub inny kompilator C++17

## Instalacja

### Ubuntu/Debian:

```bash
# Instalacja zależności
sudo apt update
sudo apt install qt6-base-dev qt6-tools-dev libqrencode-dev
sudo apt install libopencv-dev cmake build-essential
sudo apt install pkg-config

# Klonowanie i kompilacja
git clone <repository-url>
cd qr-generator-project
mkdir build && cd build
cmake ..
make

# Uruchomienie
./bin/qr-generator
```

### Arch Linux/CachyOS:

```bash
# Instalacja zależności
sudo pacman -S qt6-base qt6-tools qrencode opencv cmake
sudo pacman -S pkgconf gcc

# Kompilacja
mkdir build && cd build
cmake ..
make

# Uruchomienie
./bin/qr-generator
```

### Fedora/CentOS:

```bash
# Instalacja zależności
sudo dnf install qt6-qtbase-devel qrencode-devel opencv-devel
sudo dnf install cmake gcc-c++ pkgconfig

# Kompilacja
mkdir build && cd build
cmake ..
make

# Uruchomienie
./bin/qr-generator
```

## Kompilacja z źródeł

1. **Pobierz wymagane zależności** (patrz sekcja instalacji wyżej)

2. **Sklonuj repozytorium:**
   ```bash
   git clone <repository-url>
   cd qr-generator-project
   ```

3. **Skompiluj projekt:**
   ```bash
   mkdir build
   cd build
   cmake ..
   make -j$(nproc)
   ```

4. **Uruchom aplikację:**
   ```bash
   ./bin/qr-generator
   ```

## Użytkowanie

### Generowanie kodów QR:

1. **URL:**
   - Przejdź do zakładki "URL"
   - Wprowadź adres strony internetowej
   - Kod QR zostanie automatycznie wygenerowany

2. **Tekst:**
   - Wybierz zakładkę "Tekst"
   - Wpisz dowolny tekst w pole tekstowe
   - Kod QR pojawi się automatycznie

3. **Kontakt:**
   - Przejdź do "Kontakt"
   - Wypełnij pola z danymi kontaktowymi
   - Aplikacja wygeneruje vCard w kodzie QR

4. **WiFi:**
   - Wybierz zakładkę "WiFi"
   - Wprowadź dane sieci (SSID, hasło, typ zabezpieczeń)
   - Opcjonalnie zapisz sieć dla przyszłego użytku
   - Wygeneruj kod QR do łatwego udostępniania WiFi

### Odczytywanie kodów QR:

1. **Z pliku:**
   - Przejdź do zakładki "Czytnik QR"
   - Kliknij "Wczytaj z pliku"
   - Wybierz obraz zawierający kod QR

2. **Z kamery:**
   - Kliknij "Użyj kamery"
   - Skieruj kamerę na kod QR
   - Aplikacja automatycznie odczyta kod

3. **Z ekranu:**
   - Kliknij "Zrzut ekranu"
   - Aplikacja zrobi zrzut ekranu i znajdzie kod QR

### Zarządzanie danymi:

- **Zapisywanie obrazów:** Kliknij "Zapisz PNG" aby zapisać kod QR jako obraz
- **Kopiowanie:** Użyj "Kopiuj" aby skopiować dane do schowka
- **Zarządzanie WiFi:** Zapisuj, wczytuj i usuwaj sieci WiFi w zakładce WiFi

## Struktura projektu

```
qr-generator-project/
├── CMakeLists.txt          # Konfiguracja CMake
├── README.md               # Ten plik
├── include/
│   └── qrgenerator.h       # Definicje klasy
├── src/
│   ├── main.cpp            # Punkt wejścia aplikacji
│   ├── qrgenerator.cpp     # Konstruktor i destruktor
│   ├── ui_setup.cpp        # Ustawianie interfejsu użytkownika
│   ├── qr_generation.cpp   # Generowanie kodów QR
│   ├── wifi_handler.cpp    # Obsługa sieci WiFi
│   ├── qr_reader.cpp       # Odczytywanie kodów QR
│   └── utils.cpp           # Funkcje pomocnicze
└── build/                  # Katalog kompilacji (tworzony automatycznie)
```

## Bezpieczeństwo

- Hasła WiFi są szyfrowane przed zapisaniem do pliku
- Używane jest szyfrowanie XOR z kluczem bazowanym na systemie
- Pliki konfiguracyjne są zapisywane w standardowej lokalizacji użytkownika

## Rozwiązywanie problemów

### Błędy kompilacji:

1. **Brak Qt6:**
   ```bash
   # Ubuntu/Debian
   sudo apt install qt6-base-dev qt6-tools-dev
   
   # Arch Linux
   sudo pacman -S qt6-base qt6-tools
   ```

2. **Brak OpenCV:**
   ```bash
   # Ubuntu/Debian
   sudo apt install libopencv-dev
   
   # Arch Linux
   sudo pacman -S opencv
   ```

3. **Brak qrencode:**
   ```bash
   # Ubuntu/Debian
   sudo apt install libqrencode-dev
   
   # Arch Linux
   sudo pacman -S qrencode
   ```

### Problemy z kamerą:

- Sprawdź czy kamera nie jest używana przez inną aplikację
- Na niektórych systemach może być potrzebne dodanie użytkownika do grupy `video`
- Sprawdź uprawnienia dostępu do urządzenia `/dev/video0`

## Licencja

Ten projekt jest udostępniony na licencji MIT. Zobacz plik LICENSE dla szczegółów.

## Autorzy

Aplikacja została stworzona na podstawie specyfikacji generatora kodów QR w C++ Qt.

## Zgłaszanie błędów

W przypadku znalezienia błędów lub problemów, prosimy o utworzenie issue w repozytorium projektu.

## Przyszłe rozwój

Planowane funkcjonalności:
- Więcej formatów eksportu (SVG, PDF)
- Zaawansowane opcje stylizacji kodów QR
- Wsparcie dla kodów Data Matrix i innych formatów
- Tryb wsadowego przetwarzania
- Integracja z usługami chmurowymi
