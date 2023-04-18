#include <string>
#include <iostream>
#include <fstream>
#include <set>


using namespace std;
int main(int argc, char* argv[]) {
  if (argc < 2) {
    cout << "Faltan argumentos, Prueba: ./Vocabulario <nombrefichero>";
    exit(1);
  }
  ifstream myfile(argv[1]);
  if(myfile.is_open()) {
    
  }else {
    cout << "No se pudo abrir el fichero: " << argv[1];
  }
}