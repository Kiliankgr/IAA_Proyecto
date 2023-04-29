#ifndef CLASIFICADOR_H
#define CLASIFICADOR_H


#include "vocabulario.h"

using namespace std;
class Clasificador{

private:
  /* data */
  Vocabulario voc;
  
public:
  vector <string> resultadosSupuestos;
  vector<string> noticiasAEvaluar;
  Clasificador(Vocabulario _vocabulario, string _nFichero);
  double getTotalProbabilidad(string _noticia, Vocabulario _voc ,int _modelo); //calcula la probabilidad de una frase en el modelo especifico 0 -> pos, 1 -> neg, 2 -> neutro
  double getProbabilidadPalabra(string _palabra, Vocabulario _voc, int modelo);
  
};

Clasificador::Clasificador(Vocabulario _vocabulario, string _nFichero) {
  voc = _vocabulario;
  ifstream ficheroTest(_nFichero);
  if(!ficheroTest.is_open()) {
    cerr << "\nNo se pudo abrir el fichero: " << _nFichero;
    return;
  }
  string linea;
  vector<string> noticias;
  //Leemos las noticias y las tratamos antes de aplicar el modelo de prediccion
  int a = 0;
  while(!ficheroTest.eof()) {
    //cout << "\nLeemos linea: "<< a;
    
    getline(ficheroTest, linea);
    if(linea == "") {
      cerr << "\n Se leyó una linea vacía, dejó de leerse el fichero a partir de la linea: " << a;
      break;
    }
    linea = voc.tratarLinea(linea);
    //cout << "\nClasificador, linea resultante del tratamiento: " << linea;
    noticias.push_back(linea);
    a++;
  }
  cout << "\nNoticias a Evaluar: " << noticias.size();
  cin.get();
  //noticiasAEvaluar = procesarNoticias(noticias);
  /*for(auto n : noticias) {
    cout << "\n" << n ;
  }*/

  //Escribimos en el fichero clasificacion_alu0101222325.csv
  cout << "\nSe escribirán los resultados en el fichero <clasificacion_alu0101222325.csv> y <resumen_alu0101222325.csv>";
  ofstream ficheroClasificacion("clasificacion_alu0101222325.csv");
  ofstream ficheroResumen("resumen_alu0101222325.csv");
  string inicioNoticia; //contiene los primeros 10 caracteres de la noticia
  vector<double> probabilidadesRedondeadas;
  double probilidadMasCercaCero;
  vector<string> modelos = {"P", "N", "T"};
  int modeloMasProbable;
  for (auto noticia : noticias) {
    //cout << "\nLeemos noticia: " << noticia;
    inicioNoticia = noticia.substr(0,10);
    probabilidadesRedondeadas.clear();

    ficheroClasificacion << inicioNoticia;
    //asignamos por defecto la minProbabilidad a la positiva
    probilidadMasCercaCero = round(getTotalProbabilidad(noticia, _vocabulario, 0) *100) / 100;
    modeloMasProbable = 0;
    for(int i = 0; i <= 2; i++) {
      probabilidadesRedondeadas.push_back(round(getTotalProbabilidad(noticia, _vocabulario, i) *100) / 100);
      ficheroClasificacion << "," << probabilidadesRedondeadas[i];
      if (probabilidadesRedondeadas[i] > probilidadMasCercaCero) {
        probilidadMasCercaCero = probabilidadesRedondeadas[i];
        modeloMasProbable = i;
      }
    }

    ficheroClasificacion << "," << modelos[modeloMasProbable];
    ficheroClasificacion << endl;
    ficheroResumen << modelos[modeloMasProbable] << endl;
    resultadosSupuestos.push_back(modelos[modeloMasProbable]);
    //cin.get();
  }
  ficheroResumen.close();
  ficheroClasificacion.close();
}

/**
 * defecto modelo == 0;
 * 0 -> Positivas, 1 -> negativas, 2 -> neutras
 * Optiene la probabilidad aplicando el logaritmo neperiano según el vocabulario y el modelo 
*/
double Clasificador::getProbabilidadPalabra(string _palabra, Vocabulario _voc, int modelo) {
  double result;
  
  int numeroDeRepeticionesPalabra, numeroDePalabras, numeroDePalabrasModelo;
  numeroDePalabras = _voc.tamVocabulario;
  
   
  
  if(modelo == 1) {
    //si la palabra no se encuentra en nuestro corpus le asignamos la probabilidad de unk que es 0;
    if (_voc.palabrasNegativas.find(_palabra) != _voc.palabrasNegativas.end()) {
      numeroDeRepeticionesPalabra = _voc.palabrasNegativas.find(_palabra)->second;
    } else {
      numeroDeRepeticionesPalabra = 0;
    }
    
    numeroDePalabrasModelo = _voc.nPalabrasNegativas;
    result = (double) (numeroDeRepeticionesPalabra + 1) / (numeroDePalabras + numeroDePalabrasModelo + 1);
    result = log(result);
    return result;
  }
  if(modelo == 2) {
    if (_voc.palabrasNeutras.find(_palabra) != _voc.palabrasNeutras.end()) {
      numeroDeRepeticionesPalabra = _voc.palabrasNeutras.find(_palabra)->second;
    } else {
      numeroDeRepeticionesPalabra = 0;
    }
    
    numeroDePalabrasModelo = _voc.nPalabrasNeutras;
    result = (double) (numeroDeRepeticionesPalabra + 1) / (numeroDePalabras + numeroDePalabrasModelo + 1);
    result = log(result);
    return result;
  }
  // positivo

  if (_voc.palabrasPositivas.find(_palabra) != _voc.palabrasPositivas.end()) {
    numeroDeRepeticionesPalabra = _voc.palabrasPositivas.find(_palabra)->second;
  } else {
    numeroDeRepeticionesPalabra = 0;
  }
  
  numeroDePalabrasModelo = _voc.nPalabrasPositivas;
  result = (double) (numeroDeRepeticionesPalabra + 1) / (numeroDePalabras + numeroDePalabrasModelo + 1);
  result = log(result);
  return result;


  
}

/**
 * Modelo defecto = 0
 * 0 -> Positivas, 1 -> negativas, 2 -> neutras
 * Optiene la probrabilidad de que una noticia pertenezca al modelo
*/
double Clasificador::getTotalProbabilidad(string _noticia, Vocabulario _voc, int modelo) {
  double result = 0;
  string palabra;
  stringstream ss(_noticia);
  
  if(modelo == 1) {
    while (ss >> palabra) {
      result += getProbabilidadPalabra(palabra, _voc, modelo);
    }
    //por ultimo se le suma la probabilidad de que sea una noticia positiva
    
    result += log((double)(_voc.noticiasNegativas.size() + 1) / (_voc.noticiasPositivas.size() + _voc.noticiasNegativas.size() + _voc.noticiasNeutras.size() + 1));
    return result;
  }

  if(modelo == 2) {
    while (ss >> palabra) {
      result += getProbabilidadPalabra(palabra, _voc, modelo);
    }
    //por ultimo se le suma la probabilidad de que sea una noticia positiva
    
    result += log((double)(_voc.noticiasNeutras.size() + 1) / (_voc.noticiasPositivas.size() + _voc.noticiasNegativas.size() + _voc.noticiasNeutras.size() + 1));
    return result;
  }
  
  
  
  while (ss >> palabra) {
    result += getProbabilidadPalabra(palabra, _voc, modelo);
  }
  //por ultimo se le suma la probabilidad de que sea una noticia positiva
  
  result += log((double)(_voc.noticiasPositivas.size() + 1) / (_voc.noticiasPositivas.size() + _voc.noticiasNegativas.size() + _voc.noticiasNeutras.size() + 1));

  return result;
}





#endif