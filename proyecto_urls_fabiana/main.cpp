// eyyyy aca vamos a hacer esto jejejej
// Proyecto de Fabiana: main — arranca la aplicación Qt y muestra la ventana de análisis de URLs

#include <QApplication>
#include "mainwindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();
}
