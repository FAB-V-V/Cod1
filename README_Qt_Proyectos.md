# Guía Completa: Qt + C++ para Proyectos de Monitoria

## Índice
1. [¿Qué es Qt y cómo funciona?](#1-qué-es-qt-y-cómo-funciona)
2. [Setup: Instalación paso a paso](#2-setup-instalación-paso-a-paso)
3. [Cómo se escribe Qt: conceptos clave](#3-cómo-se-escribe-qt-conceptos-clave)
4. [Arquitectura modular del proyecto](#4-arquitectura-modular-del-proyecto)
5. [Leer archivos Excel (.xlsx) en Qt](#5-leer-archivos-excel-xlsx-en-qt)
6. [Qué va en cada archivo — lógica y semántica](#6-qué-va-en-cada-archivo--lógica-y-semántica)
7. [Proyecto de Mafe: Corrupción Colombia vs Europa](#7-proyecto-de-mafe-corrupción-colombia-vs-europa)
8. [Proyecto de Fabiana: Análisis de URLs](#8-proyecto-de-fabiana-análisis-de-urls)
9. [Flujo de trabajo recomendado](#9-flujo-de-trabajo-recomendado)

---

## 1. ¿Qué es Qt y cómo funciona?

### La idea general
Qt es un **framework de C++** que te da herramientas para hacer interfaces gráficas (ventanas, botones, tablas, gráficas) sin tener que dibujar píxeles manualmente. Piénsalo como una capa encima de C++ que ya tiene todo el trabajo de UI hecho.

### Componentes principales
```
Qt
├── Qt Core         → clases base: QString, QList, QFile, etc.
├── Qt Widgets      → ventanas, botones, tablas, layouts (la UI)
├── Qt Charts       → gráficas de barras, líneas, pie charts
├── Qt GUI          → manejo de imágenes, fuentes, eventos
└── Qt Network      → HTTP, sockets (no lo usamos aquí)
```

### El modelo Mental: Signals & Slots
Este es el concepto más importante de Qt. En C++ normal si quieres que al presionar un botón pase algo, conectas manualmente. Qt tiene su propio sistema llamado **signals & slots**:

```cpp
// Un botón emite una "signal" cuando lo presionan
// Un "slot" es una función que recibe esa signal

// Conectar: "cuando el botón sea clickeado, ejecuta esta función"
connect(boton, &QPushButton::clicked, this, &MiVentana::cargarDatos);
//      QUIÉN   QUÉ SIGNAL              EN QUIÉN  QUÉ FUNCIÓN
```

- **Signal**: evento que ocurre (click, cambio de texto, etc.)
- **Slot**: función que responde al evento
- **connect()**: une ambos

### El archivo .ui
Qt tiene un diseñador visual (Qt Designer) donde arrastras y sueltas botones, tablas, etc. Eso genera un archivo `.ui` (XML) que Qt convierte automáticamente en código C++. Puedes hacer la UI visualmente o en código puro — ambas formas funcionan.

### El sistema de build: qmake vs CMake
Qt usa su propio preprocesador llamado **MOC (Meta-Object Compiler)** que lee tu código antes de compilarlo y genera código extra para que funcionen los signals & slots. Esto pasa automático si usas:
- **qmake**: el sistema tradicional de Qt, usa archivos `.pro`
- **CMake**: el estándar moderno de C++, recomendado para proyectos nuevos

**Usaremos CMake** porque es más estándar y funciona mejor con proyectos modulares.

---

## 2. Setup: Instalación paso a paso

### Paso 1: Descargar el instalador de Qt
1. Ve a: https://www.qt.io/download-qt-installer
2. Descarga **Qt Online Installer** para Windows
3. Necesitas crear una cuenta gratuita en qt.io (te la pide durante la instalación)

### Paso 2: Qué seleccionar durante la instalación
Cuando el instalador te muestre el árbol de componentes, selecciona:

```
Qt
└── Qt 6.7.x (o la versión más reciente)
    ├── ✅ MinGW 64-bit          ← compilador (ya tienes MinGW pero instala este también)
    ├── ✅ Qt Charts             ← para hacer las gráficas comparativas
    └── ✅ Qt Designer           ← editor visual de interfaces

Developer and Designer Tools
    ├── ✅ Qt Creator 13.x       ← el IDE principal para Qt
    ├── ✅ MinGW 13.x 64-bit     ← asegúrate de que coincida con el Qt seleccionado
    └── ✅ CMake                 ← sistema de build
```

**Espacio aproximado:** ~8-12 GB con esta selección mínima.

### Paso 3: Agregar QXlsx (para leer Excel)
Qt no lee Excel nativo. Usaremos la librería **QXlsx**:

1. Descarga QXlsx desde: https://github.com/QtExcelGroup/QXlsx
2. Clona el repo o descarga ZIP:
   ```bash
   git clone https://github.com/QtExcelGroup/QXlsx.git
   ```
3. Se integra al proyecto via CMake (ver sección 5)

### Paso 4: Verificar que todo funciona
1. Abre **Qt Creator**
2. `File → New Project → Qt Widgets Application`
3. Selecciona el kit: `Desktop Qt 6.x MinGW 64-bit`
4. Dale Run (Ctrl+R) — debe abrir una ventana vacía

Si abre la ventana: todo está bien. Si hay error de compilador, revisar que el kit tenga el MinGW correcto configurado.

---

## 3. Cómo se escribe Qt: conceptos clave

### Estructura de un proyecto Qt (CMake)
```
MiProyecto/
├── CMakeLists.txt          ← configuración del proyecto
├── main.cpp                ← punto de entrada
├── mainwindow.h            ← declaración de la ventana principal
├── mainwindow.cpp          ← implementación de la ventana
├── mainwindow.ui           ← diseño visual (opcional)
├── data/
│   ├── DataStructures.h    ← TUS clases de datos (sin Qt)
│   ├── DataStructures.cpp
│   ├── DataLoader.h        ← carga Excel → llena las clases
│   └── DataLoader.cpp
└── QXlsx/                  ← librería para leer Excel
```

### CMakeLists.txt básico
```cmake
cmake_minimum_required(VERSION 3.16)
project(MiProyecto)

set(CMAKE_CXX_STANDARD 17)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Charts)

qt_add_executable(MiProyecto
    main.cpp
    mainwindow.h
    mainwindow.cpp
    data/DataStructures.h
    data/DataStructures.cpp
    data/DataLoader.h
    data/DataLoader.cpp
)

target_link_libraries(MiProyecto PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Charts
)
```

### Una clase Qt básica (MainWindow)
```cpp
// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QPushButton>
#include "data/DataStructures.h"

class MainWindow : public QMainWindow {
    Q_OBJECT                    // ← OBLIGATORIO en toda clase Qt con signals/slots

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:                  // ← funciones que responden a eventos
    void onCompararClicked();
    void onPaisSeleccionado(int index);

private:
    QPushButton* btnComparar;
    QComboBox*   comboPais1;
    QComboBox*   comboPais2;
    
    // Aquí viven tus datos (cargados desde Excel)
    std::vector<CasoCorrupcion> datos;
};

#endif
```

```cpp
// mainwindow.cpp
#include "mainwindow.h"
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    // Crear widgets
    btnComparar = new QPushButton("Comparar países", this);
    comboPais1  = new QComboBox(this);
    comboPais2  = new QComboBox(this);

    // Layout: organiza los widgets en la ventana
    QWidget* central = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(central);
    layout->addWidget(comboPais1);
    layout->addWidget(comboPais2);
    layout->addWidget(btnComparar);
    setCentralWidget(central);

    // Conectar signal → slot
    connect(btnComparar, &QPushButton::clicked, this, &MainWindow::onCompararClicked);
}

void MainWindow::onCompararClicked() {
    QString pais1 = comboPais1->currentText();
    QString pais2 = comboPais2->currentText();
    // ... lógica de comparación y graficado
}
```

### main.cpp siempre igual
```cpp
#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);   // ← inicializa Qt
    MainWindow window;
    window.show();
    return app.exec();              // ← event loop (espera clicks, etc.)
}
```

### Tipos Qt vs C++ estándar
| C++ estándar | Qt equivalente | Cuándo usar Qt |
|---|---|---|
| `std::string` | `QString` | Cuando vas a mostrarlo en UI |
| `std::vector<T>` | `QList<T>` | En la lógica de datos usa `std::vector` |
| `std::cout` | `qDebug()` | Para debug en Qt Creator |
| `int, double, bool` | igual | No cambian |

**Regla de oro**: En tus clases de datos (`DataStructures.h`) usa **C++ estándar puro** (`std::string`, `std::vector`). En el código Qt (`MainWindow`) convierte a Qt cuando sea necesario. Esto mantiene tus datos independientes de Qt.

---

## 4. Arquitectura modular del proyecto

### El principio de separación
```
┌─────────────────────────────────────────────────────┐
│                   DataStructures.h/.cpp              │
│                   (C++ PURO, sin Qt)                 │
│                                                      │
│   class CasoCorrupcion { ... };                      │
│   class PaisCorrupcion { ... };                      │
│   ← Aquí viven las entidades del dominio             │
└──────────────────────┬──────────────────────────────┘
                       │ usa
┌──────────────────────▼──────────────────────────────┐
│                   DataLoader.h/.cpp                  │
│                   (Qt para leer Excel)               │
│                                                      │
│   std::vector<CasoCorrupcion> cargarDesdeExcel(...); │
│   ← Usa QXlsx para leer, llena vectores de C++       │
└──────────────────────┬──────────────────────────────┘
                       │ usa
┌──────────────────────▼──────────────────────────────┐
│                   MainWindow.h/.cpp                  │
│                   (Qt Widgets + Qt Charts)           │
│                                                      │
│   ← Muestra datos, genera gráficas, maneja UI        │
└─────────────────────────────────────────────────────┘
```

### Por qué esta separación
- `DataStructures.h` no sabe nada de Qt — lo puedes reusar en otro programa sin Qt
- `DataLoader.h` es el único puente entre Excel y tus estructuras
- `MainWindow.h` solo se encarga de mostrar, no de procesar datos
- Si cambias la fuente de datos (CSV, base de datos, API), solo cambias `DataLoader.cpp`

### Flujo de ejecución
```
main() 
  → crea QApplication
  → crea MainWindow
    → MainWindow constructor llama DataLoader::cargar("archivo.xlsx")
      → DataLoader usa QXlsx para leer Excel fila por fila
      → por cada fila crea un objeto CasoCorrupcion y lo mete en std::vector
    → MainWindow recibe el vector lleno
    → Usuario selecciona países y presiona "Comparar"
    → MainWindow filtra el vector y dibuja la gráfica con Qt Charts
```

---

## 5. Leer archivos Excel (.xlsx) en Qt

### Integrar QXlsx al proyecto (CMakeLists.txt)
```cmake
# Agregar QXlsx como subdirectorio
add_subdirectory(QXlsx/QXlsx)

target_link_libraries(MiProyecto PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Charts
    QXlsx::QXlsx          # ← agregar esta línea
)
```

### Cómo leer un Excel con QXlsx
```cpp
#include "xlsxdocument.h"   // de QXlsx

void DataLoader::cargar(const std::string& rutaArchivo) {
    QXlsx::Document xlsx(QString::fromStdString(rutaArchivo));
    
    int fila = 2;   // empezar en 2 para saltar el encabezado (fila 1)
    
    while (true) {
        // Leer celda (fila, columna) — columnas son números: 1=A, 2=B, 3=C...
        QVariant celdaA = xlsx.read(fila, 1);
        
        // Si la celda está vacía, llegamos al final
        if (celdaA.isNull()) break;
        
        CasoCorrupcion caso;
        caso.pais      = xlsx.read(fila, 1).toString().toStdString();
        caso.anio      = xlsx.read(fila, 2).toInt();
        caso.monto     = xlsx.read(fila, 3).toDouble();
        // ... más columnas según tu Excel
        
        datos.push_back(caso);
        fila++;
    }
}
```

### Cómo encontrar qué columna es cuál
- Abre el Excel
- Fila 1 = encabezados (ej: "País", "Año", "Monto", "Tipo")
- Columna A = 1, B = 2, C = 3, etc.
- Mapea cada nombre de columna a su número

---

## 6. Qué va en cada archivo — lógica y semántica

Esta sección explica el **rol semántico** de cada archivo: qué responsabilidad tiene, qué tipo de código vive ahí, y qué no debe entrar nunca.

---

### `CMakeLists.txt` — el director de orquesta

**Qué es:** no es código C++, es el script que le dice a CMake cómo armar el ejecutable.

**Qué va ahí:**
- El nombre del proyecto y la versión de C++ (`set(CMAKE_CXX_STANDARD 17)`)
- Qué módulos de Qt necesita (`find_package(Qt6 REQUIRED COMPONENTS Core Widgets Charts)`)
- La lista de todos los `.cpp` y `.h` del proyecto (`qt_add_executable(...)`)
- Con qué librerías enlazar al final (`target_link_libraries(...)`)

**Semántica:** es la "receta de construcción". Si agregas un archivo nuevo al proyecto, **tienes que registrarlo aquí** o el compilador no lo verá. No hay lógica de negocio, no hay algoritmos — solo configuración.

**Error típico:** olvidar agregar un `.cpp` nuevo aquí y luego no entender por qué el linker dice "undefined reference".

---

### `main.cpp` — el punto de arranque

**Qué es:** el único `int main()` del programa. Son ~6 líneas y nunca deberían crecer mucho.

**Qué va ahí:**
```cpp
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);  // inicializa Qt y el event loop
    MainWindow w;                  // crea la ventana principal
    w.show();                      // la hace visible
    return app.exec();             // entra al event loop (espera clicks, etc.)
}
```

**Semántica:** su único trabajo es arrancar Qt y mostrar la ventana. **No va ninguna lógica aquí.** Si te encuentras escribiendo `if` o `for` en `main.cpp`, esa lógica pertenece a otro lado.

**Por qué `app.exec()` y no `return 0`:** `app.exec()` es un loop infinito que Qt corre internamente. Procesa clicks, redibuja la ventana, despacha eventos. Solo termina cuando el usuario cierra la ventana.

---

### `mainwindow.h` — el contrato de la ventana

**Qué es:** la declaración (`header`) de la clase `MainWindow`. Define qué existe, no cómo funciona.

**Qué va ahí:**
- Herencia de `QMainWindow` y el macro `Q_OBJECT` (obligatorio para signals/slots)
- Declaración de los **widgets** como atributos privados: botones, combos, tablas, el área de la gráfica
- Declaración de los **slots** (las funciones que responden a eventos del usuario)
- El objeto de datos (`BaseDatosCorrupcion db` o `BaseDatosURL db`) como atributo privado

**Semántica:** este archivo responde a la pregunta "¿qué tiene y qué sabe hacer la ventana?" sin decir cómo lo hace. Es el "contrato" que otros archivos pueden `#include` para saber con qué están trabajando.

**Regla clave:** en el `.h` solo van **declaraciones**, no implementaciones. Si escribes el cuerpo de una función acá (salvo getters de una línea), probablemente está en el lugar equivocado.

---

### `mainwindow.cpp` — el comportamiento de la ventana

**Qué es:** la implementación de todo lo declarado en `mainwindow.h`. Es el archivo más largo y donde ocurre la mayor parte del trabajo visible al usuario.

**Qué va ahí:**

| Sección | Qué hace |
|---|---|
| Constructor `MainWindow::MainWindow(...)` | Crea los widgets, los organiza en layouts, conecta signals→slots, llama a DataLoader para cargar los datos |
| Slots (`onCompararClicked`, `onAnalizarClicked`, etc.) | Responden a eventos del usuario: leen qué eligió, filtran datos, llaman a funciones de `BaseDatos*`, actualizan la gráfica |
| Función de graficado (`generarGrafica`, `actualizarChart`) | Traduce los datos del dominio (`std::vector`, `std::map`) a objetos Qt Charts (`QBarSeries`, `QLineSeries`) |
| Destructor | Libera recursos si es necesario (Qt maneja la memoria de widgets automáticamente vía el árbol de padre-hijo) |

**Semántica:** este archivo es el **puente entre los datos y el usuario**. Conoce tanto el dominio (`BaseDatosCorrupcion`) como Qt (widgets, charts). No procesa datos — delega a `BaseDatos*`. No sabe de Excel — eso ya lo hizo `DataLoader` en el constructor.

**Error típico:** meter lógica de negocio (filtros, cálculos estadísticos) directo en los slots. Eso va en `DataStructures.cpp`.

---

### `data/DataStructures.h` — las entidades del dominio

**Qué es:** las clases que modelan el problema real. **C++ puro, cero Qt.** No importa ningún header de Qt aquí.

**Qué va ahí:**
- Las clases que representan los datos del Excel (ej: `CasoCorrupcion`, `RegistroURL`)
- La clase "base de datos en memoria" que los contiene y tiene los métodos de consulta (`BaseDatosCorrupcion`, `BaseDatosURL`)
- Enums si hay categorías fijas (ej: `TipoURL`)
- Métodos de **filtrado y estadística**: `filtrarPorPais()`, `casosporAnio()`, `distribucionPorTipo()`

**Semántica:** este archivo representa el **modelo del mundo**. Un `CasoCorrupcion` existe independientemente de Qt, de la UI y de Excel. Si mañana haces una app de consola con los mismos datos, este archivo funciona sin tocar nada.

**Lo que no va acá:** nada de `QString`, `QList`, `QWidget`. Si aparece un `#include <Q...>`, algo está mal.

---

### `data/DataStructures.cpp` — la lógica del dominio

**Qué es:** la implementación de los métodos declarados en `DataStructures.h`.

**Qué va ahí:**
- Los algoritmos de filtrado y búsqueda sobre los vectores de datos
- Los cálculos estadísticos que después se grafican (contar casos por año, sumar montos, calcular porcentajes)
- Constructores no triviales de las clases de datos

**Semántica:** aquí vive la **inteligencia del programa** — la parte que razona sobre los datos. Si alguien pregunta "¿cuántos casos de corrupción hubo en Colombia entre 2010 y 2020?", la respuesta se calcula aquí.

**Ejemplo de lo que va:**
```cpp
std::vector<CasoCorrupcion> BaseDatosCorrupcion::filtrarPorPais(const std::string& pais) const {
    std::vector<CasoCorrupcion> resultado;
    for (const auto& caso : casos) {
        if (caso.pais == pais) resultado.push_back(caso);
    }
    return resultado;
}
```

---

### `data/DataLoader.h` — el contrato del cargador

**Qué es:** la declaración de la clase (o funciones) que leen el Excel y llenan las estructuras de datos.

**Qué va ahí:**
- La firma de la función principal: `std::vector<CasoCorrupcion> cargar(const std::string& ruta)`
- Cualquier helper privado para parsear tipos o validar filas

**Semántica:** es el **traductor** entre el mundo del Excel (filas, columnas, strings) y el mundo del dominio (objetos `CasoCorrupcion`, `RegistroURL`). Es el único archivo del proyecto que conoce la estructura interna del Excel.

---

### `data/DataLoader.cpp` — la lectura del Excel

**Qué es:** la implementación del cargador usando QXlsx.

**Qué va ahí:**
- La lógica de recorrer el Excel fila por fila con `QXlsx::Document`
- La conversión de cada celda al tipo correcto (`.toString().toStdString()`, `.toInt()`, `.toDouble()`)
- El mapeo de columna numérica a campo del objeto: columna 1 = `pais`, columna 2 = `anio`, etc.
- El manejo del caso "celda vacía = fin de datos"

**Semántica:** este es el único lugar donde el número de columna del Excel importa. Si el archivo Excel cambia de formato, **solo se toca este archivo**.

**Flujo interno:**
```
Abrir archivo xlsx
  → saltar fila 1 (encabezados)
  → por cada fila a partir de la 2:
      leer celda a celda → armar objeto del dominio → push_back al vector
  → retornar el vector lleno
```

---

### `QXlsx/` — librería externa (no la tocas)

**Qué es:** el código fuente de la librería QXlsx clonada desde GitHub.

**Qué va ahí:** nada tuyo. Es código de terceros que se compila junto al proyecto via `add_subdirectory(QXlsx/QXlsx)` en el CMakeLists.

**Lo único que hacés:** clonar el repo dentro de esta carpeta y registrarla en CMakeLists. Nunca editar archivos dentro de `QXlsx/`.

---

### Resumen visual de responsabilidades

```
¿Quién sabe qué?

CMakeLists.txt      → sabe cómo construir el ejecutable
main.cpp            → sabe cómo arrancar Qt
mainwindow.h/.cpp   → sabe cómo mostrar datos y responder al usuario
DataStructures.h/.cpp → sabe qué significan los datos y cómo consultarlos
DataLoader.h/.cpp   → sabe cómo leer el Excel específico de este proyecto
QXlsx/              → sabe leer archivos .xlsx (librería externa)

¿Quién NO sabe qué?

DataStructures      → no sabe nada de Qt ni de Excel
DataLoader          → no sabe nada de la UI ni de cómo se muestran los datos
MainWindow          → no sabe el formato del Excel ni los algoritmos de filtrado
main.cpp            → no sabe nada de nada, solo arranca
```

---

## 7. Proyecto de Mafe: Corrupción Colombia vs Europa

### Estructura de datos sugerida
```cpp
// data/DataStructures.h

class CasoCorrupcion {
public:
    std::string pais;
    std::string region;          // "Colombia" o país europeo
    int         anio;
    std::string tipoCaso;        // ej: "soborno", "malversación"
    double      montoInvolucrado;
    std::string sector;          // ej: "salud", "infraestructura"
    // [COMPLETAR con nombres reales de columnas del Excel]

    CasoCorrupcion() = default;
    CasoCorrupcion(std::string pais, int anio, double monto);
};

class BaseDatosCorrupcion {
public:
    std::vector<CasoCorrupcion> casos;

    void agregarCaso(const CasoCorrupcion& caso);
    
    // Filtros
    std::vector<CasoCorrupcion> filtrarPorPais(const std::string& pais) const;
    std::vector<CasoCorrupcion> filtrarPorAnio(int desde, int hasta) const;
    
    // Estadísticas para graficar
    std::map<int, int>    casosporAnio(const std::string& pais) const;
    std::map<int, double> montoPorAnio(const std::string& pais) const;
    std::vector<std::string> paisesDisponibles() const;
};
```

### UI sugerida
```
┌──────────────────────────────────────────────────────┐
│  [Cargar datos]                                      │
│  País 1: [▼ Colombia      ]  País 2: [▼ España   ]  │
│  Año desde: [2000] hasta: [2023]                     │
│  [Comparar]                                          │
├──────────────────────────────────────────────────────┤
│                                                      │
│   Gráfica de barras / líneas comparativa             │
│   (Qt Charts: QBarSeries o QLineSeries)              │
│                                                      │
└──────────────────────────────────────────────────────┘
```

### Gráfica comparativa con Qt Charts
```cpp
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>

void MainWindow::generarGrafica(const std::string& pais1, const std::string& pais2) {
    // Obtener datos filtrados
    auto datos1 = db.casosporAnio(pais1);
    auto datos2 = db.casosporAnio(pais2);
    
    // Crear barras
    QBarSet* set1 = new QBarSet(QString::fromStdString(pais1));
    QBarSet* set2 = new QBarSet(QString::fromStdString(pais2));
    
    QStringList categorias;  // años
    for (auto& [anio, count] : datos1) {
        *set1 << count;
        *set2 << datos2[anio];
        categorias << QString::number(anio);
    }
    
    QBarSeries* series = new QBarSeries();
    series->append(set1);
    series->append(set2);
    
    QChart* chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Comparación de casos de corrupción");
    
    QBarCategoryAxis* ejeX = new QBarCategoryAxis();
    ejeX->append(categorias);
    chart->addAxis(ejeX, Qt::AlignBottom);
    
    QChartView* chartView = new QChartView(chart);
    // Agregar chartView al layout de la ventana
}
```

---

## 8. Proyecto de Fabiana: Análisis de URLs

### Estructura de datos sugerida
```cpp
// data/DataStructures.h

enum class TipoURL {
    Benigna,
    Malware,
    Phishing,
    Defacement,
    Spam
    // [COMPLETAR según categorías en tu Excel]
};

class RegistroURL {
public:
    std::string url;
    TipoURL     tipo;
    // [COMPLETAR con columnas reales del Excel de 64k URLs]
    
    // Los 5 parámetros de análisis — [COMPLETAR con los reales]:
    double parametro1;   // ej: longitud del dominio
    double parametro2;   // ej: cantidad de caracteres especiales
    double parametro3;   // ej: tiene HTTPS (0 o 1)
    double parametro4;   // ej: cantidad de subdominios
    double parametro5;   // ej: edad del dominio

    RegistroURL() = default;
};

class TendenciaURL {
public:
    int    anio;
    int    mes;
    int    countBenignas;
    int    countMalware;
    int    countPhishing;
    // [COMPLETAR con columnas del Excel de tendencias]
};

class BaseDatosURL {
public:
    std::vector<RegistroURL>   urls;
    std::vector<TendenciaURL>  tendencias;

    // Analizar una URL nueva contra los 5 parámetros
    std::map<std::string, double> analizarURL(const std::string& url) const;
    
    // Buscar URLs similares en la base
    std::vector<RegistroURL> buscarSimilares(const std::string& url, int limite = 10) const;
    
    // Estadísticas de tendencias
    std::map<TipoURL, int> distribucionPorTipo() const;
};
```

### UI sugerida
```
┌──────────────────────────────────────────────────────┐
│  URL a analizar: [________________________] [Analizar]│
├───────────────────────────┬──────────────────────────┤
│  Resultado del análisis:  │  Tendencias históricas:  │
│                           │                          │
│  Parámetro 1: ████░░ 65%  │  Gráfica de líneas:      │
│  Parámetro 2: ██░░░░ 40%  │  Benignas ──────         │
│  Parámetro 3: █████░ 80%  │  Phishing - - - -        │
│  Parámetro 4: ███░░░ 55%  │  Malware  ......         │
│  Parámetro 5: ██░░░░ 35%  │                          │
│                           │                          │
│  Clasificación probable:  │                          │
│  ⚠ PHISHING (73%)         │                          │
└───────────────────────────┴──────────────────────────┘
```

---

## 9. Flujo de trabajo recomendado

### Orden para construir cada proyecto (uno a la vez)

```
Fase 1: Solo datos (sin Qt, sin UI)
├── Crear DataStructures.h con las clases
├── Crear DataLoader.h/.cpp que lea el Excel e imprima en consola
└── Compilar y verificar que los datos cargan bien

Fase 2: Ventana básica  
├── Crear MainWindow vacía que solo muestre una ventana
├── Conectar DataLoader → MainWindow (mostrar datos en una tabla Qt)
└── Verificar que la tabla muestra los datos correctamente

Fase 3: Lógica de negocio
├── Implementar filtros en BaseDatos* (por país, por año, etc.)
└── Conectar filtros a ComboBoxes y botones en la UI

Fase 4: Gráficas
├── Agregar Qt Charts al CMakeLists.txt
├── Implementar la función de graficado
└── Conectar al botón "Comparar" / "Analizar"
```

### Tips para no perderse
- **Compila después de cada cambio pequeño**, no acumules 200 líneas sin compilar
- **`qDebug() << variable`** es tu mejor amigo para debug (como `cout` pero aparece en Qt Creator)
- Si Qt Creator muestra error de "kit not found": `Tools → Kits → Desktop Qt 6.x MinGW 64-bit`
- Si hay error de `Q_OBJECT` no funcionando: revisa que el CMakeLists tenga `qt_add_executable` (no `add_executable`)

### Comandos útiles en Qt Creator
| Acción | Atajo |
|---|---|
| Compilar y ejecutar | `Ctrl + R` |
| Solo compilar | `Ctrl + B` |
| Ir a definición | `F2` |
| Ver output de debug | Panel "Application Output" abajo |

---

## Próximos pasos

1. ~~**Instala Qt**~~ — **Qt 6.11.0 ya está instalado** con MinGW 64-bit, Qt Creator y CMake ✓
2. **Corre los proyectos demo** en `deploy_mafe/` y `deploy_fabiana/` — doble clic en `compilar.bat` o abre en Qt Creator
3. **Clona QXlsx** cuando tengas el Excel real: `git clone https://github.com/QtExcelGroup/QXlsx.git`
4. **Conecta el Excel real** editando `DataLoader.cpp` (está comentado ahí exactamente qué cambiar)
5. Para el proyecto de Fabiana: confirmar cuáles son los 5 parámetros del dataset y ajustar `extraerParametros()` en `DataStructures.cpp`

---

## 10. Proyectos funcionales de demostración

Dentro de `C:\monitoria\` hay dos proyectos ya implementados y listos para compilar:

```
C:\monitoria\
├── deploy_mafe\        ← Comparador de corrupción Colombia vs Europa (FUNCIONAL)
├── deploy_fabiana\     ← Analizador de URLs con clasificación K-NN (FUNCIONAL)
└── modulos_qt\         ← Módulos reutilizables (VentanaBase, BotonEstilizado, etc.)
```

### Cómo abrirlos en Qt Creator

1. Abre **Qt Creator**
2. `File → Open File or Project...`
3. Navega a `deploy_mafe\CMakeLists.txt` o `deploy_fabiana\CMakeLists.txt`
4. Selecciona el kit: **Desktop Qt 6.11.0 MinGW 64-bit**
5. `Ctrl + R` para compilar y ejecutar

### Cómo compilar desde terminal (doble clic en el .bat)

```
deploy_mafe\compilar.bat     → compila y abre la app de Mafe
deploy_fabiana\compilar.bat  → compila y abre la app de Fabiana
```

### Qué hace cada demo

| Proyecto | Datos | UI | Gráficas |
|---|---|---|---|
| `deploy_mafe` | 90 casos demo (Colombia, España, Alemania, Francia, Italia) | ComboBox de países + rango de años | Barras agrupadas + líneas de monto |
| `deploy_fabiana` | 68 URLs demo clasificadas + tendencias 2021–2024 | Campo de URL + barras de parámetros | Líneas de tendencia + pie de distribución |

### Conectar los datos reales (Excel → demo)

En cada `DataLoader.cpp` hay un bloque de comentarios con el código exacto para reemplazar la función demo por la lectura real con QXlsx. Solo tienes que:
1. Clonar QXlsx en el proyecto
2. Agregar QXlsx al `CMakeLists.txt`  
3. Descomentar el código del bloque `// Para conectar el Excel real...`

---

## 11. Módulos Qt reutilizables

En `modulos_qt/` hay cuatro módulos independientes que se pueden copiar a cualquier proyecto:

```
modulos_qt/
├── VentanaBase.h/.cpp      ← ventana principal con barra de estado lista
├── BotonEstilizado.h/.cpp  ← botones con colores predefinidos (primario/éxito/peligro)
├── TablaDatos.h/.cpp       ← tabla con buscador en tiempo real
└── GraficaWidget.h/.cpp    ← widget de gráficas (barras, líneas, pie) de alto nivel
```

---

### `VentanaBase` — cómo crear una ventana principal

```cpp
// En tu ventana, hereda de VentanaBase en vez de QMainWindow:
#include "modulos_qt/VentanaBase.h"

class MiVentana : public VentanaBase {
    Q_OBJECT
public:
    MiVentana(QWidget* parent = nullptr)
        : VentanaBase("Título de mi app", 900, 600, parent)
    {
        // Tu widget central:
        QWidget* contenido = new QWidget();
        // ... armar layout, agregar botones, etc.
        ponerContenido(contenido);   // ← coloca el widget en la ventana
        setEstado("App lista.");     // ← escribe en la barra de estado
    }
};
```

**¿Qué agrega VentanaBase sobre QMainWindow?**
- Barra de estado ya configurada con estilo
- Método `setEstado(msg)` para mensajes rápidos al usuario
- Tamaño mínimo y título ya puestos en el constructor

---

### `BotonEstilizado` — cómo crear botones con color

```cpp
#include "modulos_qt/BotonEstilizado.h"
#include <QVBoxLayout>

// Dentro del constructor de tu ventana:
QVBoxLayout* layout = new QVBoxLayout(centralWidget);

auto* btnGuardar  = new BotonEstilizado("Guardar",   BotonEstilizado::Exito,   this);
auto* btnComparar = new BotonEstilizado("Comparar",  BotonEstilizado::Primario, this);
auto* btnEliminar = new BotonEstilizado("Eliminar",  BotonEstilizado::Peligro,  this);
auto* btnCancelar = new BotonEstilizado("Cancelar",  BotonEstilizado::Neutro,   this);

layout->addWidget(btnGuardar);
layout->addWidget(btnComparar);

// Conectar igual que cualquier QPushButton:
connect(btnComparar, &QPushButton::clicked, this, &MiVentana::onComparar);
```

| Variante | Color | Cuándo usarlo |
|---|---|---|
| `Primario` | Azul | Acción principal de la pantalla |
| `Exito` | Verde | Guardar, confirmar, cargar datos |
| `Peligro` | Rojo | Eliminar, limpiar, reset |
| `Neutro` | Gris | Cancelar, cerrar, acciones secundarias |

---

### `TablaDatos` — tabla con búsqueda en tiempo real

```cpp
#include "modulos_qt/TablaDatos.h"

// Crear con los nombres de las columnas:
auto* tabla = new TablaDatos({"País", "Año", "Tipo", "Monto (M$)", "Sector"}, this);
layout->addWidget(tabla, 1);  // el 1 le dice al layout que se expanda

// Agregar filas (cada fila es un QStringList):
for (const auto& caso : db.filtrarPorPais("Colombia")) {
    tabla->agregarFila({
        QString::fromStdString(caso.pais),
        QString::number(caso.anio),
        QString::fromStdString(caso.tipoCaso),
        QString::number(caso.monto, 'f', 1),
        QString::fromStdString(caso.sector)
    });
}

// Limpiar la tabla (ej: cuando el usuario cambia el filtro):
tabla->limpiar();

// Cuántas filas hay actualmente:
int n = tabla->totalFilas();
```

**El buscador filtra en todas las columnas al mismo tiempo** — si el usuario escribe "salud", solo se muestran las filas que tengan "salud" en cualquier columna.

---

### `GraficaWidget` — gráficas en tres líneas

```cpp
#include "modulos_qt/GraficaWidget.h"

auto* grafica = new GraficaWidget(this);
layout->addWidget(grafica, 1);

// ── Gráfica de BARRAS agrupadas ──────────────────────────────────────────
QStringList anios = {"2020", "2021", "2022", "2023"};

grafica->mostrarBarras("Casos por año",
    anios,
    {
        { "Colombia", {6, 5, 4, 4} },  // una barra roja por año
        { "España",   {1, 1, 1, 1} },  // una barra azul por año
    }
);
grafica->setTituloEjeX("Año");
grafica->setTituloEjeY("Número de casos");

// ── Gráfica de LÍNEAS múltiples ──────────────────────────────────────────
grafica->mostrarLineas("Monto involucrado (M$)",
    anios,
    {
        { "Colombia", {48.3, 38.9, 44.1, 51.6} },
        { "España",   {7.1,  6.8,  5.9,  5.3 } },
    }
);

// ── PIE CHART ────────────────────────────────────────────────────────────
std::map<QString, int> dist = {
    {"Benigna", 8200}, {"Phishing", 3100}, {"Malware", 2400}
};
grafica->mostrarPie("Distribución del dataset", dist);
```

**Nota**: `GraficaWidget` necesita `Qt6::Charts` en el `CMakeLists.txt`. Asegúrate de tenerlo en `target_link_libraries`.

---

### Cómo agregar un módulo a tu proyecto

1. Copia los dos archivos (`Modulo.h` y `Modulo.cpp`) dentro de tu carpeta de proyecto
2. Agrégalos al `CMakeLists.txt`:

```cmake
qt_add_executable(MiProyecto
    main.cpp
    mainwindow.h
    mainwindow.cpp
    data/DataStructures.h
    data/DataStructures.cpp
    data/DataLoader.h
    data/DataLoader.cpp
    # ↓ Agrega los módulos que vayas a usar:
    VentanaBase.h
    VentanaBase.cpp
    BotonEstilizado.h
    BotonEstilizado.cpp
    TablaDatos.h
    TablaDatos.cpp
    GraficaWidget.h
    GraficaWidget.cpp
)
```

3. Incluye el header donde los necesites:
```cpp
#include "BotonEstilizado.h"
#include "TablaDatos.h"
#include "GraficaWidget.h"
```

---

## 12. Dónde va cada cosa — guía rápida visual

```
¿Tengo que agregar X? ¿En qué archivo va?

X = una clase nueva de datos (CasoCorrupcion, RegistroURL, etc.)
    → DataStructures.h (declaración) + DataStructures.cpp (métodos)

X = leer una nueva columna del Excel
    → DataLoader.cpp — solo ahí

X = un cálculo nuevo (promedio, filtro, estadística)
    → DataStructures.cpp — un nuevo método de BaseDatos*

X = un botón nuevo en la ventana
    → mainwindow.h (declarar como atributo privado)
      mainwindow.cpp (crear en setupUI(), conectar signal→slot)

X = una pantalla / ventana secundaria nueva
    → Crear DialogoNuevo.h + DialogoNuevo.cpp
      mainwindow.cpp lo abre así:
        DialogoNuevo* d = new DialogoNuevo(this);
        d->exec();  // modal (bloquea) o d->show() (no bloquea)

X = una gráfica nueva
    → mainwindow.cpp — una función privada generarGraficaXxx()
      que llama a db.metodoDeDatos() y luego crea QChart + series

X = un widget reutilizable (tabla, botón especial, gráfica)
    → modulos_qt/ — su propio .h y .cpp independientes

X = algo que tiene que pasar "cuando el usuario hace Y"
    → es un SLOT en mainwindow.h (declarar bajo "private slots:")
      implementarlo en mainwindow.cpp
      conectar con connect(widget, &Widget::signal, this, &MainWindow::onX)
```

### El mapa completo de dependencias

```
main.cpp
  └─ crea QApplication y MainWindow

MainWindow (mainwindow.h/.cpp)
  ├─ usa DataLoader para cargar los datos al arrancar
  ├─ contiene BaseDatos* como atributo (los datos en memoria)
  ├─ crea todos los widgets en setupUI()
  ├─ conecta signals→slots en setupUI()
  └─ en cada slot: llama métodos de BaseDatos* y actualiza widgets

BaseDatosCorrupcion / BaseDatosURL (DataStructures.h/.cpp)
  ├─ contiene el vector principal de datos
  ├─ tiene todos los filtros y cálculos
  └─ NO sabe nada de Qt ni de la UI

DataLoader (DataLoader.h/.cpp)
  ├─ lee el Excel fila por fila con QXlsx
  ├─ construye objetos del dominio
  └─ devuelve BaseDatos* llena

QXlsx/ (librería externa — no se toca)
  └─ hace posible leer archivos .xlsx
```

---

*Generado para proyectos de Monitoria — C++ Qt con arquitectura modular*
