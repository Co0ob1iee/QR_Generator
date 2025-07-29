#include "qrgenerator.h"

void QRGenerator::setupUrlTab()
{
    QWidget* urlWidget = new QWidget();
    QFormLayout* layout = new QFormLayout(urlWidget);
    
    // Pole URL z etykietą opisową
    QLabel* titleLabel = new QLabel("Generowanie kodu QR dla adresu internetowego");
    titleLabel->setStyleSheet("font-weight: bold; font-size: 14px; margin-bottom: 10px;");
    layout->addRow(titleLabel);
    
    m_urlEdit = new QLineEdit();
    m_urlEdit->setPlaceholderText("https://example.com");
    layout->addRow("Adres URL:", m_urlEdit);
    
    QLabel* helpLabel = new QLabel("Wprowadź pełny adres strony internetowej. Protokół https:// zostanie dodany automatycznie jeśli go nie podasz.");
    helpLabel->setWordWrap(true);
    helpLabel->setStyleSheet("color: gray; font-size: 11px;");
    layout->addRow(helpLabel);
    
    // Połączenie sygnału zmiany tekstu z generowaniem QR
    connect(m_urlEdit, &QLineEdit::textChanged, this, &QRGenerator::generateUrlQR);
    
    m_tabWidget->addTab(urlWidget, "URL");
}

void QRGenerator::setupTextTab()
{
    QWidget* textWidget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(textWidget);
    
    QLabel* titleLabel = new QLabel("Generowanie kodu QR dla dowolnego tekstu");
    titleLabel->setStyleSheet("font-weight: bold; font-size: 14px; margin-bottom: 10px;");
    layout->addWidget(titleLabel);
    
    QLabel* textLabel = new QLabel("Treść tekstowa:");
    layout->addWidget(textLabel);
    
    m_textEdit = new QTextEdit();
    m_textEdit->setPlaceholderText("Wprowadź dowolny tekst, który ma być zakodowany w kodzie QR...");
    m_textEdit->setMaximumHeight(200);
    layout->addWidget(m_textEdit);
    
    QLabel* helpLabel = new QLabel("Możesz wprowadzić wieloliniowy tekst, numery telefonów, adresy email lub inne informacje.");
    helpLabel->setWordWrap(true);
    helpLabel->setStyleSheet("color: gray; font-size: 11px;");
    layout->addWidget(helpLabel);
    
    layout->addStretch(); // Rozciągnięcie na dole
    
    connect(m_textEdit, &QTextEdit::textChanged, this, &QRGenerator::generateTextQR);
    
    m_tabWidget->addTab(textWidget, "Tekst");
}

void QRGenerator::setupContactTab()
{
    QWidget* contactWidget = new QWidget();
    QFormLayout* layout = new QFormLayout(contactWidget);
    
    QLabel* titleLabel = new QLabel("Generowanie kodu QR dla danych kontaktowych (vCard)");
    titleLabel->setStyleSheet("font-weight: bold; font-size: 14px; margin-bottom: 10px;");
    layout->addRow(titleLabel);
    
    // Pola kontaktowe
    m_firstNameEdit = new QLineEdit();
    m_firstNameEdit->setPlaceholderText("Jan");
    layout->addRow("Imię:", m_firstNameEdit);
    
    m_lastNameEdit = new QLineEdit();
    m_lastNameEdit->setPlaceholderText("Kowalski");
    layout->addRow("Nazwisko:", m_lastNameEdit);
    
    m_phoneEdit = new QLineEdit();
    m_phoneEdit->setPlaceholderText("+48 123 456 789");
    layout->addRow("Telefon:", m_phoneEdit);
    
    m_emailEdit = new QLineEdit();
    m_emailEdit->setPlaceholderText("jan.kowalski@example.com");
    layout->addRow("Email:", m_emailEdit);
    
    m_companyEdit = new QLineEdit();
    m_companyEdit->setPlaceholderText("Nazwa firmy");
    layout->addRow("Firma:", m_companyEdit);
    
    m_websiteEdit = new QLineEdit();
    m_websiteEdit->setPlaceholderText("https://example.com");
    layout->addRow("Strona WWW:", m_websiteEdit);
    
    QLabel* helpLabel = new QLabel("Wygenerowany kod QR będzie zawierał dane kontaktowe w standardzie vCard, które można zaimportować do książki adresowej.");
    helpLabel->setWordWrap(true);
    helpLabel->setStyleSheet("color: gray; font-size: 11px; margin-top: 10px;");
    layout->addRow(helpLabel);
    
    // Połączenie wszystkich pól z funkcją generowania
    connect(m_firstNameEdit, &QLineEdit::textChanged, this, &QRGenerator::generateContactQR);
    connect(m_lastNameEdit, &QLineEdit::textChanged, this, &QRGenerator::generateContactQR);
    connect(m_phoneEdit, &QLineEdit::textChanged, this, &QRGenerator::generateContactQR);
    connect(m_emailEdit, &QLineEdit::textChanged, this, &QRGenerator::generateContactQR);
    connect(m_companyEdit, &QLineEdit::textChanged, this, &QRGenerator::generateContactQR);
    connect(m_websiteEdit, &QLineEdit::textChanged, this, &QRGenerator::generateContactQR);
    
    m_tabWidget->addTab(contactWidget, "Kontakt");
}

void QRGenerator::setupWiFiTab()
{
    QWidget* wifiWidget = new QWidget();
    QVBoxLayout* mainLayout = new QVBoxLayout(wifiWidget);
    
    // Sekcja nowej sieci WiFi
    QGroupBox* newWiFiGroup = new QGroupBox("Nowa sieć WiFi");
    QFormLayout* newWiFiLayout = new QFormLayout(newWiFiGroup);
    
    m_wifiSSIDEdit = new QLineEdit();
    m_wifiSSIDEdit->setPlaceholderText("Nazwa_Sieci_WiFi");
    newWiFiLayout->addRow("Nazwa sieci (SSID):", m_wifiSSIDEdit);
    
    // Layout poziomy dla hasła i przycisku pokazywania
    QHBoxLayout* passwordLayout = new QHBoxLayout();
    m_wifiPasswordEdit = new QLineEdit();
    m_wifiPasswordEdit->setEchoMode(QLineEdit::Password);
    m_wifiPasswordEdit->setPlaceholderText("Hasło do sieci");
    passwordLayout->addWidget(m_wifiPasswordEdit);
    
    m_showPasswordButton = new QPushButton("👁");
    m_showPasswordButton->setMaximumWidth(30);
    m_showPasswordButton->setToolTip("Pokaż/ukryj hasło");
    passwordLayout->addWidget(m_showPasswordButton);
    
    QWidget* passwordWidget = new QWidget();
    passwordWidget->setLayout(passwordLayout);
    newWiFiLayout->addRow("Hasło:", passwordWidget);
    
    m_wifiSecurityCombo = new QComboBox();
    m_wifiSecurityCombo->addItems({"WPA", "WEP", "nopass"});
    newWiFiLayout->addRow("Zabezpieczenia:", m_wifiSecurityCombo);
    
    m_wifiHiddenCheck = new QCheckBox("Sieć ukryta");
    newWiFiLayout->addRow(m_wifiHiddenCheck);
    
    // Przyciski dla nowej sieci
    QHBoxLayout* newWiFiButtonsLayout = new QHBoxLayout();
    QPushButton* saveWiFiButton = new QPushButton("Zapisz sieć");
    QPushButton* generateWiFiButton = new QPushButton("Generuj QR");
    newWiFiButtonsLayout->addWidget(saveWiFiButton);
    newWiFiButtonsLayout->addWidget(generateWiFiButton);
    newWiFiButtonsLayout->addStretch();
    
    newWiFiLayout->addRow(newWiFiButtonsLayout);
    
    mainLayout->addWidget(newWiFiGroup);
    
    // Sekcja zapisanych sieci
    QGroupBox* savedWiFiGroup = new QGroupBox("Zapisane sieci WiFi");
    QVBoxLayout* savedWiFiLayout = new QVBoxLayout(savedWiFiGroup);
    
    m_wifiListWidget = new QListWidget();
    m_wifiListWidget->setMaximumHeight(150);
    savedWiFiLayout->addWidget(m_wifiListWidget);
    
    QHBoxLayout* savedWiFiButtonsLayout = new QHBoxLayout();
    QPushButton* loadWiFiButton = new QPushButton("Wczytaj");
    QPushButton* deleteWiFiButton = new QPushButton("Usuń");
    QPushButton* refreshWiFiButton = new QPushButton("Odśwież");
    
    savedWiFiButtonsLayout->addWidget(loadWiFiButton);
    savedWiFiButtonsLayout->addWidget(deleteWiFiButton);
    savedWiFiButtonsLayout->addWidget(refreshWiFiButton);
    savedWiFiButtonsLayout->addStretch();
    
    savedWiFiLayout->addLayout(savedWiFiButtonsLayout);
    mainLayout->addWidget(savedWiFiGroup);
    
    mainLayout->addStretch();
    
    // Połączenia sygnałów
    connect(m_wifiSSIDEdit, &QLineEdit::textChanged, this, &QRGenerator::generateWiFiQR);
    connect(m_wifiPasswordEdit, &QLineEdit::textChanged, this, &QRGenerator::generateWiFiQR);
    connect(m_wifiSecurityCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &QRGenerator::generateWiFiQR);
    connect(m_wifiHiddenCheck, &QCheckBox::toggled, this, &QRGenerator::generateWiFiQR);
    
    connect(saveWiFiButton, &QPushButton::clicked, this, &QRGenerator::saveWiFiNetwork);
    connect(m_showPasswordButton, &QPushButton::clicked, this, &QRGenerator::togglePasswordVisibility);
    connect(loadWiFiButton, &QPushButton::clicked, this, &QRGenerator::loadSelectedWiFi);
    connect(deleteWiFiButton, &QPushButton::clicked, this, &QRGenerator::deleteSelectedWiFi);
    connect(refreshWiFiButton, &QPushButton::clicked, this, &QRGenerator::refreshWiFiList);
    
    connect(m_wifiListWidget, &QListWidget::itemSelectionChanged, this, &QRGenerator::onWiFiSelectionChanged);
    
    // Wczytaj zapisane sieci
    refreshWiFiList();
    
    m_tabWidget->addTab(wifiWidget, "WiFi");
}

void QRGenerator::setupReaderTab()
{
    QWidget* readerWidget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(readerWidget);
    
    QLabel* titleLabel = new QLabel("Czytnik kodów QR");
    titleLabel->setStyleSheet("font-weight: bold; font-size: 16px; margin-bottom: 15px;");
    layout->addWidget(titleLabel);
    
    // Przyciski do wyboru źródła
    QGroupBox* sourceGroup = new QGroupBox("Wybierz źródło kodu QR");
    QHBoxLayout* sourceLayout = new QHBoxLayout(sourceGroup);
    
    m_readFileButton = new QPushButton("Wczytaj z pliku");
    m_readFileButton->setToolTip("Wybierz plik obrazu zawierający kod QR");
    m_readCameraButton = new QPushButton("Użyj kamery");
    m_readCameraButton->setToolTip("Włącz kamerę do skanowania kodów QR");
    m_readScreenButton = new QPushButton("Zrzut ekranu");
    m_readScreenButton->setToolTip("Zrób zrzut ekranu i znajdź kod QR");
    
    sourceLayout->addWidget(m_readFileButton);
    sourceLayout->addWidget(m_readCameraButton);
    sourceLayout->addWidget(m_readScreenButton);
    
    layout->addWidget(sourceGroup);
    
    // Obszar wyników
    QGroupBox* resultGroup = new QGroupBox("Wynik odczytu");
    QVBoxLayout* resultLayout = new QVBoxLayout(resultGroup);
    
    m_resultTextEdit = new QTextEdit();
    m_resultTextEdit->setPlaceholderText("Wyniki odczytu kodów QR pojawią się tutaj...");
    m_resultTextEdit->setReadOnly(true);
    resultLayout->addWidget(m_resultTextEdit);
    
    QPushButton* copyResultButton = new QPushButton("Kopiuj wynik");
    resultLayout->addWidget(copyResultButton);
    
    layout->addWidget(resultGroup);
    
    // Połączenia sygnałów
    connect(m_readFileButton, &QPushButton::clicked, this, &QRGenerator::readQRFromFile);
    connect(m_readCameraButton, &QPushButton::clicked, this, &QRGenerator::readQRFromCamera);
    connect(m_readScreenButton, &QPushButton::clicked, this, &QRGenerator::readQRFromScreen);
    connect(copyResultButton, &QPushButton::clicked, this, &QRGenerator::copyResult);
    
    m_tabWidget->addTab(readerWidget, "Czytnik QR");
}

void QRGenerator::setupQRPanel(QWidget* parent)
{
    QVBoxLayout* layout = new QVBoxLayout(parent);
    
    // Tytuł panelu
    QLabel* titleLabel = new QLabel("Podgląd kodu QR");
    titleLabel->setStyleSheet("font-weight: bold; font-size: 14px; margin-bottom: 10px;");
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);
    
    // Label do wyświetlania kodu QR
    m_qrLabel = new QLabel();
    m_qrLabel->setMinimumSize(300, 300);
    m_qrLabel->setMaximumSize(400, 400);
    m_qrLabel->setAlignment(Qt::AlignCenter);
    m_qrLabel->setStyleSheet("border: 2px dashed #ccc; background-color: white;");
    m_qrLabel->setText("Kod QR pojawi się tutaj");
    layout->addWidget(m_qrLabel);
    
    // Przyciski akcji
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    
    m_saveImageButton = new QPushButton("Zapisz PNG");
    m_copyButton = new QPushButton("Kopiuj");
    m_clearButton = new QPushButton("Wyczyść");
    
    buttonLayout->addWidget(m_saveImageButton);
    buttonLayout->addWidget(m_copyButton);
    buttonLayout->addWidget(m_clearButton);
    
    layout->addLayout(buttonLayout);
    layout->addStretch();
    
    // Połączenia sygnałów
    connect(m_saveImageButton, &QPushButton::clicked, this, &QRGenerator::saveQRImage);
    connect(m_copyButton, &QPushButton::clicked, this, &QRGenerator::copyToClipboard);
    connect(m_clearButton, &QPushButton::clicked, this, &QRGenerator::clearQR);
}
