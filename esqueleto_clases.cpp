#include <iostream>
#include <string>
using namespace std;

// CLASE
class caracteristicas_URL {
    private:
        string url;
        int cant_dot;
        int cant_slash;
        int cant_num;
        int cant_hyphen;
    
    public:
        //CONSTRUCTOR
        caracteristicas_URL(string url, int cant_dot, int cant_slash, int cant_num, int cant_hyphen) {
            this-> url = url;                       //this-> para asignar un puntero en un dirección de memoria
            this-> cant_dot = cant_dot ;
            this-> cant_slash = cant_slash ;
            this-> cant_num = cant_num ;
            this-> cant_hyphen = cant_hyphen ;      
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
        
        //METODO
        int get_length() {
            return url.length();
        };
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

int main() {
    return 0;
};
