// eyyyy aca vamos a hacer esto jejejej
// Proyecto de Fabiana: implementación de DataLoader — recorre fila por fila y construye cada RegistroURL y TendenciaURL

void DataLoader::cargarDemo(BaseDatosURL& db) {
    // URLs benignas típicas: pocas características sospechosas
    db.agregar({TipoURL::Benigna, 2, 20, 0, 0, 0, 0, 0, 2, 0});  // google.com
    db.agregar({TipoURL::Benigna, 3, 30, 0, 0, 0, 0, 0, 0, 0});  // wikipedia.org/wiki/algo

    // URLs maliciosas típicas: muchas características sospechosas
    db.agregar({TipoURL::Maliciosa, 5, 80, 3, 1, 2, 4, 0, 10, 1}); // phishing largo
    db.agregar({TipoURL::Maliciosa, 4, 60, 2, 1, 0, 6, 3, 8,  1}); // con @ y sin https
    // ... al menos 5-10 de cada tipo
}
