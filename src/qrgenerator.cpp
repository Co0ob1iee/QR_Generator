#include "qrgenerator.h"

// Implementacja konstruktora
QRGenerator::QRGenerator(QWidget *parent) : QMainWindow(parent), m_tabWidget(nullptr), m_qrLabel(nullptr), m_passwordVisible(false), m_cameraTimer(new QTimer(this))
{
    // Ustawienie podstawowych właściwości okna
    setWindowTitle("Generator Kodów QR - C++ Qt");
    setMinimumSize(1000, 800);
    resize(1200, 900);
    
    // Inicjalizacja katalogu konfiguracyjnego
    m_configDir = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/QRGenerator";
    QDir().mkpath(m_configDir);
    m_wifiFile = m_configDir + "/wifi_networks.json";
    
    // Inicjalizacja szyfrowania i wczytanie danych
    initializeEncryption();
    loadWiFiNetworks();
    
    // Ustawienie interfejsu użytkownika
    setupUI();
    
    // Połączenie timera kamery
    connect(m_cameraTimer, &QTimer::timeout, this, &QRGenerator::readQRFromCamera);
}

QRGenerator::~QRGenerator()
{
    // Zwolnienie zasobów kamery
    if (m_camera.isOpened()) {
        m_camera.release();
    }
    
    // Zapisanie konfiguracji WiFi
    saveWiFiNetworks();
}

void QRGenerator::setupUI()
{
    // Główny widget centralny
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    // Główny layout poziomy z splitterem
    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);
    QSplitter* mainSplitter = new QSplitter(Qt::Horizontal, this);
    mainLayout->addWidget(mainSplitter);
    
    // Lewa strona - zakładki z formularzami
    m_tabWidget = new QTabWidget();
    m_tabWidget->setMinimumWidth(500);
    mainSplitter->addWidget(m_tabWidget);
    
    // Prawa strona - panel z kodem QR
    QWidget* rightPanel = new QWidget();
    rightPanel->setMinimumWidth(400);
    rightPanel->setMaximumWidth(500);
    mainSplitter->addWidget(rightPanel);
    
    // Proporcje splittera (60% lewy panel, 40% prawy)
    mainSplitter->setSizes({600, 400});
    
    // Ustawienie zakładek
    setupUrlTab();
    setupTextTab();
    setupContactTab();
    setupWiFiTab();
    setupReaderTab();
    
    // Ustawienie panelu QR
    setupQRPanel(rightPanel);
}

// Dołącz plik MOC (wymagane dla Qt)
#include "qrgenerator.moc"
