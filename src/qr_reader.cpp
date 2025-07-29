#include "qrgenerator.h"

// Implementacja metod odczytu QR

void QRGenerator::readQRFromFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        "Wybierz plik z kodem QR",
        QStandardPaths::writableLocation(QStandardPaths::PicturesLocation),
        "Pliki obrazów (*.png *.jpg *.jpeg *.bmp *.gif *.tiff)");
    
    if (fileName.isEmpty()) {
        return;
    }
    
    try {
        // Wczytanie obrazu przez OpenCV
        cv::Mat image = cv::imread(fileName.toStdString());
        
        if (image.empty()) {
            showError("Nie można wczytać pliku obrazu");
            return;
        }
        
        QString result = decodeQRFromImage(image);
        
        if (!result.isEmpty()) {
            displayQRResult(result, "Plik: " + QFileInfo(fileName).fileName());
        } else {
            displayQRResult("Nie znaleziono kodu QR w obrazie", "Błąd odczytu");
        }
        
    } catch (const std::exception& e) {
        showError(QString("Błąd podczas odczytu pliku: %1").arg(e.what()));
    }
}

void QRGenerator::readQRFromCamera()
{
    try {
        // Sprawdź czy kamera jest już włączona
        if (m_cameraTimer->isActive()) {
            // Zatrzymaj kamerę
            m_cameraTimer->stop();
            m_camera.release();
            m_readCameraButton->setText("Użyj kamery");
            return;
        }
        
        // Włącz kamerę
        m_camera.open(0); // Domyślna kamera
        
        if (!m_camera.isOpened()) {
            showError("Nie można otworzyć kamery");
            return;
        }
        
        m_readCameraButton->setText("Zatrzymaj kamerę");
        
        // Uruchom timer do cyklicznego odczytu z kamery
        m_cameraTimer->start(100); // Co 100ms
        
        // Funkcja wywoływana przez timer
        connect(m_cameraTimer, &QTimer::timeout, [this]() {
            cv::Mat frame;
            m_camera >> frame;
            
            if (!frame.empty()) {
                QString result = decodeQRFromImage(frame);
                if (!result.isEmpty()) {
                    // Znaleziono kod QR - zatrzymaj kamerę
                    m_cameraTimer->stop();
                    m_camera.release();
                    m_readCameraButton->setText("Użyj kamery");
                    
                    displayQRResult(result, "Kamera");
                }
            }
        });
        
    } catch (const std::exception& e) {
        showError(QString("Błąd kamery: %1").arg(e.what()));
    }
}

void QRGenerator::readQRFromScreen()
{
    // Ukryj okno aplikacji na czas robienia zrzutu
    hide();
    
    // Daj użytkownikowi czas na przygotowanie
    QTimer::singleShot(1000, [this]() {
        try {
            // Zrób zrzut całego ekranu
            QScreen* screen = QGuiApplication::primaryScreen();
            QPixmap screenshot = screen->grabWindow(0);
            
            // Pokaż okno z powrotem
            show();
            
            // Konwertuj QPixmap do cv::Mat
            QImage qimage = screenshot.toImage();
            qimage = qimage.convertToFormat(QImage::Format_RGB888);
            
            cv::Mat image(qimage.height(), qimage.width(), CV_8UC3, 
                         (void*)qimage.constBits(), qimage.bytesPerLine());
            cv::cvtColor(image, image, cv::COLOR_RGB2BGR);
            
            QString result = decodeQRFromImage(image);
            
            if (!result.isEmpty()) {
                displayQRResult(result, "Zrzut ekranu");
            } else {
                displayQRResult("Nie znaleziono kodu QR na ekranie", "Błąd odczytu");
            }
            
        } catch (const std::exception& e) {
            show(); // Upewnij się, że okno jest widoczne
            showError(QString("Błąd podczas robienia zrzutu ekranu: %1").arg(e.what()));
        }
    });
}

QString QRGenerator::decodeQRFromImage(const cv::Mat& image)
{
    try {
        // Tu powinna być implementacja dekodowania QR przy użyciu biblioteki ZXing
        // Ponieważ ZXing-C++ może być skomplikowane w konfiguracji, 
        // przedstawię uproszczoną wersję z użyciem OpenCV QRCodeDetector
        
        cv::QRCodeDetector qrDecoder;
        
        // Próba dekodowania
        std::string decodedText = qrDecoder.detectAndDecode(image);
        
        if (!decodedText.empty()) {
            return QString::fromStdString(decodedText);
        }
        
        // Jeśli nie udało się, spróbuj z konwersją do skali szarości
        cv::Mat grayImage;
        if (image.channels() > 1) {
            cv::cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);
        } else {
            grayImage = image;
        }
        
        decodedText = qrDecoder.detectAndDecode(grayImage);
        
        if (!decodedText.empty()) {
            return QString::fromStdString(decodedText);
        }
        
        return QString(); // Nie znaleziono kodu QR
        
    } catch (const std::exception& e) {
        qWarning() << "Błąd dekodowania QR:" << e.what();
        return QString();
    }
}

void QRGenerator::displayQRResult(const QString& result, const QString& type)
{
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString formattedResult;
    
    if (!type.isEmpty()) {
        formattedResult = QString("[%1] %2:\n").arg(timestamp).arg(type);
    } else {
        formattedResult = QString("[%1] Wynik:\n").arg(timestamp);
    }
    
    formattedResult += result + "\n";
    
    // Analiza typu danych
    if (result.startsWith("WIFI:")) {
        formattedResult += "\n--- Dane sieci WiFi ---\n";
        // Parsowanie danych WiFi
        QStringList parts = result.split(';');
        for (const QString& part : parts) {
            if (part.startsWith("S:")) {
                formattedResult += "SSID: " + part.mid(2) + "\n";
            } else if (part.startsWith("T:")) {
                formattedResult += "Zabezpieczenia: " + part.mid(2) + "\n";
            } else if (part.startsWith("P:")) {
                formattedResult += "Hasło: " + part.mid(2) + "\n";
            } else if (part.startsWith("H:")) {
                formattedResult += "Ukryta: " + (part.mid(2) == "true" ? QString("Tak") : QString("Nie")) + "\n";
            }
        }
    } else if (result.startsWith("BEGIN:VCARD")) {
        formattedResult += "\n--- Dane kontaktowe (vCard) ---\n";
        // Można dodać parsowanie vCard
    } else if (result.startsWith("http://") || result.startsWith("https://")) {
        formattedResult += "\n--- Adres URL ---\n";
    }
    
    formattedResult += "\n" + QString(50, '=') + "\n\n";
    
    // Dodaj do pola wyników
    m_resultTextEdit->append(formattedResult);
    
    // Przejdź do zakładki czytnika
    m_tabWidget->setCurrentIndex(4);
}
