/*
 * QR Code Generator - Aplikacja Qt/C++ dla Linux
 * 
 * Wymagane biblioteki:
 * - Qt5 lub Qt6 (Core, Widgets, GUI)
 * - libqrencode (dla generowania kodów QR)
 * - OpenCV (dla odczytu kodów QR z kamery/obrazów)
 * 
 * Instalacja zależności (Ubuntu/Debian):
 * sudo apt install qt6-base-dev qt6-tools-dev libqrencode-dev
 * sudo apt install libopencv-dev
 * 
 * Kompilacja:
 * mkdir build && cd build
 * cmake .. && make
 */

#ifndef QRGENERATOR_H
#define QRGENERATOR_H

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QInputDialog>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QSplitter>
#include <QtGui/QPixmap>
#include <QtGui/QClipboard>
#include <QtGui/QScreen>
#include <QtGui/QGuiApplication>
#include <QtCore/QSettings>
#include <QtCore/QStandardPaths>
#include <QtCore/QDir>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QDateTime>
#include <QtCore/QCryptographicHash>
#include <QtCore/QTimer>
#include <QtCore/QFileInfo>

// Zewnętrzne biblioteki
#include <qrencode.h>
#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>

#include <memory>
#include <map>
#include <string>

// Struktura przechowująca dane sieci WiFi
struct WiFiNetwork {
    QString ssid;
    QString encryptedPassword;  // Zaszyfrowane hasło
    QString security;           // WPA, WEP, nopass
    bool hidden;
    QDateTime savedDate;
    
    // Serializacja do JSON
    QJsonObject toJson() const {
        QJsonObject obj;
        obj["ssid"] = ssid;
        obj["password"] = encryptedPassword;
        obj["security"] = security;
        obj["hidden"] = hidden;
        obj["saved_date"] = savedDate.toString(Qt::ISODate);
        return obj;
    }
    
    // Deserializacja z JSON
    static WiFiNetwork fromJson(const QJsonObject& obj) {
        WiFiNetwork network;
        network.ssid = obj["ssid"].toString();
        network.encryptedPassword = obj["password"].toString();
        network.security = obj["security"].toString();
        network.hidden = obj["hidden"].toBool();
        network.savedDate = QDateTime::fromString(obj["saved_date"].toString(), Qt::ISODate);
        return network;
    }
};

class QRGenerator : public QMainWindow
{
    Q_OBJECT

public:
    explicit QRGenerator(QWidget *parent = nullptr);
    ~QRGenerator();

private slots:
    // Sloty do generowania kodów QR
    void generateUrlQR();
    void generateTextQR();
    void generateContactQR();
    void generateWiFiQR();
    
    // Sloty dla funkcji WiFi
    void saveWiFiNetwork();
    void loadSelectedWiFi();
    void deleteSelectedWiFi();
    void refreshWiFiList();
    void onWiFiSelectionChanged();
    void togglePasswordVisibility();
    
    // Sloty dla odczytu QR
    void readQRFromFile();
    void readQRFromCamera();
    void readQRFromScreen();
    
    // Sloty dla akcji z kodem QR
    void saveQRImage();
    void copyToClipboard();
    void clearQR();
    void copyResult();

private:
    // Metody inicjalizacji interfejsu
    void setupUI();
    void setupUrlTab();
    void setupTextTab();
    void setupContactTab();
    void setupWiFiTab();
    void setupReaderTab();
    void setupQRPanel(QWidget* parent);
    
    // Metody generowania kodów QR
    void generateQRCode(const QString& data);
    QPixmap createQRImage(const QString& data, int size = 300);
    
    // Metody obsługi WiFi
    void initializeEncryption();
    QString encryptPassword(const QString& password);
    QString decryptPassword(const QString& encryptedPassword);
    void loadWiFiNetworks();
    void saveWiFiNetworks();
    
    // Metody odczytu QR
    QString decodeQRFromImage(const cv::Mat& image);
    void displayQRResult(const QString& result, const QString& type = "");
    
    // Pomocnicze metody
    QString generateVCard() const;
    QString generateWiFiString() const;
    void showError(const QString& message);
    void showInfo(const QString& message);

private:
    // Główne komponenty UI
    QTabWidget* m_tabWidget;
    QLabel* m_qrLabel;
    QPixmap m_currentQR;
    
    // Zakładka URL
    QLineEdit* m_urlEdit;
    
    // Zakładka Tekst
    QTextEdit* m_textEdit;
    
    // Zakładka Kontakt
    QLineEdit* m_firstNameEdit;
    QLineEdit* m_lastNameEdit;
    QLineEdit* m_phoneEdit;
    QLineEdit* m_emailEdit;
    QLineEdit* m_companyEdit;
    QLineEdit* m_websiteEdit;
    
    // Zakładka WiFi
    QLineEdit* m_wifiSSIDEdit;
    QLineEdit* m_wifiPasswordEdit;
    QComboBox* m_wifiSecurityCombo;
    QCheckBox* m_wifiHiddenCheck;
    QListWidget* m_wifiListWidget;
    QPushButton* m_showPasswordButton;
    bool m_passwordVisible;
    
    // Zakładka Czytnik
    QTextEdit* m_resultTextEdit;
    QPushButton* m_readFileButton;
    QPushButton* m_readCameraButton;
    QPushButton* m_readScreenButton;
    
    // Przyciski akcji
    QPushButton* m_saveImageButton;
    QPushButton* m_copyButton;
    QPushButton* m_clearButton;
    
    // Dane aplikacji
    std::map<QString, WiFiNetwork> m_wifiNetworks;
    QString m_configDir;
    QString m_wifiFile;
    QString m_encryptionKey;
    
    // OpenCV dla kamery
    cv::VideoCapture m_camera;
    QTimer* m_cameraTimer;
};

#endif // QRGENERATOR_H
