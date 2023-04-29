/*Fichero principal del programa que llamará a la clase Vocabulario y luego a la de clasificador
*/

#include <string>
#include <iostream>

#include "vocabulario.h"
#include "clasificador.h"


using namespace std;
int main(int argc, char* argv[]) {
  
  string nFichero = "F75_train.csv";
  if (argc >= 2) {
    if (argv[1] == "-h") {
      cout << "\nPrueba: ./Vocabulario <nombrefichero>,\n o: ./Vocabulario, este buscara por defecto el fichero F75_train.csv";
      exit(0);
    } 
    nFichero = argv[1];
  }
  //Vocabulario voc(nFichero);
  
  cout << "\nVocabulario implementado, procedemos a clasificar";

  cout << "\n¿Quiere introducir el nombre del fichero de prueba?, sino se buscará el fichero <F75_test.csv>(0 -> no, 1 -> si)";
  string opcion;
  do{
    cin >> opcion;
  } while (opcion != "1" && opcion != "0" );
  
  if (opcion == "1") {
    cout << "\nIntroduzca el nombre del fichero: ";
    cin >> nFichero;
    } else {
    nFichero = "F75_test.csv";
  }
  
  //Clasificador clasificador(voc, nFichero);

  //Ahora comparamos los resultados del clasificador con los valores correctos
  //primero con  F75_train.csv
  vector<string> resultadosCorrectos;
  ifstream ficheroResultados("F75_train.csv");
  regex resultadoExpresion("[a-zA-Z]+$");
  smatch matches;
  string linea;
  string palabra;
  int aciertos = 0;
  double probabilidad;
  while(!ficheroResultados.eof()) {
    getline(ficheroResultados, linea);
    stringstream ss(linea);
    //cojo la ultima palabra de la linea y guardo como fue clasificada
    while(ss >> palabra) {} 
    if(palabra.find("positive") != string::npos) {
      resultadosCorrectos.push_back("P");
    } else if (palabra.find("negative") != string::npos) {
      resultadosCorrectos.push_back("N");
    } else if (palabra.find("neutral") != string::npos) {
      resultadosCorrectos.push_back("T");
    }
  }
  cout << "\nprueba con train";
  cin.get();
  
  Vocabulario voc1("F75_train.csv");
  //Le pasamos al clasificador el fichero sin la clase
  Clasificador clasificador1(voc1, "F75_train_noClase.csv");
  
  //comparamos
  cout << "\nResultados size: " << resultadosCorrectos.size();
  for(int i = 0; i < resultadosCorrectos.size(); i++) {
    if (resultadosCorrectos[i] == clasificador1.resultadosSupuestos[i]) {
      aciertos++;
      cout << "\nacerté";
    }
  }
  probabilidad = aciertos;
  probabilidad = (double) probabilidad / resultadosCorrectos.size();
  probabilidad = probabilidad * 100;
  cout << "\nProbabilidad: " << probabilidad << " Numero de aciertos" << aciertos;
  
  cout <<  endl;
}