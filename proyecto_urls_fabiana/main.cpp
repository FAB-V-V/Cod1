#include <QApplication>
#include "mainwindow.h"

// Se crear el objeto QApplication, el cual, maneja el ciclo de eventos, todo lo visual, los argumentos del sistema operativo y mostrar la ventana principal
int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();        //Pone el programa en un bucle infinito que espera eventos que haga el usuario (clicks) y los despacha a donde corresponda
}
