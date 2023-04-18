#include <string>
#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <regex>



//En un futuro podemos usar la libreria de nltk
using namespace std;
class Vocabulario {
  protected:
    vector<string> palabrasReservadas = {"a","able","about","across","after","all","almost","also","am","among","an","and","any","are","as","at","be","because","been","but","by","can","cannot","could","dear","did","do","does","either","else","ever","every","for","from","get","got","had","has","have","he","her","hers","him","his","how","however","i","if","in","into","is","it","its","just","least","let","like","likely","may","me","might","most","must","my","neither","no","nor","not","of","off","often","on","only","or","other","our","own","rather","said","say","says","she","should","since","so","some","than","that","the","their","them","then","there","these","they","this","tis","to","too","twas","us","wants","was","we","were","what","when","where","which","while","who","whom","why","will","with","would","yet","you","your", "www"};
    set<string> diccionario; //contendrá todas las palabras
  public:
    Vocabulario(string nFichero);
    void tratarLinea(string _linea);
    void guardarDiccionario(string _nFichero = "vocabulario.txt");
};

Vocabulario::Vocabulario(string _nFichero) {
  ifstream myfile(_nFichero);
  if(!myfile.is_open()) {
    cout << "No se pudo abrir el fichero: " << _nFichero;
    exit(0);
  }
  
  string linea;
  int maxLineas = 2500;
  while (!myfile.eof() && maxLineas > 0) {
    getline(myfile, linea);
    tratarLinea(linea);
    maxLineas--;
  }
  myfile.close();
  guardarDiccionario();
}

void Vocabulario::guardarDiccionario(string _nFichero) {
  ofstream ficheroSalida(_nFichero);
  ficheroSalida << "Numero de palabras: " << diccionario.size() << endl;
  for(auto val : diccionario) {
    ficheroSalida << val << endl;
  }
}

void Vocabulario::tratarLinea(string _linea) {
  regex regExpresion("[a-zA-Z]+");
  smatch matches;
  string aux;
  regex_search(_linea, matches, regExpresion);
  
  while (regex_search(_linea, matches, regExpresion)) {
    aux = matches.str(0);
    
    //la pasamos a minuscula
    transform(aux.begin(), aux.end(), aux.begin(), ::tolower);
    //eliminamos espacios en  blanco
    
    //aux.erase(remove_if(aux.begin(), aux.end(), ::isspace), aux.end());
    //Comprobamos que no sea una palabra reservada
    if ( std::find(palabrasReservadas.begin(), palabrasReservadas.end(), aux) == palabrasReservadas.end() )
      diccionario.insert(aux);
     
    //quitamos de la linea match encontrado
    _linea = matches.suffix().str();
    
  }
 
  //cin.get();
}

int main(int argc, char* argv[]) {
  string nFichero = "F75_train.csv";
  if (argc >= 2) {
    if (argv[1] == "-h") {
      cout << "\nPrueba: ./Vocabulario <nombrefichero>,\n o: ./Vocabulario, este buscara por defecto el fichero F75_train.csv";
      exit(0);
    } 
    nFichero = argv[1];
  }
  Vocabulario voc(nFichero);
}