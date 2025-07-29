#include "qrgenerator.h"

// Implementacja metod generowania kodów QR

void QRGenerator::generateUrlQR()
{
    QString url = m_urlEdit->text().trimmed();
    if (!url.isEmpty()) {
        // Dodaj protokół jeśli brakuje
        if (!url.startsWith("http://") && !url.startsWith("https://")) {
            url = "https://" + url;
        }
        generateQRCode(url);
    } else {
        clearQR();
    }
}

void QRGenerator::generateTextQR()
{
    QString text = m_textEdit->toPlainText().trimmed();
    if (!text.isEmpty()) {
        generateQRCode(text);
    } else {
        clearQR();
    }
}

void QRGenerator::generateContactQR()
{
    QString vcard = generateVCard();
    if (!vcard.isEmpty()) {
        generateQRCode(vcard);
    } else {
        clearQR();
    }
}

void QRGenerator::generateWiFiQR()
{
    QString wifiString = generateWiFiString();
    if (!wifiString.isEmpty()) {
        generateQRCode(wifiString);
    } else {
        clearQR();
    }
}

void QRGenerator::generateQRCode(const QString& data)
{
    try {
        // Utworzenie kodu QR przy użyciu libqrencode
        QRcode* qrCode = QRcode_encodeString(data.toUtf8().constData(), 
                                           0, // wersja (0 = auto)
                                           QR_ECLEVEL_M, // poziom korekcji błędów
                                           QR_MODE_8, // tryb kodowania
                                           1); // case sensitive
        
        if (!qrCode) {
            showError("Nie można wygenerować kodu QR");
            return;
        }
        
        // Konwersja do QPixmap
        int qrSize = qrCode->width;
        int scale = 8; // skalowanie pikseli
        int border = 4; // ramka
        int imageSize = (qrSize + 2 * border) * scale;
        
        QImage qrImage(imageSize, imageSize, QImage::Format_RGB32);
        qrImage.fill(Qt::white);
        
        // Rysowanie kodu QR
        for (int y = 0; y < qrSize; y++) {
            for (int x = 0; x < qrSize; x++) {
                if (qrCode->data[y * qrSize + x] & 1) {
                    // Czarny piksel
                    for (int dy = 0; dy < scale; dy++) {
                        for (int dx = 0; dx < scale; dx++) {
                            int px = (x + border) * scale + dx;
                            int py = (y + border) * scale + dy;
                            if (px < imageSize && py < imageSize) {
                                qrImage.setPixel(px, py, qRgb(0, 0, 0));
                            }
                        }
                    }
                }
            }
        }
        
        // Zwolnienie pamięci
        QRcode_free(qrCode);
        
        // Konwersja do pixmap i wyświetlenie
        m_currentQR = QPixmap::fromImage(qrImage);
        
        // Skalowanie do rozmiaru labela z zachowaniem proporcji
        QPixmap scaledQR = m_currentQR.scaled(m_qrLabel->size(), 
                                            Qt::KeepAspectRatio, 
                                            Qt::SmoothTransformation);
        
        m_qrLabel->setPixmap(scaledQR);
        
    } catch (const std::exception& e) {
        showError(QString("Błąd podczas generowania QR: %1").arg(e.what()));
    }
}

QString QRGenerator::generateVCard() const
{
    QString firstName = m_firstNameEdit->text().trimmed();
    QString lastName = m_lastNameEdit->text().trimmed();
    QString phone = m_phoneEdit->text().trimmed();
    QString email = m_emailEdit->text().trimmed();
    QString company = m_companyEdit->text().trimmed();
    QString website = m_websiteEdit->text().trimmed();
    
    // Sprawdź czy są jakieś dane
    if (firstName.isEmpty() && lastName.isEmpty() && phone.isEmpty() && 
        email.isEmpty() && company.isEmpty() && website.isEmpty()) {
        return QString();
    }
    
    // Generuj vCard
    QString vcard = "BEGIN:VCARD\n";
    vcard += "VERSION:3.0\n";
    
    if (!firstName.isEmpty() || !lastName.isEmpty()) {
        QString fullName = (firstName + " " + lastName).trimmed();
        vcard += "FN:" + fullName + "\n";
        vcard += "N:" + lastName + ";" + firstName + ";;;\n";
    }
    
    if (!phone.isEmpty()) {
        vcard += "TEL:" + phone + "\n";
    }
    
    if (!email.isEmpty()) {
        vcard += "EMAIL:" + email + "\n";
    }
    
    if (!company.isEmpty()) {
        vcard += "ORG:" + company + "\n";
    }
    
    if (!website.isEmpty()) {
        vcard += "URL:" + website + "\n";
    }
    
    vcard += "END:VCARD";
    
    return vcard;
}

QString QRGenerator::generateWiFiString() const
{
    QString ssid = m_wifiSSIDEdit->text().trimmed();
    if (ssid.isEmpty()) {
        return QString();
    }
    
    QString password = m_wifiPasswordEdit->text();
    QString security = m_wifiSecurityCombo->currentText();
    QString hidden = m_wifiHiddenCheck->isChecked() ? "true" : "false";
    
    // Format: WIFI:T:WPA;S:mynetwork;P:mypass;H:false;;
    return QString("WIFI:T:%1;S:%2;P:%3;H:%4;;")
           .arg(security)
           .arg(ssid)
           .arg(password)
           .arg(hidden);
}
