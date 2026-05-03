# Pasos a seguir — Proyecto Clasificador de URLs

Hola Fabiana. Este documento te explica qué te falta hacer, en qué orden hacerlo, y **por qué** cada decisión importa. No es solo una lista de tareas: es para que entiendas la lógica detrás de cómo se estructura un proyecto así.

---

## Diagnóstico rápido

Antes de empezar, esto es lo que hay ahora mismo en tu proyecto:

| Archivo | Estado |
|---|---|
| `CMakeLists.txt` | Vacío (solo tiene un comentario) |
| `main.cpp` | Vacío |
| `mainwindow.cpp` | Vacío |
| `data/DataStructures.cpp` | Vacío |
| `data/DataLoader.h` | Vacío |
| `data/DataLoader.cpp` | Vacío |
| `mainwindow.h` | Parcialmente escrito, pero con errores que impiden compilar |
| `data/DataStructures.h` | Parcialmente escrito, con errores |
| `clases.cpp` | Código duplicado y con errores — este archivo no debería existir |

**El proyecto actualmente no compila.** Hay que arreglarlo paso a paso, de abajo hacia arriba: primero la configuración, luego los datos, luego la lógica, y al final la interfaz.

---

## Paso 1 — Eliminar `clases.cpp`

### Qué hacer
Borrá ese archivo del proyecto.

### Por qué
En C++ existe una regla llamada **ODR (One Definition Rule)**: cada función, clase o variable solo puede estar *definida* una vez en todo el programa. Podés *declararla* en varios lados (eso es para lo que sirven los `.h`), pero definirla — es decir, escribir el cuerpo — solo una vez.

Tu archivo `clases.cpp` define la clase `caracteristicas_URL` con sus métodos. Pero esa misma clase también aparece en `data/DataStructures.h` y en `mainwindow.h`. Cuando el compilador intenta juntar todo, encuentra la misma clase definida tres veces y no sabe cuál usar, entonces falla.

La solución no es "elegir cuál de los tres se queda": hay que reorganizar todo desde cero (eso lo hacemos en los pasos 4 y 5) y este archivo queda completamente de más.

---

## Paso 2 — Arreglar `CMakeLists.txt`

### Qué hacer
Reemplazá el contenido actual (que es solo un comentario) por esto:

```cmake
cmake_minimum_required(VERSION 3.16)
project(proyecto_urls_fabiana VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Charts)

qt_standard_project_setup()

qt_add_executable(proyecto_urls_fabiana
    WIN32
    main.cpp
    mainwindow.h
    mainwindow.cpp
    data/DataStructures.h
    data/DataStructures.cpp
    data/DataLoader.h
    data/DataLoader.cpp
)

target_link_libraries(proyecto_urls_fabiana
    PRIVATE Qt6::Core Qt6::Widgets Qt6::Charts
)
```

### Por qué
`CMakeLists.txt` es el **mapa de construcción** del proyecto. Cuando apretás "compilar" en Qt Creator, el IDE no sabe mágicamente qué archivos juntar ni qué librerías de Qt necesitás. Lee este archivo para saberlo.

Cada línea tiene un propósito concreto:

- `cmake_minimum_required(VERSION 3.16)`: indica la versión mínima de CMake. Si alguien tiene una versión vieja, le avisa que actualice en lugar de fallar con un error críptico.
- `set(CMAKE_CXX_STANDARD 17)`: le dice al compilador que use C++17. Necesitás esto porque vas a usar cosas como `std::count_if`, inicialización de structs con `{}`, y otras características modernas.
- `find_package(Qt6 REQUIRED COMPONENTS Core Widgets Charts)`: le dice a CMake que busque Qt6 instalado en la computadora. `Core` es la base, `Widgets` es para la interfaz gráfica (botones, ventanas, etc.), y `Charts` es para las gráficas de torta y barras.
- `qt_add_executable(...)`: lista todos tus archivos fuente. Si creás un archivo nuevo y no lo agregás acá, el compilador ni lo va a ver.
- `WIN32`: hace que en Windows el ejecutable no abra una consola negra de fondo cuando lo corrés.
- `target_link_libraries(...)`: conecta tu programa con las librerías de Qt. Sin esto, el compilador encuentra las declaraciones pero no el código real de Qt, y falla al momento de linkear.

---

## Paso 3 — Arreglar `main.cpp`

### Qué hacer
Reemplazá el contenido por esto:

```cpp
#include <QApplication>
#include "mainwindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();
}
```

### Por qué
`main()` es el punto de entrada de cualquier programa en C++. Cuando ejecutás el programa, el sistema operativo llama a esta función y desde acá arranca todo.

En Qt específicamente, `main()` tiene una responsabilidad muy acotada: crear el objeto `QApplication` (que maneja el ciclo de eventos, los temas visuales, los argumentos del sistema operativo) y mostrar la ventana principal. Nada más.

`app.exec()` es la línea más importante: pone el programa en un bucle infinito que espera eventos del usuario (clicks, teclas, etc.) y los despacha a donde corresponde. Cuando el usuario cierra la ventana, `exec()` retorna y el programa termina limpiamente.

**Por qué no poner lógica acá:** todo lo que no sea "arrancar Qt y mostrar la ventana" pertenece a otras clases. Si mezclaras lógica en `main()`, sería imposible de testear y de mantener. La regla es: `main()` arranca, `MainWindow` hace.

---

## Paso 4 — Rediseñar `data/DataStructures.h`

### Por qué este archivo es el más importante
Todo el proyecto gira alrededor de un problema: dado una URL, decir si es benigna o maliciosa. Para resolver ese problema, el programa necesita trabajar con datos de cierta forma. `DataStructures.h` define exactamente esa forma: los "moldes" que va a usar todo el código.

Si este archivo está mal, todo lo demás falla en cascada. Por eso se empieza acá.

### 4.1 — Un enum para clasificar URLs

```cpp
enum class TipoURL { Benigna = 0, Maliciosa = 1 };
```

**Por qué un `enum` y no un `string`:** podrías guardar la clasificación como el texto `"Benigna"` o `"Maliciosa"`, pero eso tiene problemas. Si en algún lado escribís `"benigna"` (minúscula) o `"Beninga"` (typo), el programa acepta el valor sin quejarse y el error es silencioso. Un `enum` hace que el compilador acepte solo `TipoURL::Benigna` o `TipoURL::Maliciosa`, y si escribís cualquier otra cosa, el programa directamente no compila. Los errores en tiempo de compilación son siempre preferibles a los errores en tiempo de ejecución.

**Por qué `enum class` y no `enum` a secas:** el `enum class` (llamado "enum fuertemente tipado") obliga a escribir `TipoURL::Benigna` en lugar de solo `Benigna`. Eso evita colisiones de nombres cuando tenés varios enums en el mismo proyecto.

### 4.2 — Un struct para cada URL del dataset

```cpp
struct RegistroURL {
    TipoURL tipo;
    int numDots;         // cantidad de puntos en la URL
    int urlLength;       // longitud total del string
    int numDash;         // cantidad de guiones
    int atSymbol;        // 1 si tiene @, 0 si no
    int numUnderscore;   // cantidad de _
    int numPercent;      // cantidad de %
    int numAmpersand;    // cantidad de &
    int numNumericChars; // cantidad de dígitos (0-9)
    int noHttps;         // 1 si NO empieza con https://, 0 si sí
};
```

**Por qué estas 9 características:** el algoritmo K-NN (que viene en el paso 5) necesita comparar URLs numéricamente. No puede comparar el texto `"http://banco.com/login"` con `"http://ph1sh1ng-banco.malware.ru/login@user"` directamente, porque los strings son cualitativos. Pero sí puede comparar `[2, 23, 0, 0, 0, 0, 0, 0, 0]` contra `[4, 46, 1, 1, 0, 0, 0, 3, 1]` y decir que el segundo es más sospechoso.

Estas 9 características son señales estadísticas que se sabe que distinguen URLs maliciosas de benignas. Por ejemplo: las URLs de phishing tienden a ser largas, tener `@` para confundir al usuario (`http://banco.com@malware.ru`), no usar HTTPS, y tener muchos caracteres de escape como `%`.

**Por qué `struct` y no `class`:** en C++, la única diferencia real entre `struct` y `class` es que en `struct` todo es `public` por defecto y en `class` es `private`. Para contenedores de datos simples (como este, que solo guarda valores sin lógica compleja), `struct` es la convención habitual: es más conciso y comunica la intención de "esto es solo datos".

### 4.3 — Un struct para el resultado del análisis

```cpp
struct ResultadoAnalisis {
    std::string url;          // la URL que se analizó
    TipoURL clasificacion;    // la decisión del algoritmo
    double confianza;         // qué tan seguro está (0.0 a 1.0)
    double scoreBenigna;      // "votos" a favor de benigna
    double scoreMaliciosa;    // "votos" a favor de maliciosa
    // las 9 características que se extrajeron de esta URL
    int numDots, urlLength, numDash, atSymbol, numUnderscore;
    int numPercent, numAmpersand, numNumericChars, noHttps;
};
```

**Por qué separar esto de `RegistroURL`:** `RegistroURL` representa una URL que *ya está clasificada* y vive en el dataset. `ResultadoAnalisis` representa el resultado de *analizar* una URL nueva: además de la clasificación, incluye la confianza y los scores, que son cosas que el algoritmo K-NN produce y que la interfaz necesita mostrar. Son objetos con propósitos distintos aunque se parezcan.

**Por qué `double` para confianza y scores:** la confianza es un porcentaje expresado como decimal (0.85 significa "85% de confianza"). Los `int` no pueden representar fracciones, así que usamos `double`.

### 4.4 — La clase principal: la base de datos en memoria

```cpp
class BaseDatosURL {
public:
    std::vector<RegistroURL> urls;

    void agregar(const RegistroURL& r);
    ResultadoAnalisis analizar(const std::string& url) const;
    std::map<TipoURL, int> distribucionPorTipo() const;
    int total() const;

    static RegistroURL extraerParametros(const std::string& url);

private:
    double distancia(const RegistroURL& a, const RegistroURL& b) const;
};
```

**Por qué una clase y no funciones globales:** podrías tener funciones sueltas como `analizarURL(vector<RegistroURL>& datos, string url)`, pero eso obliga a pasar el vector cada vez que llamás a cualquier función. Agrupar los datos (`urls`) junto con las operaciones que los usan (`analizar`, `agregar`, etc.) dentro de una clase es el principio de **encapsulamiento**: el objeto sabe cuidarse solo.

**Por qué `static extraerParametros`:** esta función no necesita acceder a ningún dato de la instancia (no usa `urls`), solo transforma un string en un `RegistroURL`. Marcándola `static` indicamos que es una utilidad de la clase, no una operación sobre los datos. La podés llamar como `BaseDatosURL::extraerParametros(url)` sin tener ningún objeto creado.

**Por qué `distancia` es `private`:** es un detalle de implementación interno del algoritmo K-NN. La interfaz gráfica, el DataLoader, y `main()` nunca necesitan calcular distancias directamente. Si algo es privado, el compilador te protege de usarlo por error desde afuera.

> **Error crítico a corregir:** el `int main()` que tenías dentro de un `.h` tiene que desaparecer. Los archivos `.h` son incluidos por múltiples `.cpp`. Si `main()` está en un `.h`, termina definida en todos los `.cpp` que lo incluyan, y el linker se queja porque hay varias funciones `main()` cuando debería haber exactamente una.

---

## Paso 5 — Implementar `data/DataStructures.cpp`

### 5.1 — Extraer las características de una URL

```cpp
RegistroURL BaseDatosURL::extraerParametros(const std::string& url) {
    RegistroURL r;
    r.urlLength       = url.size();
    r.numDots         = std::count(url.begin(), url.end(), '.');
    r.numDash         = std::count(url.begin(), url.end(), '-');
    r.atSymbol        = std::count(url.begin(), url.end(), '@');
    r.numUnderscore   = std::count(url.begin(), url.end(), '_');
    r.numPercent      = std::count(url.begin(), url.end(), '%');
    r.numAmpersand    = std::count(url.begin(), url.end(), '&');
    r.numNumericChars = std::count_if(url.begin(), url.end(), ::isdigit);
    r.noHttps         = (url.substr(0, 8) != "https://") ? 1 : 0;
    return r;
}
```

**Por qué este enfoque:** el análisis semántico de URLs (parsear el dominio, el path, los parámetros) es complejo. Este proyecto usa un enfoque más sencillo pero efectivo: ignorar el significado del texto y solo contar caracteres. Funciona porque los patrones estadísticos son consistentes — las URLs maliciosas tienden a tener estructuras diferentes a las benignas en esos conteos.

`std::count` recorre el string de principio a fin (`url.begin()` a `url.end()`) contando cuántas veces aparece un carácter dado. `std::count_if` hace lo mismo pero con una condición: `::isdigit` devuelve verdadero para cualquier carácter que sea un dígito del 0 al 9.

### 5.2 — El algoritmo K-NN

Este es el núcleo del proyecto. Necesitás entender bien la idea antes de codificarlo.

**El problema que resuelve:** tenés un dataset de miles de URLs ya clasificadas. Llega una URL nueva que nunca viste. ¿Cómo decidís si es benigna o maliciosa?

**La intuición detrás de K-NN:** las URLs similares tienden a tener la misma clasificación. Si la URL nueva es muy parecida (en sus 9 características) a 14 URLs maliciosas y solo a 1 benigna, probablemente es maliciosa.

**Cómo se mide la similitud — distancia euclidiana:**

Pensá cada URL como un punto en un espacio de 9 dimensiones (una por cada característica). La distancia entre dos puntos en ese espacio es:

```
d = sqrt( (a.numDots - b.numDots)² + (a.urlLength - b.urlLength)² + ... )
```

Cuanto menor la distancia, más parecidas son las dos URLs.

**El problema de escala:** `urlLength` puede valer 200, mientras que `atSymbol` vale 0 o 1. Sin normalización, la longitud dominaría la distancia y las otras características casi no influirían. La solución es dividir cada característica por su valor máximo en el dataset antes de calcular la distancia, dejando todo en el rango [0, 1].

**El algoritmo completo, paso a paso:**

1. Extraer los 9 parámetros de la URL nueva.
2. Calcular la distancia normalizada entre la URL nueva y *cada* URL del dataset.
3. Ordenar las distancias de menor a mayor y quedarse con las K más pequeñas (K=15 es un buen valor).
4. Los K vecinos "votan": cada uno vota por su tipo (Benigna o Maliciosa). El voto tiene peso inversamente proporcional a la distancia — un vecino muy cercano vale más que uno lejano.
5. Sumar los votos por tipo. El tipo ganador es la clasificación. La fracción de votos que obtuvo es la confianza.

```
peso de un vecino = 1 / distancia
```

Si la distancia es 0 (URL idéntica), el peso sería infinito, así que se maneja como caso especial retornando directamente la clasificación de esa URL.

**Si el dataset está vacío** (el usuario no cargó datos todavía), podés hacer una clasificación heurística simple:

```
si tiene @ → maliciosa
si no tiene https → sospechoso (suma puntos maliciosa)
si tiene muchos % o & → sospechoso
```

---

## Paso 6 — Implementar `data/DataLoader.h` y `DataLoader.cpp`

### Por qué existe esta clase separada
El clasificador (K-NN en `BaseDatosURL`) no debería saber de dónde vienen los datos. Solo sabe que tiene un `vector<RegistroURL>` y trabaja con él. `DataLoader` es el responsable de llenarlo, ya sea desde un CSV o desde datos hardcodeados de ejemplo.

Esta separación se llama **principio de responsabilidad única**: cada clase hace una sola cosa. Si mañana querés cargar datos desde una base de datos en lugar de un CSV, solo modificás `DataLoader`, sin tocar el algoritmo.

### `DataLoader.h` — la declaración

```cpp
#pragma once
#include "DataStructures.h"
#include <string>

class DataLoader {
public:
    static BaseDatosURL cargar(const std::string& rutaCSV = "");
private:
    static void cargarCSV(BaseDatosURL& db, const std::string& ruta);
    static void cargarDemo(BaseDatosURL& db);
};
```

Todos los métodos son `static` porque `DataLoader` no tiene estado propio — no necesita que crees un objeto, solo llamás `DataLoader::cargar(...)` directamente.

### `DataLoader.cpp` — la implementación

**`cargarDemo()`** es la función más importante para empezar a probar. Crea manualmente 10-20 registros típicos para que el K-NN tenga datos con qué trabajar aunque no tengas un CSV:

```cpp
void DataLoader::cargarDemo(BaseDatosURL& db) {
    // URLs benignas típicas: pocas características sospechosas
    db.agregar({TipoURL::Benigna, 2, 20, 0, 0, 0, 0, 0, 2, 0});  // google.com
    db.agregar({TipoURL::Benigna, 3, 30, 0, 0, 0, 0, 0, 0, 0});  // wikipedia.org/wiki/algo

    // URLs maliciosas típicas: muchas características sospechosas
    db.agregar({TipoURL::Maliciosa, 5, 80, 3, 1, 2, 4, 0, 10, 1}); // phishing largo
    db.agregar({TipoURL::Maliciosa, 4, 60, 2, 1, 0, 6, 3, 8,  1}); // con @ y sin https
    // ... al menos 5-10 de cada tipo
}
```

**`cargarCSV()`** abre el archivo con `std::ifstream`, lee línea por línea, y parte cada línea por el separador `;` para construir un `RegistroURL`. El formato esperado es:

```
NumDots;UrlLength;NumDash;AtSymbol;NumUnderscore;NumPercent;NumAmpersand;NumNumericChars;NoHttps;Label
2;25;0;0;0;0;0;3;0;Benigna
5;80;3;1;2;4;2;10;1;Maliciosa
```

La primera línea es el encabezado — hay que saltarla. Cada línea siguiente es una URL del dataset.

---

## Paso 7 — Arreglar `mainwindow.h`

### Error 1: `Q_OBJECT` en el lugar equivocado

Tu código tiene:
```cpp
class MainWindow : public QMainWindow {
private:
    Q_OBJECT   // ← MAL
```

Debe ser:
```cpp
class MainWindow : public QMainWindow {
    Q_OBJECT   // ← bien: antes de cualquier sección public/private
public:
    ...
```

**Por qué importa:** `Q_OBJECT` no es código C++ normal. Es un **macro** que Qt reemplaza por código generado automáticamente antes de que el compilador lo vea. Ese proceso se llama MOC (Meta-Object Compiler). El MOC espera encontrar `Q_OBJECT` al inicio del cuerpo de la clase. Si está dentro de `private:`, el MOC puede no procesarlo correctamente y el sistema de señales y slots — la forma en que Qt conecta botones con funciones — no va a funcionar.

### Error 2: la clase `caracteristicas_URL` dentro del `.h`

Tenés la clase `caracteristicas_URL` definida dentro de `mainwindow.h`. Esto está mal por dos razones:

1. **Organización:** `mainwindow.h` debería describir solo la ventana principal. Mezclar una clase de datos ahí hace el archivo confuso y difícil de mantener.
2. **Duplicación:** esa clase también aparece en `DataStructures.h` y `clases.cpp`, violando la ODR mencionada en el paso 1.

Borrala de `mainwindow.h` — esos datos van en `DataStructures.h`.

### Error 3: `#endif` sin `#ifndef`

Tenés al final del archivo:
```cpp
}
#endif
```

Pero no hay un `#ifndef MAINWINDOW_H` al principio que lo abra. Esto rompe el compilador.

La solución más simple: en lugar de usar `#ifndef` / `#define` / `#endif` (los "include guards" clásicos), usá simplemente:

```cpp
#pragma once
```

Al inicio del archivo, como primera línea. Hace exactamente lo mismo (evita que el archivo se incluya más de una vez) pero sin posibilidad de olvidarse de cerrar el guard.

### Cómo debería quedar el `.h`

```cpp
#pragma once
#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QProgressBar>
#include "data/DataStructures.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    void setupUI();
    void mostrarResultado(const ResultadoAnalisis& r);

    QLineEdit*    inputURL;
    QPushButton*  btnAnalizar;
    QPushButton*  btnCargarCSV;
    QLabel*       lblClasificacion;
    QLabel*       lblConfianza;
    QProgressBar* barNumDots;
    QProgressBar* barUrlLength;
    // ... una por cada feature ...
    QProgressBar* barBenigna;
    QProgressBar* barMaliciosa;
    BaseDatosURL  db;

private slots:
    void onAnalizarClicked();
    void onCargarCSVClicked();
};
```

**Por qué punteros (`QLineEdit*`) y no objetos directos (`QLineEdit`):** en Qt, los widgets son manejados por un sistema de padres e hijos. Cuando un widget padre se destruye, destruye automáticamente a todos sus hijos. Si usás punteros, Qt puede manejar ese árbol de objetos. Si usás objetos directos como miembros, el destructor automático de Qt puede intentar destruir algo que ya fue destruido, causando crashes.

---

## Paso 8 — Implementar `mainwindow.cpp`

Con todo lo anterior listo, la interfaz es la capa final. Los cuatro métodos clave:

### `setupUI()` — construir la interfaz

Se llama desde el constructor. Crea todos los widgets y los organiza en layouts:

```cpp
MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setupUI();
    db = DataLoader::cargar();  // carga datos demo al arrancar
}

void MainWindow::setupUI() {
    inputURL   = new QLineEdit(this);
    btnAnalizar = new QPushButton("Analizar", this);
    // ... crear el resto de widgets ...

    auto* layout = new QVBoxLayout;
    layout->addWidget(inputURL);
    layout->addWidget(btnAnalizar);
    // ... agregar el resto ...

    auto* central = new QWidget(this);
    central->setLayout(layout);
    setCentralWidget(central);

    connect(btnAnalizar, &QPushButton::clicked, this, &MainWindow::onAnalizarClicked);
    connect(btnCargarCSV, &QPushButton::clicked, this, &MainWindow::onCargarCSVClicked);
}
```

**Por qué `connect`:** Qt usa un sistema de **señales y slots** para comunicar eventos. Cuando el usuario hace click en `btnAnalizar`, el botón emite la señal `clicked()`. `connect` le dice a Qt "cuando esa señal se emita, llamá a `onAnalizarClicked()`". Es la forma en que Qt desacopla los widgets de la lógica.

### `onAnalizarClicked()` — procesar el click

```cpp
void MainWindow::onAnalizarClicked() {
    QString url = inputURL->text().trimmed();
    if (url.isEmpty()) return;
    ResultadoAnalisis resultado = db.analizar(url.toStdString());
    mostrarResultado(resultado);
}
```

Este método es el puente entre la UI y el algoritmo. Lee lo que el usuario escribió, se lo pasa al K-NN, y muestra el resultado.

### `mostrarResultado()` — actualizar la interfaz

```cpp
void MainWindow::mostrarResultado(const ResultadoAnalisis& r) {
    QString tipo = (r.clasificacion == TipoURL::Benigna) ? "BENIGNA" : "MALICIOSA";
    lblClasificacion->setText(tipo);

    // Colorear según resultado
    QString color = (r.clasificacion == TipoURL::Benigna) ? "green" : "red";
    lblClasificacion->setStyleSheet("color: " + color);

    // Actualizar barras de features (ajustá los maximos según tu dataset)
    barNumDots->setValue(r.numDots);
    barUrlLength->setValue(r.urlLength);
    // ...

    barBenigna->setValue(static_cast<int>(r.scoreBenigna * 100));
    barMaliciosa->setValue(static_cast<int>(r.scoreMaliciosa * 100));
}
```

### `onCargarCSVClicked()` — cargar un dataset nuevo

```cpp
void MainWindow::onCargarCSVClicked() {
    QString ruta = QFileDialog::getOpenFileName(this, "Cargar dataset", "", "CSV (*.csv)");
    if (ruta.isEmpty()) return;
    db = DataLoader::cargar(ruta.toStdString());
    lblClasificacion->setText("—");
}
```

`QFileDialog::getOpenFileName` abre el explorador de archivos del sistema operativo y retorna la ruta del archivo que el usuario seleccionó. Si el usuario cierra sin elegir nada, retorna un string vacío.

---

## Orden recomendado para trabajar

```
Paso 1 → Borrar clases.cpp
Paso 2 → CMakeLists.txt         (sin esto no podés compilar nada)
Paso 3 → main.cpp               (mínimo para que Qt arranque)
Paso 4 → DataStructures.h       (los tipos de datos — todo depende de esto)
Paso 5 → DataStructures.cpp     (el algoritmo K-NN — lo más importante)
Paso 6 → DataLoader             (para tener datos de prueba sin CSV)
Paso 7 → mainwindow.h           (corregir los errores)
Paso 8 → mainwindow.cpp         (la interfaz, lo último)
```

**Estrategia de compilación:** después de cada paso intentá compilar aunque la app no haga nada todavía. Un error en el paso 4 que lo encontrás en el paso 8 es mucho más difícil de rastrear que uno que lo encontrás apenas terminás el paso 4.

---

## Lo que ya tenés bien

- La idea de contar características de la URL (`cant_dot`, `cant_slash`, etc.) es exactamente la dirección correcta. Solo hay que reorganizarlo en el `struct RegistroURL` con los nombres que usa el proyecto.
- Tenés getters y setters bien estructurados. La lógica de encapsulamiento la entendés — solo hay que aplicarla en el lugar correcto.
- La clase `caracteristicas_URL` muestra que entendés cómo modelar datos. El problema no era el concepto sino la ubicación y la duplicación.

---

## Preguntas frecuentes

**¿Por qué no puedo tener `int main()` dentro de un `.h`?**

Los archivos `.h` se "copian y pegan" en cada `.cpp` que los incluye con `#include`. Si ponés `main()` en un `.h`, el compilador termina viendo múltiples definiciones de `main()` y el linker no sabe cuál es el punto de entrada real. El programa no compila. `main()` vive en un único `.cpp` y listo.

**¿Por qué los `.h` no tienen `#include` de todo?**

Porque cada `#include` en un `.h` se propaga a todos los archivos que lo incluyen. Si en `DataStructures.h` incluís `<QApplication>` (que no necesitás ahí), todos los archivos que usen `DataStructures.h` van a cargar `<QApplication>` aunque no lo necesiten, haciendo la compilación más lenta. La regla es: en un `.h`, incluí solo lo que ese `.h` necesita declarar. En el `.cpp`, incluí lo que esa implementación necesita usar.

**¿Por qué `Q_OBJECT` tiene que estar al principio?**

Qt tiene un paso de preprocesamiento llamado MOC (Meta-Object Compiler) que corre *antes* del compilador de C++. El MOC busca `Q_OBJECT` en las clases para generar el código del sistema de señales y slots. Si está dentro de una sección `private:`, el MOC puede no reconocerlo correctamente y el `connect(...)` de los botones simplemente no va a funcionar en tiempo de ejecución.

**¿Qué es K-NN en palabras simples?**

"Decime con quién te juntás y te diré quién sos." Si una URL nueva tiene las mismas características que 13 de 15 URLs maliciosas conocidas y solo 2 benignas, el algoritmo la clasifica como maliciosa con 86% de confianza. No "aprende" una fórmula — simplemente busca los casos más similares en el pasado y vota.

**¿Por qué usar `const std::string&` y no `std::string` en los parámetros?**

Cuando pasás un `std::string` por valor (`void f(std::string s)`), C++ copia todo el string cada vez que llamás a la función. Si el string es largo o la función se llama muchas veces, eso es tiempo y memoria desperdiciada. Pasar por referencia constante (`const std::string& s`) le dice al compilador "usá el original sin copiarlo, y prometé que no lo vas a modificar". Es la forma estándar de pasar strings y objetos que no querés modificar.

---

## Errores concretos que tenés ahora — encontralos y arreglalos vos

Esta sección describe exactamente qué está mal en cada archivo en este momento. No es una lista de soluciones: es una lista de pistas. Para cada error se explica qué está mal y por qué rompe el programa, pero la corrección la tenés que encontrar vos.

---

### Error A — `data/DataStructures.h`: el archivo no tiene include guard

Mirá la primera línea del archivo. Ahora mirá cómo empieza `data/DataLoader.h`. ¿Qué diferencia hay?

Los include guards (o `#pragma once`) evitan que el compilador procese el mismo archivo dos veces si se incluye desde varios lugares. Sin eso, todas las clases y funciones quedan definidas múltiples veces y el compilador falla con errores de "redefinición".

**Pregunta:** ¿Qué directiva de una sola línea necesitás agregar al principio del archivo para que esto no pase?

---

### Error B — `data/DataStructures.h`: `using namespace std` en un archivo `.h`

`using namespace std` le dice al compilador "cuando veas `string`, buscalo en el namespace `std`". Si eso está en un `.h`, se "contagia" a *todos* los archivos que lo incluyen, incluso si ellos no querían eso. En proyectos grandes esto genera colisiones de nombres muy difíciles de rastrear.

La regla es: nunca pongas `using namespace` en un `.h`. En un `.cpp` es aceptable porque solo afecta ese archivo.

**Pregunta:** Si sacás `using namespace std`, ¿cómo tenés que escribir `string` en todos los lugares donde aparece? ¿Y `vector`, `map`?

---

### Error C — `data/DataStructures.h`: `BaseDatosURL` usa tipos que no existen en este archivo

Mirá la sección `// 4. BASE DE DATOS EN MEMORIA`. La clase `BaseDatosURL` usa `RegistroURL`, `TipoURL` y `ResultadoAnalisis`. Ahora buscá en el mismo archivo dónde están definidos esos tipos. ¿Los encontrás?

No están. El compilador llega a `vector<RegistroURL>` y no sabe qué es `RegistroURL` porque nadie lo definió antes. Recordá que el compilador lee el archivo de arriba hacia abajo: si usás un tipo, ese tipo tiene que estar *antes* en el archivo.

**Pregunta:** ¿Qué tres cosas tenés que agregar al archivo (el `enum class TipoURL`, el `struct RegistroURL` y el `struct ResultadoAnalisis`) y en qué orden tienen que aparecer para que `BaseDatosURL` pueda usarlos?

---

### Error D — `data/DataStructures.h`: cuerpos de funciones en el `.h`

Las funciones `calcular_promedio`, `ordenar_lista`, `calcular_mediana`, `calcular_rango`, `elevar_potencia`, `calcular_varianza`, `calcular_desviacion` y `simular_procesamiento_base_datos` tienen su cuerpo completo (el código entre `{` y `}`) dentro del `.h`.

¿Cuál es el problema? Recordá lo que dice el paso 1 sobre ODR. Si este `.h` lo incluye `mainwindow.cpp` y también lo incluye `DataStructures.cpp`, el compilador ve esas funciones *definidas dos veces*, y el linker no sabe cuál de las dos usar.

La regla es: en el `.h` van solo las *declaraciones* (firma de la función, sin cuerpo). En el `.cpp` van las *definiciones* (con el cuerpo).

**Pregunta:** ¿Cómo se ve una declaración de función sin cuerpo? Ejemplo: `float calcular_promedio(int lista[], int tamano);` — notá el `;` al final y que no hay `{...}`. ¿Cuántas de tus funciones tenés que convertir a esa forma en el `.h` y mover el cuerpo al `.cpp`?

---

### Error E — `data/DataStructures.h`: `int copia[];` es C++ inválido

Mirá la función `calcular_mediana`. En la primera línea del cuerpo dice:

```cpp
int copia[];
```

Esto no compila en C++ estándar. Un array en C++ necesita tener su tamaño definido en tiempo de compilación (como `int copia[10]`) o usar un mecanismo dinámico. `int copia[]` sin tamaño es un array de tamaño desconocido — el compilador no sabe cuánta memoria reservar.

Ahora pensá en el problema de fondo: la función recibe `int tamano` que solo se conoce cuando el programa está corriendo. Si el dataset tiene 100 URLs, `tamano` es 100. Si tiene 64.000, es 64.000. Un array C con `[]` no puede cambiar de tamaño en tiempo de ejecución.

La solución está en la biblioteca estándar de C++. Hay un tipo de dato diseñado exactamente para esto: una lista que crece dinámicamente sin límite de tamaño. La declaración se incluye con `#include <vector>` y se usa así:

```cpp
std::vector<int> copia;   // lista vacía
copia.push_back(5);       // agrega el número 5
```

**Pregunta:** ¿Cómo reemplazarías la línea `int copia[];` y el loop que copia los datos, usando `std::vector<int>` en lugar de un array? ¿Cómo le pasarías ese vector a `ordenar_lista` si actualmente esa función espera `int lista[]`?

---

### Error F — `data/DataStructures.h`: falta `calcular_moda`

El proyecto ahora tiene **5 medidas** de tendencia central y dispersión:

| # | Nombre | Qué mide |
|---|--------|----------|
| 1 | Media (promedio) | El valor promedio de la lista |
| 2 | Mediana | El valor del centro cuando la lista está ordenada |
| 3 | **Moda** | El valor que **más veces se repite** |
| 4 | Rango | La diferencia entre el máximo y el mínimo |
| 5 | Desviación estándar | Qué tan dispersos están los datos alrededor de la media |

La moda no está implementada. Necesitás agregar `calcular_moda`.

**Algoritmo para la moda:** recorrés la lista contando cuántas veces aparece cada valor (un `map<int,int>` donde la clave es el valor y el contenido es cuántas veces apareció). Después buscás cuál tiene el conteo más alto. Ese es la moda.

**Pregunta:** ¿Cómo declarás esa función en el `.h` y cómo la implementás en el `.cpp`? ¿Qué retorna si la lista está vacía?

---

### Error G — `data/DataStructures.cpp`: incluye el archivo equivocado

La primera línea que no es comentario dice:

```cpp
#include <DataStructures.cpp>
```

Hay dos errores en esa sola línea:

1. **Nunca se hace `#include` de un `.cpp`**. Los `.cpp` no se incluyen: se compilan por separado y se "unen" al final. Si incluís un `.cpp`, estás pegando todo su código textualmente acá, y el linker termina viendo todo definido dos veces.

2. **Los ángulos `<>` son para headers del sistema** (como `<iostream>`, `<vector>`). Para archivos del proyecto propio se usan comillas: `"DataStructures.h"`.

**Pregunta:** ¿Qué línea debería ir ahí en su lugar?

---

### Error H — `data/DataStructures.cpp`: hay código después de un `return`

Mirá la función `extraerParametros`. En algún punto dice `return r;` y después del `return` sigue código (el loop que cuenta los caracteres). Ese código nunca se ejecuta — el `return` termina la función.

Además, ese loop usa variables que no existen en ese scope: `length` (no está declarada) y `url_ingresada` (el parámetro se llama `url`).

**Pregunta:** ¿Por qué el compilador no se queja del código muerto en muchos casos? ¿Qué tenés que hacer para que el loop realmente cuente los caracteres? Pista: mirá cómo lo hace `analizar_una_url` en `DataStructures.h`.

---

### Error I — `data/DataStructures.cpp`: los nombres de campo no existen

En el mismo loop problemático, se usan `r.dots`, `r.underscores`, `r.hyphens`, `r.queries`. Pero mirá el `struct RegistroURL` que definiste (o que vas a definir). ¿Esos campos existen con esos nombres?

El compilador es exacto con los nombres. `r.numDots` y `r.dots` son completamente distintos para él, aunque para vos signifiquen lo mismo.

**Pregunta:** ¿Cuáles son los nombres correctos de los 9 campos de `RegistroURL`? Pista: los definís vos cuando escribas el struct en el paso anterior.

---

### Error J — `data/DataLoader.cpp`: le faltan los includes y la mitad del código

Abrí el archivo. Lo que hay ahora es solo la función `cargarDemo`, y sin los `#include` necesarios para que compile.

Hay dos cosas que faltan:

**1. Los includes.** El `.cpp` usa `BaseDatosURL`, `TipoURL` y `RegistroURL`. ¿Qué archivo `.h` los declara? Ese `#include` tiene que estar al principio.

**2. La función `cargarCSV`** — la que abre un archivo CSV, lee línea por línea, y construye un `RegistroURL` por cada fila. Esta es la función que le permite al programa cargar un dataset real en lugar de los datos de ejemplo. Está declarada en el `.h` pero no implementada en el `.cpp`.

El formato del CSV que tiene que leer es:
```
NumDots;UrlLength;NumDash;AtSymbol;NumUnderscore;NumPercent;NumAmpersand;NumNumericChars;NoHttps;Label
2;25;0;0;0;0;0;3;0;0
```
Separador `;`, primera línea es encabezado (hay que saltearla), `Label` es `0` para Benigna y `1` para Maliciosa.

Para leer archivos en Qt se usa `QFile` y `QTextStream`. Incluís `<QFile>` y `<QTextStream>`, y el patrón básico es:

```cpp
QFile archivo(QString::fromStdString(ruta));
if (!archivo.open(QIODevice::ReadOnly | QIODevice::Text)) return;  // si no abre, salir

QTextStream in(&archivo);
in.setEncoding(QStringConverter::Utf8);
in.readLine(); // saltar encabezado

while (!in.atEnd()) {
    QString linea = in.readLine();
    QStringList columnas = linea.split(';');
    // columnas[0] es el primer dato, columnas[1] el segundo, etc.
    // columnas[i].toInt() convierte el texto a número entero
}
```

**3. La función `cargar`** — el punto de entrada que decide si cargar del CSV o usar demo. Está declarada en el `.h` pero no existe en el `.cpp`.

**Pregunta:** ¿Cómo implementarías `cargar`? Si `rutaCSV` está vacío, llama a `cargarDemo`. Si no está vacío, llama a `cargarCSV`. Y si después de llamar a `cargarCSV` el vector de urls quedó vacío (porque el archivo no existía o tenía errores), llama igualmente a `cargarDemo` como respaldo.

---

### Error K — `mainwindow.h`: `private slots:` está afuera de la clase

Este es el error que hace que el programa no corra en Qt aunque compile.

Mirá el archivo. En la línea donde está el `};` que cierra la clase `MainWindow`, contá las llaves. Después de ese `};`, ¿qué aparece?

```cpp
};          // ← acá cierra MainWindow

private slots:       // ← esto está AFUERA de la clase
    void onAnalizarClicked();
    void onCargarCSVClicked();
};          // ← segundo }; que no cierra nada
```

`private slots:` es una sección especial que Qt entiende, pero *solo puede estar dentro de una clase que tenga `Q_OBJECT`*. Si está afuera, el compilador C++ no sabe qué hacer con eso y falla.

El `Q_OBJECT` está en `MainWindow`. Los slots tienen que estar dentro de `MainWindow`. La sección `private slots:` tiene que aparecer antes del `};` que cierra la clase.

**Pregunta:** ¿Dónde exactamente, dentro del cuerpo de la clase, ubicarías la sección `private slots:` con sus dos funciones?

---

### Error L — `mainwindow.h`: sobran llaves y un segundo `#pragma once`

Después de los problemas del error K, quedan en el archivo:
- Un `}` suelto (línea 59)
- Un `#pragma once` al final del archivo (línea 61)

El `#pragma once` al final no hace nada — solo tiene efecto al principio del archivo (ya lo tenés correcto en la línea 4). El `}` suelto rompe el parsing del compilador.

**Pregunta:** ¿Cuántas llaves de cierre necesita el archivo en total? Contá los `{` que abren y los `}` que cierran. Tenés que tener el mismo número de cada uno.

---

### Error M — `mainwindow.h`: declaraciones de funciones que ya no tienen sentido

Las líneas 48–58 declaran funciones como `comparacion_cant_length_URL_tendencia(...)`. Esas funciones nunca se implementaron, usan parámetros con nombres de campos que ya no existen, y no se llaman desde ningún lado.

Son restos de un diseño anterior que quedaron ahí y van a causar errores de linker ("función declarada pero no definida").

**Pregunta:** ¿Hay alguna razón para mantener esas declaraciones? Si no se implementan y no se usan, ¿qué debería pasar con ellas?

---

### Error N — `mainwindow.cpp`: está vacío

El archivo solo tiene dos líneas de comentario. Nada funciona hasta que este archivo esté implementado.

Los cuatro métodos que necesitás escribir acá son exactamente los mismos que describe el paso 8 de este documento: `MainWindow::MainWindow()` (constructor), `setupUI()`, `onAnalizarClicked()`, `onCargarCSVClicked()`, y `mostrarResultado()`.

---

## Nuevo enfoque: las 5 medidas aplicadas al dataset

Antes se pensaba en las medidas de tendencia central solo para analizar URLs individuales. El enfoque nuevo es aplicarlas a las **columnas del dataset**:

Cuando cargás un CSV con 10.000 URLs, el dataset tiene 9 columnas numéricas (una por feature). Para la columna `urlLength`, por ejemplo, podés calcular:
- Media: el largo promedio de todas las URLs en el dataset
- Mediana: el largo "del centro" cuando ordenás todas las URLs por longitud
- Moda: el largo que más se repite
- Rango: diferencia entre la URL más larga y la más corta
- Desviación estándar: qué tan variadas son las longitudes

Esto es valioso porque te dice "las URLs maliciosas en este dataset promedian 150 caracteres, las benignas 35".

Para poder hacer esto, `BaseDatosURL` necesita un método que dado un índice de feature (0 para numDots, 1 para urlLength, etc.) devuelva todos los valores de esa columna como un `vector<int>`. Con ese vector podés llamar a cualquiera de las 5 funciones estadísticas.

**Pregunta:** ¿Cómo declararías ese método en el `.h`? ¿Cómo lo implementarías en el `.cpp`? Pista: tenés que recorrer `urls` y para cada `RegistroURL` acceder al campo que corresponde al índice.

---

## Orden actualizado para terminar el proyecto

```
[✓] CMakeLists.txt        — ya está bien
[✓] main.cpp              — ya está bien
[ ] DataStructures.h      — corregir errores A, B, C, D, E; agregar F (moda)
[ ] DataStructures.cpp    — corregir errores G, H, I; mover cuerpos del .h
[ ] DataLoader.cpp        — corregir error J (includes + cargarCSV + cargar)
[ ] mainwindow.h          — corregir errores K, L, M
[ ] mainwindow.cpp        — implementar desde cero (error N)
```

**Estrategia:** empezá siempre por `DataStructures.h` y compilá apenas lo terminés. Los errores que aparezcan ahí son los más difíciles de rastrear si los dejás para el final.
