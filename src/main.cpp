#include "qrgenerator.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // Ustawienia aplikacji
    app.setApplicationName("QR Generator");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("QRGenerator");
    app.setOrganizationDomain("qrgenerator.local");
    
    // Ustaw styl aplikacji (opcjonalne)
    app.setStyle("Fusion");
    
    // Utwórz i pokaż główne okno
    QRGenerator window;
    window.show();
    
    return app.exec();
}
