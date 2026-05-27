#include "mainwindow.h"
#include "data/DataLoader.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QString>
#include <QFileDialog>
#include <QFileInfo>
#include <QFont>
#include <QChart>
#include <QChartView>
#include <QBarSeries>
#include <QBarCategoryAxis>
#include <QLinearGradient>
#include <QPainter>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    db = DataLoader::cargar();   // carga y une las dos bases por defecto
    setupUI();
    actualizarEstadoBD();        // refleja en la UI cuantos registros se cargaron
}

MainWindow::~MainWindow() {}

// Construye una gráfica de barras "bonita": fondo claro, animaciones, antialiasing
// y un degradado vertical verde -> azul en las barras.
QWidget* MainWindow::crearGrafica(const QString& titulo, const QStringList& categorias,
                                  QBarSet*& setSalida, QValueAxis*& ejeYSalida) {
    QBarSet* set = new QBarSet(titulo);
    for (int i = 0; i < categorias.size(); ++i)
        *set << 0;                              // empezamos todas las barras en 0

    // Degradado verde (arriba) -> azul (abajo). ObjectBoundingMode lo aplica a cada barra.
    QLinearGradient grad(0, 0, 0, 1);
    grad.setCoordinateMode(QGradient::ObjectBoundingMode);
    grad.setColorAt(0.0, QColor("#43E97B"));    // verde
    grad.setColorAt(1.0, QColor("#2196F3"));    // azul
    set->setBrush(grad);
    set->setBorderColor(QColor("#1565C0"));     // borde azul oscuro
    set->setLabelColor(QColor("#0D47A1"));

    QBarSeries* series = new QBarSeries();
    series->append(set);
    series->setLabelsVisible(true);             // muestra el valor encima de cada barra
    series->setLabelsPosition(QAbstractBarSeries::LabelsOutsideEnd);

    QChart* chart = new QChart();
    chart->addSeries(series);
    chart->setTitle(titulo);
    chart->setTitleFont(QFont("Segoe UI", 11, QFont::Bold));
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->hide();
    chart->setBackgroundBrush(QColor("#F5FBFF")); // fondo azul muy claro

    QBarCategoryAxis* ejeX = new QBarCategoryAxis();
    ejeX->append(categorias);
    chart->addAxis(ejeX, Qt::AlignBottom);
    series->attachAxis(ejeX);

    QValueAxis* ejeY = new QValueAxis();
    ejeY->setRange(0, 10);
    ejeY->setLabelFormat("%d");
    chart->addAxis(ejeY, Qt::AlignLeft);
    series->attachAxis(ejeY);

    QChartView* view = new QChartView(chart);
    view->setRenderHint(QPainter::Antialiasing);
    view->setMinimumHeight(220);

    setSalida  = set;
    ejeYSalida = ejeY;
    return view;
}

void MainWindow::setupUI() {
    QWidget* central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout* mainLayout = new QVBoxLayout(central);

    // Fila de entrada
    QHBoxLayout* inputRow = new QHBoxLayout();
    inputURL     = new QLineEdit(this);
    btnAnalizar  = new QPushButton("Analizar", this);
    inputURL->setPlaceholderText("Ingrese una URL...");
    inputRow->addWidget(inputURL);
    inputRow->addWidget(btnAnalizar);
    mainLayout->addLayout(inputRow);

    // Fila para cargar las dos bases de datos (phishing + benignas)
    QHBoxLayout* basesRow = new QHBoxLayout();
    btnCargarPhishing = new QPushButton("Cargar BD Phishing", this);
    btnCargarBenignas = new QPushButton("Cargar BD Benignas", this);
    basesRow->addWidget(btnCargarPhishing);
    basesRow->addWidget(btnCargarBenignas);
    mainLayout->addLayout(basesRow);

    // Etiquetas de resultado
    lblClasificacion = new QLabel("Clasificación: -", this);
    lblConfianza     = new QLabel("Confianza: -",     this);
    lblEstadoBD      = new QLabel("Base unida: -",    this);
    lblClasificacion->setFont(QFont("Segoe UI", 11, QFont::Bold));
    mainLayout->addWidget(lblClasificacion);
    mainLayout->addWidget(lblConfianza);
    mainLayout->addWidget(lblEstadoBD);

    // Gráfica 1: características de la URL
    mainLayout->addWidget(crearGrafica(
        "Características de la URL",
        {"Longitud", "Puntos", "Guiones bajos", "Guiones", "Queries"},
        setCaracteristicas, ejeCaracteristicas));

    // Gráfica 2: distribución de los vecinos más cercanos (K-NN)
    mainLayout->addWidget(crearGrafica(
        "Distribución vecinos más cercanos (K=5)",
        {"Benigna", "Phishing"},
        setDistribucion, ejeDistribucion));
    ejeDistribucion->setRange(0, 5);            // los votos van de 0 a K

    connect(btnAnalizar,       &QPushButton::clicked, this, &MainWindow::onAnalizarClicked);
    connect(btnCargarPhishing, &QPushButton::clicked, this, &MainWindow::onCargarPhishingClicked);
    connect(btnCargarBenignas, &QPushButton::clicked, this, &MainWindow::onCargarBenignasClicked);

    resize(720, 760);
}

void MainWindow::mostrarResultado(const ResultadoAnalisis& r) {
    static const char* nombres[] = {"Benigna", "Phishing", "Maleware", "Defacement", "Desconocida"};
    int idx = (int)r.clasificacion;
    lblClasificacion->setText(QString("Clasificación: %1").arg(nombres[idx]));
    lblConfianza->setText(QString("Confianza: %1%").arg((double)r.confianza, 0, 'f', 1));

    // Gráfica de características
    setCaracteristicas->replace(0, r.urlLength);
    setCaracteristicas->replace(1, r.dots);
    setCaracteristicas->replace(2, r.underscores);
    setCaracteristicas->replace(3, r.hyphens);
    setCaracteristicas->replace(4, r.queries);

    // reescalamos el eje Y para que la barra más alta no se salga de la gráfica
    int maxCaract = qMax(qMax(qMax(r.urlLength, r.dots), qMax(r.underscores, r.hyphens)), r.queries);
    ejeCaracteristicas->setRange(0, qMax(10, (int)(maxCaract * 1.15) + 1));
    ejeCaracteristicas->applyNiceNumbers();

    // Gráfica de distribución de vecinos
    auto get = [&](TipoURL t) -> int {
        auto it = r.distribucion.find(t);
        return (it != r.distribucion.end()) ? it->second : 0;
    };
    setDistribucion->replace(0, get(TipoURL::Benigna));
    setDistribucion->replace(1, get(TipoURL::Phishing));
}

void MainWindow::onAnalizarClicked() {
    QString url = inputURL->text().trimmed();
    if (url.isEmpty()) return;
    ResultadoAnalisis resultado = db.analizar(url.toStdString());
    mostrarResultado(resultado);
}

void MainWindow::onCargarPhishingClicked() {
    QString ruta = QFileDialog::getOpenFileName(this, "Abrir base de PHISHING", "", "CSV (*.csv)");
    if (ruta.isEmpty()) return;
    rutaPhishing = ruta;
    recargarBaseUnida();
}

void MainWindow::onCargarBenignasClicked() {
    QString ruta = QFileDialog::getOpenFileName(this, "Abrir base de BENIGNAS", "", "CSV (*.csv)");
    if (ruta.isEmpty()) return;
    rutaBenignas = ruta;
    recargarBaseUnida();
}

void MainWindow::recargarBaseUnida() {
    if (rutaPhishing.isEmpty() && rutaBenignas.isEmpty())
        return;

    // une las bases cargadas en una sola: esta es la que analiza el K-NN.
    db = DataLoader::unir(rutaPhishing.toStdString(), rutaBenignas.toStdString());

    // si ya estan las dos bases, guardamos la union en un tercer archivo
    // (junto a la base de phishing seleccionada), reconstruyendo el dataset original.
    if (!rutaPhishing.isEmpty() && !rutaBenignas.isEmpty()) {
        QString salida = QFileInfo(rutaPhishing).absolutePath() + "/BASE_DATOS_UNIDA.csv";
        DataLoader::guardarUnion(rutaPhishing.toStdString(),
                                 rutaBenignas.toStdString(),
                                 salida.toStdString());
    }

    actualizarEstadoBD();
}

void MainWindow::actualizarEstadoBD() {
    auto dist = db.distribucionPorTipo();
    int phishing = dist.count(TipoURL::Phishing) ? dist[TipoURL::Phishing] : 0;
    int benignas = dist.count(TipoURL::Benigna)  ? dist[TipoURL::Benigna]  : 0;
    lblEstadoBD->setText(QString("Base unida: %1 registros  (Phishing: %2, Benignas: %3)")
                             .arg(db.total()).arg(phishing).arg(benignas));
}
