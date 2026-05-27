#include "mainwindow.h"
#include "data/DataLoader.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QString>
#include <QFileDialog>
#include <QFont>
#include <QChart>
#include <QChartView>
#include <QBarSeries>
#include <QBarCategoryAxis>
#include <QLinearGradient>
#include <QPainter>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    db = DataLoader::cargar();
    setupUI();
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
    btnCargarCSV = new QPushButton("Cargar CSV", this);
    inputURL->setPlaceholderText("Ingrese una URL...");
    inputRow->addWidget(inputURL);
    inputRow->addWidget(btnAnalizar);
    inputRow->addWidget(btnCargarCSV);
    mainLayout->addLayout(inputRow);

    // Etiquetas de resultado
    lblClasificacion = new QLabel("Clasificación: -", this);
    lblConfianza     = new QLabel("Confianza: -",     this);
    lblClasificacion->setFont(QFont("Segoe UI", 11, QFont::Bold));
    mainLayout->addWidget(lblClasificacion);
    mainLayout->addWidget(lblConfianza);

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

    connect(btnAnalizar,  &QPushButton::clicked, this, &MainWindow::onAnalizarClicked);
    connect(btnCargarCSV, &QPushButton::clicked, this, &MainWindow::onCargarCSVClicked);

    resize(720, 720);
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

void MainWindow::onCargarCSVClicked() {
    QString ruta = QFileDialog::getOpenFileName(this, "Abrir CSV", "", "CSV (*.csv)");
    if (ruta.isEmpty()) return;
    db = DataLoader::cargar(ruta.toStdString());
}
