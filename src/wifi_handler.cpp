#include "qrgenerator.h"

// Implementacja metod obsługi WiFi

void QRGenerator::initializeEncryption()
{
    // Prosty klucz szyfrowania bazowany na nazwie użytkownika i komputera
    QString baseKey = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QCryptographicHash hash(QCryptographicHash::Sha256);
    hash.addData(baseKey.toUtf8());
    hash.addData("QRGenerator_Secret_Key_2023");
    m_encryptionKey = hash.result().toHex().left(32); // 32 znaki dla AES
}

QString QRGenerator::encryptPassword(const QString& password)
{
    if (password.isEmpty()) {
        return QString();
    }
    
    // Proste szyfrowanie XOR (w prawdziwej aplikacji użyj AES)
    QByteArray data = password.toUtf8();
    QByteArray key = m_encryptionKey.toUtf8();
    
    for (int i = 0; i < data.size(); ++i) {
        data[i] = data[i] ^ key[i % key.size()];
    }
    
    return data.toBase64();
}

QString QRGenerator::decryptPassword(const QString& encryptedPassword)
{
    if (encryptedPassword.isEmpty()) {
        return QString();
    }
    
    try {
        QByteArray data = QByteArray::fromBase64(encryptedPassword.toUtf8());
        QByteArray key = m_encryptionKey.toUtf8();
        
        for (int i = 0; i < data.size(); ++i) {
            data[i] = data[i] ^ key[i % key.size()];
        }
        
        return QString::fromUtf8(data);
    } catch (...) {
        return QString();
    }
}

void QRGenerator::loadWiFiNetworks()
{
    QFile file(m_wifiFile);
    if (!file.open(QIODevice::ReadOnly)) {
        return; // Plik nie istnieje lub nie można otworzyć
    }
    
    QByteArray data = file.readAll();
    file.close();
    
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);
    
    if (error.error != QJsonParseError::NoError) {
        showError("Błąd wczytywania zapisanych sieci WiFi: " + error.errorString());
        return;
    }
    
    QJsonObject root = doc.object();
    m_wifiNetworks.clear();
    
    for (auto it = root.begin(); it != root.end(); ++it) {
        WiFiNetwork network = WiFiNetwork::fromJson(it.value().toObject());
        m_wifiNetworks[it.key()] = network;
    }
}

void QRGenerator::saveWiFiNetworks()
{
    QJsonObject root;
    
    for (const auto& pair : m_wifiNetworks) {
        root[pair.first] = pair.second.toJson();
    }
    
    QJsonDocument doc(root);
    
    QFile file(m_wifiFile);
    if (!file.open(QIODevice::WriteOnly)) {
        showError("Nie można zapisać sieci WiFi do pliku");
        return;
    }
    
    file.write(doc.toJson());
    file.close();
}

void QRGenerator::saveWiFiNetwork()
{
    QString ssid = m_wifiSSIDEdit->text().trimmed();
    QString password = m_wifiPasswordEdit->text();
    
    if (ssid.isEmpty()) {
        showError("Podaj nazwę sieci (SSID)");
        return;
    }
    
    WiFiNetwork network;
    network.ssid = ssid;
    network.encryptedPassword = encryptPassword(password);
    network.security = m_wifiSecurityCombo->currentText();
    network.hidden = m_wifiHiddenCheck->isChecked();
    network.savedDate = QDateTime::currentDateTime();
    
    m_wifiNetworks[ssid] = network;
    saveWiFiNetworks();
    refreshWiFiList();
    
    showInfo(QString("Sieć '%1' została zapisana").arg(ssid));
}

void QRGenerator::loadSelectedWiFi()
{
    QListWidgetItem* item = m_wifiListWidget->currentItem();
    if (!item) {
        showError("Wybierz sieć z listy");
        return;
    }
    
    // Wyciągnij SSID z tekstu elementu (przed pierwszym nawiasem)
    QString displayText = item->text();
    QString ssid = displayText.split(" (").first();
    
    auto it = m_wifiNetworks.find(ssid);
    if (it != m_wifiNetworks.end()) {
        const WiFiNetwork& network = it->second;
        
        m_wifiSSIDEdit->setText(network.ssid);
        m_wifiPasswordEdit->setText(decryptPassword(network.encryptedPassword));
        
        // Znajdź i ustaw typ zabezpieczeń
        int securityIndex = m_wifiSecurityCombo->findText(network.security);
        if (securityIndex >= 0) {
            m_wifiSecurityCombo->setCurrentIndex(securityIndex);
        }
        
        m_wifiHiddenCheck->setChecked(network.hidden);
        
        // Wygeneruj kod QR
        generateWiFiQR();
    }
}

void QRGenerator::deleteSelectedWiFi()
{
    QListWidgetItem* item = m_wifiListWidget->currentItem();
    if (!item) {
        showError("Wybierz sieć z listy do usunięcia");
        return;
    }
    
    QString displayText = item->text();
    QString ssid = displayText.split(" (").first();
    
    int ret = QMessageBox::question(this, "Potwierdzenie", 
                                   QString("Czy na pewno chcesz usunąć sieć '%1'?").arg(ssid),
                                   QMessageBox::Yes | QMessageBox::No);
    
    if (ret == QMessageBox::Yes) {
        m_wifiNetworks.erase(ssid);
        saveWiFiNetworks();
        refreshWiFiList();
        showInfo(QString("Sieć '%1' została usunięta").arg(ssid));
    }
}

void QRGenerator::refreshWiFiList()
{
    m_wifiListWidget->clear();
    
    for (const auto& pair : m_wifiNetworks) {
        const WiFiNetwork& network = pair.second;
        QString displayText = QString("%1 (%2) - %3")
                             .arg(network.ssid)
                             .arg(network.security)
                             .arg(network.savedDate.toString("yyyy-MM-dd hh:mm"));
        
        m_wifiListWidget->addItem(displayText);
    }
}

void QRGenerator::onWiFiSelectionChanged()
{
    // Ta metoda może być używana do dodatkowych akcji przy zmianie selekcji
}

void QRGenerator::togglePasswordVisibility()
{
    m_passwordVisible = !m_passwordVisible;
    
    if (m_passwordVisible) {
        m_wifiPasswordEdit->setEchoMode(QLineEdit::Normal);
        m_showPasswordButton->setText("🙈");
        m_showPasswordButton->setToolTip("Ukryj hasło");
    } else {
        m_wifiPasswordEdit->setEchoMode(QLineEdit::Password);
        m_showPasswordButton->setText("👁");
        m_showPasswordButton->setToolTip("Pokaż hasło");
    }
}
