#include <string>
#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include <vector>
#include <regex>



//En un futuro podemos usar la libreria de nltk
using namespace std;
class Vocabulario {
  protected:
    vector<string> palabrasReservadas = {"a","able","about","across","after","all","almost","also","am","among","an","and","any","are","as","at","be","because","been","but","by","can","cannot","could","dear","did","do","does","either","else","ever","every","for","from","get","got","had","has","have","he","her","hers","him","his","how","however","i","if","in","into","is","it","its","just","least","let","like","likely","may","me","might","most","must","my","neither","no","nor","not","of","off","often","on","only","or","other","our","own","rather","said","say","says","she","should","since","so","some","than","that","the","their","them","then","there","these","they","this","tis","to","too","twas","us","wants","was","we","were","what","when","where","which","while","who","whom","why","will","with","would","yet","you","your", "www"};
    set<string> diccionario; //contendrá todas las palabras
    map<string,int> palabrasPositivas;
    map<string,int> palabrasNegativas;
    map<string,int> palabrasNeutras;
    int tamVocabulario;
    int nPalabrasPositivas;
    int nPalabrasNegativas;
    int nPalabrasNeutras;
  public:
    Vocabulario(string nFichero);
    string tratarLinea(string _linea);
    void guardarDiccionario(string _nFichero = "vocabulario.txt");
    int noticiaEs(string _linea);
};

Vocabulario::Vocabulario(string _nFichero) {
  nPalabrasPositivas = 0;
  nPalabrasNegativas = 0;
  nPalabrasNeutras = 0;
  ifstream myfile(_nFichero);
  if(!myfile.is_open()) {
    cout << "No se pudo abrir el fichero: " << _nFichero;
    exit(0);
  }
  ofstream corpusPositivo("corpusP.txt");
  ofstream corpusNegativo("corpusN.txt");
  ofstream corpusNeutro("corpusT.txt");
  stringstream ss;
  string linea, palabra;
  string resultado;
  int maxLineas = 2500, tipoNoticia;
  while (!myfile.eof() && maxLineas > 0) {
    
    getline(myfile, linea);
    resultado = tratarLinea(linea);
    //resultado = tratarLinea("The new company  DiaPol s.r.l.  would manufacture tools tools meant algo.com for glass and stone pre-processing .,positive");
    maxLineas--;

    //comprobamos si la noticia es positiva, negativa o netura y la guardamos en el corpus correspondiente;
    tipoNoticia = noticiaEs(resultado);

    ss = stringstream(resultado);
    resultado = "";
    while (ss >> palabra) {
      if (palabra == "negative" || palabra == "positive" || palabra == "neutral") {
        continue;
      }
      resultado += palabra;
      resultado += " ";
    }
    ss = stringstream(resultado);
    if ( tipoNoticia == 0) { //positive
      corpusPositivo << resultado << endl;
      
      while(ss >> palabra) {
        
        if( palabrasPositivas.find(palabra) == palabrasPositivas.end() ) { //si no encuentra la palabra la añade
          
          palabrasPositivas.insert(pair <string,int> (palabra, 1));
        } else {
          
          palabrasPositivas.find(palabra)->second++;
        }
        nPalabrasPositivas++;
      }
    }
    if (tipoNoticia == 1) { //negative
      corpusNegativo << resultado << endl;
      while(ss >> palabra) {
        
        if( palabrasNegativas.find(palabra) == palabrasNegativas.end() ) { //si no encuentra la palabra la añade
          
          palabrasNegativas.insert(pair <string,int> (palabra, 1));
        } else {
          
          palabrasNegativas.find(palabra)->second++;
        }
        nPalabrasNegativas++;
      }
    }
    if (tipoNoticia == 2) { //neutral
      corpusNeutro << resultado << endl;
      while(ss >> palabra) {
        
        if( palabrasNeutras.find(palabra) == palabrasNeutras.end() ) { //si no encuentra la palabra la añade
          
          palabrasNeutras.insert(pair <string,int> (palabra, 1));
        } else {
          
          palabrasNeutras.find(palabra)->second++;
        }
        nPalabrasNeutras++;
      }
    }
  }
  
  //tratarLinea("algo.com kili+an gon(zal/ez S.r.l");
  //tratarLinea("The new company  DiaPol s.r.l.  would manufacture tools meant algo.com for glass and stone pre-processing .,neutral");

  cout << "\nNumero de palabras positivas: " << palabrasPositivas.size() << endl;
  /*for (auto val : palabrasPositivas) {
    cout << val.first << " -> " << val.second;
  }*/
  
  myfile.close();
  
  tamVocabulario = diccionario.size();

  guardarDiccionario();

  corpusNegativo.close();
  corpusPositivo.close();
  corpusNeutro.close();
  
  // Toca realizar los modelos
  ofstream modeloPositivo("modelo_lenguaje_P.txt");
  ofstream modeloNegativo("modelo_lenguaje_N.txt");
  ofstream modeloNeutro("modelo_lenguaje_T.txt");
}

void Vocabulario::guardarDiccionario(string _nFichero) {
  ofstream ficheroSalida(_nFichero);
  ficheroSalida << "Numero de palabras: " << diccionario.size() << endl;
  for(auto val : diccionario) {
    ficheroSalida << val << endl;
  }
}

string Vocabulario::tratarLinea(string _linea) {
  regex regEliminarURL("[a-zA-Z]+([.][a-zA-Z]+)+[.]?");
  regex regExpresion("[a-zA-Z]+");
  smatch matches;
  string aux;
  

  // pasamos todo a minuscula
  transform(_linea.begin(), _linea.end(), _linea.begin(), ::tolower);
  stringstream ss(_linea);
  //cout << "\nLinea con URL:" << _linea;
  _linea = "";
  //eliminar URL
  while(ss >> aux) {
    if(regex_match(aux, regEliminarURL)) {
      
      continue;
    }
      
    _linea += aux + " ";
  }
  //cout << "\nLinea sin URL:" << _linea;
  ss = stringstream(_linea);
  _linea = "";
  //Juntar palabras separadas por caracteres especiales
  while (ss >> aux) {
    for (auto ch : aux) {
      if(isalpha(ch) || ch == ',' || ch == '.' || ch == '\'') {
        _linea += ch;
      }
    }
    _linea += " ";
  }
  //cout << "\nLinea sin Especiales:" << _linea;
  string resultado = "";
  while (regex_search(_linea, matches, regExpresion)) {
    aux = matches.str(0);
    
    if ( std::find(palabrasReservadas.begin(), palabrasReservadas.end(), aux) == palabrasReservadas.end() ) {
      //las palabras neutral positive y negative no deben ser añadidas al vovabulario pero se deben de tener encuenta pal corpus
      if (aux == "negative" || aux == "positive" || aux == "neutral") {
        
      } else {
        diccionario.insert(aux);
      }     
      resultado += aux;
      resultado += " ";
    }           
    //quitamos de la linea match encontrado
    _linea = matches.suffix().str();
  }
  
  
  /*
  ss = stringstream(_linea);
  _linea = "";
  
  while (ss >> aux) {
    
      //Comprobamos que no sea una palabra reservada
    if ( std::find(palabrasReservadas.begin(), palabrasReservadas.end(), aux) == palabrasReservadas.end() ) {
      diccionario.insert(aux);
      _linea += aux;
      _linea += " ";
    }
  }*/
  //cout << "\nLinea Final:" << resultado;
  return resultado;
  //cin.get();
}

int Vocabulario::noticiaEs(string _linea) {
  string resultado, aux;
  stringstream ss(_linea);
  while (ss >> aux) {
    if (aux == "positive")
      return 0;
    if (aux == "negative")
      return 1;
    if (aux == "neutral")
      return 2;
  }
  return -1;
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
  cout <<  endl;
}