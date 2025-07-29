#include "qrgenerator.h"

// Implementacja pozostałych metod

void QRGenerator::saveQRImage()
{
    if (m_currentQR.isNull()) {
        showError("Brak kodu QR do zapisania");
        return;
    }
    
    QString fileName = QFileDialog::getSaveFileName(this,
        "Zapisz kod QR",
        QStandardPaths::writableLocation(QStandardPaths::PicturesLocation) + "/qr_code.png",
        "Pliki PNG (*.png)");
    
    if (!fileName.isEmpty()) {
        if (m_currentQR.save(fileName, "PNG")) {
            showInfo("Kod QR został zapisany jako: " + QFileInfo(fileName).fileName());
        } else {
            showError("Nie można zapisać pliku");
        }
    }
}

void QRGenerator::copyToClipboard()
{
    QClipboard* clipboard = QApplication::clipboard();
    
    // Sprawdź aktywną zakładkę i skopiuj odpowiednie dane
    int currentTab = m_tabWidget->currentIndex();
    QString textToCopy;
    
    switch (currentTab) {
        case 0: // URL
            textToCopy = m_urlEdit->text();
            break;
        case 1: // Tekst
            textToCopy = m_textEdit->toPlainText();
            break;
        case 2: // Kontakt
            textToCopy = generateVCard();
            break;
        case 3: // WiFi
            textToCopy = generateWiFiString();
            break;
        default:
            textToCopy = "Brak danych do skopiowania";
    }
    
    if (!textToCopy.isEmpty()) {
        clipboard->setText(textToCopy);
        showInfo("Skopiowano do schowka");
    }
}

void QRGenerator::clearQR()
{
    m_qrLabel->clear();
    m_qrLabel->setText("Kod QR pojawi się tutaj");
    m_currentQR = QPixmap();
}

void QRGenerator::copyResult()
{
    QString selectedText = m_resultTextEdit->textCursor().selectedText();
    
    if (selectedText.isEmpty()) {
        selectedText = m_resultTextEdit->toPlainText();
    }
    
    if (!selectedText.isEmpty()) {
        QApplication::clipboard()->setText(selectedText);
        showInfo("Wynik skopiowany do schowka");
    }
}

void QRGenerator::showError(const QString& message)
{
    QMessageBox::critical(this, "Błąd", message);
}

void QRGenerator::showInfo(const QString& message)
{
    QMessageBox::information(this, "Informacja", message);
}

// Metoda pomocnicza - createQRImage (opcjonalna - może być używana w przyszłości)
QPixmap QRGenerator::createQRImage(const QString& data, int size)
{
    // Ta metoda może być używana do tworzenia QR o określonym rozmiarze
    try {
        QRcode* qrCode = QRcode_encodeString(data.toUtf8().constData(), 
                                           0, QR_ECLEVEL_M, QR_MODE_8, 1);
        
        if (!qrCode) {
            return QPixmap();
        }
        
        int qrSize = qrCode->width;
        int scale = size / qrSize;
        if (scale < 1) scale = 1;
        
        int border = 4;
        int imageSize = (qrSize + 2 * border) * scale;
        
        QImage qrImage(imageSize, imageSize, QImage::Format_RGB32);
        qrImage.fill(Qt::white);
        
        for (int y = 0; y < qrSize; y++) {
            for (int x = 0; x < qrSize; x++) {
                if (qrCode->data[y * qrSize + x] & 1) {
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
        
        QRcode_free(qrCode);
        return QPixmap::fromImage(qrImage);
        
    } catch (...) {
        return QPixmap();
    }
}


