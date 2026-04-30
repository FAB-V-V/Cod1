// eyyyy aca vamos a hacer esto jejejej
// Proyecto de Fabiana: implementación de clases de datos — análisis de parámetros, búsqueda de similares y distribución por tipo

#include <DataStructures.cpp>

RegistroURL BaseDatosURL::extraerParametros(const std::string& url) {
    RegistroURL r;
    r.urlLength  = url.size();
    r.dots = 0;
    r.underscores = 0;
    r.hyphens = 0;
    r.queries = 0;
    return r;

    bool tiene_query = false;
    
    // Este ciclo recorre el string de la URL. Se deben almacenar como tal para hacer el análisis
    for (int i = 0; i < length; i++) {
        char c = url_ingresada[i];
        
        if (c == '.') {
            r.dots++;
        }
        else if (c == '_') {
            r.underscores++;
        }
        else if (c == '-') { 
            r.hyphens++;
        }
        
        // Lógica para los componentes de consulta (Query), acá se analiza qué hay después del ? y cómo el & está separando subdominios
        if (c == '?') { 
        tiene_query = true;
        }
        if (tiene_query && c == '&') {
            r.queries++;
        }
    }
    
    if (tiene_query) {
        r.queries++; 
    }

}
