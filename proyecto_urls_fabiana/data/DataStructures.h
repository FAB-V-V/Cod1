// eyyyy aca vamos a hacer esto jejejej
// Proyecto de Fabiana: clases RegistroURL, TendenciaURL y BaseDatosURL — C++ puro, sin Qt

#include <iostream>
#include <string>

// CLASE
class caracteristicas_URL {
    private:
        string url;
        int cant_dot;
        int cant_slash;
        int cant_num;
        int cant_hyphen;
        int cant_underscore;
        int cant_ampersand;
        int cant_https;
        int cant_at;
        int cant_percent;
    
    public:
        //CONSTRUCTOR
        caracteristicas_URL(string url, int cant_dot, int cant_slash, int cant_num, int cant_hyphen, int cant_underscore, int cant_ampersand, int cant_https, int cant_at, int cant_percent) {
            this-> url = url;                       //this-> para asignar un puntero en un dirección de memoria
            this-> cant_dot = cant_dot ;
            this-> cant_slash = cant_slash ;
            this-> cant_num = cant_num ;
            this-> cant_hyphen = cant_hyphen ;      
            this-> cant_underscore = cant_underscore ;   
            this-> cant_ampersand = cant_ampersand ;   
            this-> cant_https = cant_https ;   
            this-> cant_at = cant_at ;   
            this-> cant_percent = cant_percent ;   
        }


        //DESTRUCTOR
        ~caracteristicas_URL() {};
                
        // GETTERS
        string get_url () {
            return url;
        };
        int get_cant_dot (){
            return cant_dot ;
        };
        int get_cant_slash (){
            return cant_slash ;
        };
        int get_cant_num (){
            return cant_num ;
        };
        int get_cant_hyphen (){
            return cant_hyphen ;
        };
        int get_cant_underscore(){
            return cant_underscore ;
        };
        int get_cant_ampersand (){
            return cant_ampersand ;
        };
        int get_cant_https (){
            return cant_https ;
        };
        int get_cant_at  (){
            return cant_at  ;
        };
        int get_cant_percent  (){
            return cant_percent  ;
        };

        //SETTERS
        void set_url (string url) {
            this-> url = url;
        };
        void set_cant_dot (int cant_dot){
            this-> cant_dot = cant_dot;
        };
        void set_cant_slash (int cant_slash){
            this-> cant_slash = cant_slash;
        };
        void set_cant_num (int cant_num){
            this-> cant_num = cant_num;
        };
        void set_cant_hyphen (int cant_hyphen){
            this-> cant_hyphen = cant_hyphen;
        };
        void set_cant_underscore(int cant_underscore){
            this-> cant_underscore = cant_underscore;
        };
        void set_cant_ampersand (int cant_ampersand){
            this-> cant_ampersand = cant_ampersand;
        };
        void set_cant_https (int cant_https){
            this-> cant_https = cant_https;
        };
        void set_cant_at (int cant_at){
            this-> cant_at = cant_at;
        };
        void set_cant_percent (int cant_percent){
            this-> cant_percent = cant_percent;
        }; 

        //METODO
        int get_length() {
            return url.length();
        };
        void contarCaracteres(); 

        // Método para contar
        void contarCaracteres() {
            for (char c : url) {
                if (c == '.') {cant_dot++;}
                else if (c == '-') {cant_hyphen++;}
                else if (c == '/') {cant_slash++;}
                else if (isdigit(c)) {cant_num++;}      // (Hecho por claude no hemos visto listas en c++)
                else if (c == '&') {cant_ampersand++;}
                else if (c == '_') {cant_underscore++;}
                else if (c == '@') {cant_at++;}
                else if (c == '%') {cant_percent++;}
            }
        // Busca la subcadena "https" (Hecho por claude no hemos visto listas en c++)
            if (url.find("https") != string::npos) {
            cant_https = 1;  // booleano: está o no está
            }       
        }
};

//FUNCIONES PARA ACCEDER
string get_url(caracteristicas_URL &link) {     //Para accerder al puntero, & llama al atributo en la dirección de memoria
    return link.get_url();
};
int get_cant_dot(caracteristicas_URL &link) {
    return link.get_cant_dot();
};
int get_cant_slash(caracteristicas_URL &link) {
    return link.get_cant_slash();
};
int get_cant_num(caracteristicas_URL &link) {
    return link.get_cant_num();
};
int get_cant_hyphen(caracteristicas_URL &link) {
    return link.get_cant_hyphen();
};
int get_cant_underscore(caracteristicas_URL &link) {
    return link.get_cant_underscore();
};
int get_cant_ampersand(caracteristicas_URL &link) {
    return link.get_cant_ampersand();
};
int get_cant_https(caracteristicas_URL &link) {
    return link.get_cant_https();
};
int get_cant_at(caracteristicas_URL &link) {
    return link.get_cant_at();
};
int get_cant_percent(caracteristicas_URL &link) {
    return link.get_cant_percent();
};

int main() {
    return 0;
};

        
