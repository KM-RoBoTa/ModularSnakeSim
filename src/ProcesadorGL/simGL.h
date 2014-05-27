 
 
 #include <assimp/Importer.hpp> // C++ importer interface
#include <assimp/scene.h> // Output data structure
#include <assimp/postprocess.h> // Post processing flags
#include <assimp/cimport.h>
#include <assimp/mesh.h>
#include <string>

class simGL
{
  simSerpiente * serpiente;
  
  //Valores necesarios para simular
  int cuadroActual;
  int tiempoBase;
  char* fps;
  
  //para la camara
  float centroX;
  float centroY;
  float centroZ;
  float camX;
  float camY;
  float camZ;
  float theta;
  float incrementoTheta;
  float phi;
  float incrementoPhi;
  float movAdelante;
  float movLateral;
  float movVertical;
  
  void calcularFPS();
  void posicionCamara();
  void dibujarAmbiente();
  void dibujarObjetos();
  void dibujarSombras();
  
  int tiempoAnterior;
  
  //Para sombras
  
  GLfloat MatrizSombras[4][4];
  
  //Para Assimo
  Assimp::Importer importador;
  string Archivo;
  const aiScene* scene;
  void recursive_render (const struct aiNode* nd);
  
  
public:
  //Constructor
  simGL(int argc, char ** argv);
    
  //funcion de GL
  void cicloGL();
  
  //funciones de callback
  void Dibujar();
  void Redimensionar( int w, int h);
  void TeclaAscii(unsigned char tecla, int xx, int yy);
  void TeclaAsciiSoltada(unsigned char tecla, int xx, int yy);
  void TeclaEspecialPresionada(int tecla, int xx, int yy);
  void TeclaEspecialSoltada(int tecla, int xx, int yy);
  void pintarPantalla(simSerpiente * pintarSerpiente);
  
  //Imprimir Texto
  void escribirEnBitmap(void *font, char *string);
  
  //Setters
  void setSerpiente(simSerpiente * nuevaSerpiente);
  void setSimulador(simODE * sim);
  //Getters
  simODE & getSimulador();
  
  //Destructor
  ~simGL();
  
  //Auxiliarles para el callback
  static void Auxiliar_para_Dibujar();
  static void Auxiliar_para_Redimensionar( int w, int h);
  static void Auxiliar_para_TeclaEspecialSoltada(int tecla, int xx, int yy);
  static void Auxiliar_para_TeclaEspecialPresionada(int tecla, int xx, int yy);
  static void Auxiliar_para_TeclaAscii(unsigned char tecla, int xx, int yy);
  static void Auxiliar_para_TeclaAsciiSoltada(unsigned char tecla, int xx, int yy);
  
  //Funciones para quaternion
  dReal* transformAxisAngle(const dReal * quat);
};

#include "simGL.cpp"