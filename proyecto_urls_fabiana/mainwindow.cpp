#include "mainwindow.h"
#include "data/DataLoader.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QString>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    db = DataLoader::cargar();
    setupUI();
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI() {
    QWidget* central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout* mainLayout = new QVBoxLayout(central);

    // Fila de entrada
    QHBoxLayout* inputRow = new QHBoxLayout();
    inputURL     = new QLineEdit(this);
    btnAnalizar  = new QPushButton("Analizar", this);
    btnCargarCSV = new QPushButton("Cargar CSV", this);
    inputURL->setPlaceholderText("Ingrese una URL...");
    inputRow->addWidget(inputURL);
    inputRow->addWidget(btnAnalizar);
    inputRow->addWidget(btnCargarCSV);
    mainLayout->addLayout(inputRow);

    // Etiquetas de resultado
    lblClasificacion = new QLabel("Clasificación: -", this);
    lblConfianza     = new QLabel("Confianza: -",     this);
    mainLayout->addWidget(lblClasificacion);
    mainLayout->addWidget(lblConfianza);

    // Barras de características de la URL
    auto addBar = [&](const QString& label, QProgressBar*& bar, int max) {
        QHBoxLayout* row = new QHBoxLayout();
        row->addWidget(new QLabel(label, this));
        bar = new QProgressBar(this);
        bar->setRange(0, max);
        row->addWidget(bar);
        mainLayout->addLayout(row);
    };
    addBar("url_length",      url_length,      300);
    addBar("cant_dot",        cant_dot,         20);
    addBar("cant_underscore", cant_underscore,  10);
    addBar("cant_hyphen",     cant_hyphen,      10);
    addBar("cant_query",      cant_query,       10);

    mainLayout->addWidget(new QLabel("Distribución vecinos más cercanos:", this));
    addBar("Benigna",    barBenigna,    5);
    addBar("Phishing",   barPhising,    5);
    addBar("Defacement", barDefacement, 5);
    addBar("Maleware",   barMaleware,   5);

    connect(btnAnalizar,  &QPushButton::clicked, this, &MainWindow::onAnalizarClicked);
    connect(btnCargarCSV, &QPushButton::clicked, this, &MainWindow::onCargarCSVClicked);
}

void MainWindow::mostrarResultado(const ResultadoAnalisis& r) {
    static const char* nombres[] = {"Benigna", "Phishing", "Maleware", "Defacement", "Desconocida"};
    int idx = (int)r.clasificacion;
    lblClasificacion->setText(QString("Clasificación: %1").arg(nombres[idx]));
    lblConfianza->setText(QString("Confianza: %1%").arg((double)r.confianza, 0, 'f', 1));

    url_length->setValue(r.urlLength);
    cant_dot->setValue(r.dots);
    cant_underscore->setValue(r.underscores);
    cant_hyphen->setValue(r.hyphens);
    cant_query->setValue(r.queries);

    auto get = [&](TipoURL t) -> int {
        auto it = r.distribucion.find(t);
        return (it != r.distribucion.end()) ? it->second : 0;
    };
    barBenigna->setValue(get(TipoURL::Benigna));
    barPhising->setValue(get(TipoURL::Phishing));
    barDefacement->setValue(get(TipoURL::Defacement));
    barMaleware->setValue(get(TipoURL::Maleware));
}

void MainWindow::onAnalizarClicked() {
    QString url = inputURL->text().trimmed();
    if (url.isEmpty()) return;
    ResultadoAnalisis resultado = db.analizar(url.toStdString());
    mostrarResultado(resultado);
}

void MainWindow::onCargarCSVClicked() {
    QString ruta = QFileDialog::getOpenFileName(this, "Abrir CSV", "", "CSV (*.csv)");
    if (ruta.isEmpty()) return;
    db = DataLoader::cargar(ruta.toStdString());
}
