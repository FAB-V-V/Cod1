// eyyyy aca vamos a hacer esto jejejej
// Proyecto de Fabiana: implementación de clases de datos — análisis de parámetros, búsqueda de similares y distribución por tipo

#include <DataStructures.cpp>

caracteristicas_URL analizar_una_url(string url_ingresada) {
    int dots = 0, underscores = 0, hyphens = 0, queries = 0;
    bool tiene_query = false;
    
    // Sacamos la longitud primero con .length()
    int length = url_ingresada.length();

    // Este ciclo recorre el string de la URL. Se deben almacenar como tal para hacer el análisis
    for (int i = 0; i < length; i++) {
        char c = url_ingresada[i];
        
        if (c == '.') {
            dots++;
        }
        else if (c == '_') {
            underscores++;
        }
        else if (c == '-') { 
            hyphens++;
        }
        
        // Lógica para los componentes de consulta (Query), acá se analiza qué hay después del ? y cómo el & está separando subdominios
        if (c == '?') { 
        tiene_query = true;
        }
        if (tiene_query && c == '&') {
            queries++;
        }
    }
    
    if (tiene_query) {
        queries++; 
    }

    // Después del análisis y con los contadores actualizados, toca mostrarlos según lo que se obtuvo
    return caracteristicas_URL(url_ingresada, length, dots, underscores, hyphens, queries);
}
