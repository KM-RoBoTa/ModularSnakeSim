// // #include <GL/freeglut.h>
#include "constantesGL.h"
//#include "../Caja/simCaja.h"
#include <iostream>
#include <sstream>
#include <math.h>



using namespace std;;

void* pt2simGL;
dReal * rta=new dReal(4);


//Constructor
simGL::simGL(int argc, char ** argv)
{
  //parametros de inicio de GLUT
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE |  GLUT_DEPTH);
  glutInitWindowSize(ANCHO, ALTO);
  glutInitWindowPosition(POSICION_HOR, POSICION_VER);
  glutCreateWindow("retoJuanse");
  glClearColor(ROJO_CLEAR, VERDE_CLEAR, AZUL_CLEAR, ALFA_CLEAR);
  
  //Para ocultar objetos detrás de otros
  glEnable(GL_DEPTH_TEST);
  
  //Para suavizar las líneas
  glEnable(GL_LINE_SMOOTH);
  
  //Para suavizar los polígonos
  glEnable(GL_POLYGON_SMOOTH);
  
  //Agregamos neblina
  GLfloat colorNeblina[4] = {0.1, 0.1, 0.1, 1.0};
  //glEnable(GL_FOG);
  glFogi(GL_FOG_MODE, GL_LINEAR);
  glFogf (GL_FOG_DENSITY, 20);
  glFogf (GL_FOG_START, 3);
  glFogf (GL_FOG_END, 50);
  glFogf(GL_FOG_COORD_SRC,GL_FRAGMENT_DEPTH);
  glFogfv (GL_FOG_COLOR, colorNeblina);
  glEnable(GL_FOG);
  
  //Asignaciones de Callback;
  pt2simGL=(void*) this;
  glutDisplayFunc(simGL::Auxiliar_para_Dibujar);
  glutReshapeFunc(simGL::Auxiliar_para_Redimensionar);
  //glutIdleFunc(simGL::Auxiliar_para_Dibujar);
  
  //Para teclado
  glutIgnoreKeyRepeat(1);
  glutKeyboardFunc(simGL::Auxiliar_para_TeclaAscii);
  glutKeyboardUpFunc(simGL::Auxiliar_para_TeclaAsciiSoltada);
  glutSpecialFunc(simGL::Auxiliar_para_TeclaEspecialPresionada);
  glutSpecialUpFunc(simGL::Auxiliar_para_TeclaEspecialSoltada);
  
  //Luces y sombras
  glEnable(GL_NORMALIZE);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  //Modelo de sombreado
  glShadeModel(GL_SMOOTH);
  
  // Creamos los colores de las luces
  GLfloat luzAmbiente[] = { 0.5f, 0.5f, 0.5f, 1.0f };
  GLfloat luzDifusa[] = { 0.8f, 0.8f, 0.8f, 0.8f };
  GLfloat luzEspecular[] = { 0.2f, 0.2f, 0.2f, 1.0f };
  GLfloat posicion[] = { 1.0f, 1.0f, 1.0f, 1.0f };

  // Assign created components to GL_LIGHT0
  glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa);
  glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular);
  glLightfv(GL_LIGHT0, GL_POSITION, posicion);
  
  //Color tracking, los colores son tomados como materiales.
  glColorMaterial ( GL_FRONT_AND_BACK,  GL_AMBIENT_AND_DIFFUSE ) ;
  glEnable ( GL_COLOR_MATERIAL ) ;
  
  //Para las sombras en el suelo
  GLfloat planoSuelo[4] = {0.0, 1.0, 0.0, 0.0};
  GLfloat Ppunto = planoSuelo[0]*posicion[0] + planoSuelo[1]*posicion[1] + planoSuelo[2]*posicion[2] +
	 planoSuelo[3]*posicion[3];
	MatrizSombras[0][0] = Ppunto - posicion[0]*planoSuelo[0];
	MatrizSombras[1][0] =     - posicion[0]*planoSuelo[1];
	MatrizSombras[2][0] =     - posicion[0]*planoSuelo[2];
	MatrizSombras[3][0] =     - posicion[0]*planoSuelo[3];

	MatrizSombras[0][1] =     - posicion[1]*planoSuelo[0];
	MatrizSombras[1][1] = Ppunto - posicion[1]*planoSuelo[1];
	MatrizSombras[2][1] =     - posicion[1]*planoSuelo[2];
	MatrizSombras[3][1] =     - posicion[1]*planoSuelo[3];

	MatrizSombras[0][2] =     - posicion[2]*planoSuelo[0];
	MatrizSombras[1][2] =     - posicion[2]*planoSuelo[1];
	MatrizSombras[2][2] = Ppunto - posicion[2]*planoSuelo[2];
	MatrizSombras[3][2] =     - posicion[2]*planoSuelo[3];

	MatrizSombras[0][3] =     - posicion[3]*planoSuelo[0];
	MatrizSombras[1][3] =     - posicion[3]*planoSuelo[1];
	MatrizSombras[2][3] =     - posicion[3]*planoSuelo[2];
	MatrizSombras[3][3] = Ppunto - posicion[3]*planoSuelo[3];
  
  
  //Iniciación de valores de los atributos
  this->cuadroActual=0;
  this->fps=new char(100);
  this->tiempoBase=0;
  
  this->centroX=0.28;
  this->centroY=-1.65;
  this->centroZ=0.27;
  this->camX=0;
  this->camY=0;
  this->camZ=0;
  this->theta=43.0;
  this->phi=39.8;
  this->incrementoTheta=0;
  this->incrementoPhi=0;
  this->movAdelante=0;
  this->movLateral=0;
  this->movVertical=0;
  this->tiempoAnterior=0;
  
  
  Archivo="CADS2.stl";
  // And have it read the given file with some example postprocessing
  // Usually - if speed is not the most important aspect for you - you'll
  // propably to request more postprocessing than we do in this example.
  this->scene = (this->importador).ReadFile( Archivo, aiProcess_CalcTangentSpace |aiProcess_Triangulate |aiProcess_JoinIdenticalVertices |aiProcess_SortByPType|aiProcess_OptimizeMeshes|aiProcess_OptimizeGraph |aiProcess_SortByPType|aiProcess_ImproveCacheLocality |	aiProcess_GenSmoothNormals );
  if( !scene)
  {
    cout<<"error: "<<(this->importador).GetErrorString()<<endl;; 
  }
}

//funcion de GL
void simGL::cicloGL()
{
  glutMainLoop();
}

//Setter
void simGL::setSerpiente(simSerpiente * nuevaSerpiente)
{
  this->serpiente=nuevaSerpiente;
}

void simGL::pintarPantalla(simSerpiente * pintarSerpiente)
{
	this->serpiente=pintarSerpiente;
	glutPostRedisplay();
}

void simGL::Auxiliar_para_Dibujar()
{
       // explicitly cast global variable <pt2Object> to a pointer to TClassB
       // warning: <pt2Object> MUST point to an appropriate object!
       simGL* mySelf = (simGL*) pt2simGL;

       // call member
       mySelf->Dibujar();
}

void simGL::Dibujar()
{
  
  
  //////////////////////ciclo glut//////////////////////////////////
  // Clear Color and Depth Buffers
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(0.0f,0.0f,0.0f,1.0f);
  
  calcularFPS();
  posicionCamara();

 //Ubicación de cámara 
  glLoadIdentity();  
  gluLookAt(this->camX, this->camY,  this->camZ,
	    this->centroX, this->centroY, this->centroZ,
	    0.0f, 1.0f,  0.0f);

  //Dibujar el plano y los ejes
  dibujarAmbiente();
  
  //dibujarSombras();
  
  dibujarObjetos();
  
  //Utilizando assimp
  
  glutSwapBuffers();
 
  
  
}

void simGL::Auxiliar_para_Redimensionar(int w, int h)
{
       // explicitly cast global variable <pt2Object> to a pointer to TClassB
       // warning: <pt2Object> MUST point to an appropriate object!
       simGL* mySelf = (simGL*) pt2simGL;

       // call member
       mySelf->Redimensionar(w,h);
}

void simGL::Redimensionar( int w, int h)
{
  // Prevent a divide by zero, when window is too short
        // (you cant make a window of zero width).
        if (h == 0)
                h = 1;

        float ratio =  w * 1.0 / h;

        // Use the Projection Matrix
        glMatrixMode(GL_PROJECTION);

        // Reset Matrix
        glLoadIdentity();

        // Set the viewport to be the entire window
        glViewport(0, 0, w, h);

        // Set the correct perspective.
        gluPerspective(45.0f, ratio, 0.1f, 5000.0f);

        // Get Back to the Modelview
        glMatrixMode(GL_MODELVIEW);

}
  
//Destructor
simGL::~simGL()
{
  
}

//Función para escribir texto
void simGL::escribirEnBitmap(void *font, char *string)
{  
   char *c;
   for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

//Funciones de dispositivos de entrada
void simGL::Auxiliar_para_TeclaAscii(unsigned char tecla, int xx, int yy)
{
       // explicitly cast global variable <pt2Object> to a pointer to TClassB
       // warning: <pt2Object> MUST point to an appropriate object!
       simGL* mySelf = (simGL*) pt2simGL;

       // call member
       mySelf->TeclaAscii(tecla,xx,yy);
}

void simGL::TeclaAscii(unsigned char tecla, int xx, int yy) 
{
        if (tecla == 27)
              exit(0);
	if (tecla == 'w'||tecla == 'W')
	{
	    if (glutGetModifiers()==GLUT_ACTIVE_SHIFT)
	    {
	      movVertical=0.1;
	    }
	    else
	    {
	      movAdelante=-0.1;
	    }
	}
	if (tecla == 's'||tecla == 'S')
	{
	    if (glutGetModifiers()==GLUT_ACTIVE_SHIFT)
	    {
	      movVertical=-0.05;
	    }
	    else
	    {
	      movAdelante=0.05;
	    }
	} 
	if (tecla == 'a'||tecla == 'A')
	{
	  movLateral=-0.05;
	}
	if (tecla == 'd'||tecla == 'D')
	{
	  movLateral=0.05;
	}
}

void simGL::Auxiliar_para_TeclaAsciiSoltada(unsigned char tecla, int xx, int yy)
{
       // explicitly cast global variable <pt2Object> to a pointer to TClassB
       // warning: <pt2Object> MUST point to an appropriate object!
       simGL* mySelf = (simGL*) pt2simGL;

       // call member
       mySelf->TeclaAsciiSoltada(tecla,xx,yy);
}

void simGL::TeclaAsciiSoltada(unsigned char tecla, int xx, int yy) 
{
        if (tecla == 'w'||tecla == 's'||tecla == 'W'||tecla == 'S')
	{
	  movAdelante=0;
	  movVertical=0;
	}
	if (tecla == 'a'||tecla == 'd'||tecla == 'A'||tecla == 'D')
	{
	  movLateral=0;
	} 
        cout<<"centro "<<centroX<<" "<<centroY<<" "<<centroZ<<" "<<" Theta "<<theta<<" Phi "<<phi<<endl;
} 

void simGL::Auxiliar_para_TeclaEspecialPresionada(int tecla, int xx, int yy)
{
       // explicitly cast global variable <pt2Object> to a pointer to TClassB
       // warning: <pt2Object> MUST point to an appropriate object!
       simGL* mySelf = (simGL*) pt2simGL;

       // call member
       mySelf->TeclaEspecialPresionada(tecla,xx,yy);
}

void simGL::TeclaEspecialPresionada(int tecla, int xx, int yy) 
{

       switch (tecla) {
             case GLUT_KEY_UP : 
	       this->incrementoTheta = 0.8; 
	       break;
             case GLUT_KEY_DOWN : 
	       this->incrementoTheta = -0.8; 
	       break;
	     case GLUT_KEY_LEFT : 
	       this->incrementoPhi = 0.8; 
	       break;
             case GLUT_KEY_RIGHT : 
	       this->incrementoPhi = -0.8; 
	       break;
       }
} 

void simGL::Auxiliar_para_TeclaEspecialSoltada(int tecla, int xx, int yy)
{
       // explicitly cast global variable <pt2Object> to a pointer to TClassB
       // warning: <pt2Object> MUST point to an appropriate object!
       simGL* mySelf = (simGL*) pt2simGL;

       // call member
       mySelf->TeclaEspecialSoltada(tecla,xx,yy);
}

void simGL::TeclaEspecialSoltada(int tecla, int xx, int yy) 
{ 	

        switch (tecla) {
             case GLUT_KEY_UP : case GLUT_KEY_DOWN :
	      this->incrementoTheta = 0;
	      break;
	     case GLUT_KEY_RIGHT : case GLUT_KEY_LEFT :
	      this->incrementoPhi = 0;
	      break;
        }
}

//Funciones Adicionales
void simGL::calcularFPS()
{
   /////////////CALCULO DE FPS
  this->cuadroActual++;
  int tiempoTranscurrido=glutGet(GLUT_ELAPSED_TIME);
  if (tiempoTranscurrido - this->tiempoBase > 1000) 
  {	  
    sprintf(this->fps, "fps: %f",this->cuadroActual*1000.0/(tiempoTranscurrido-this->tiempoBase) );
    this->tiempoBase = tiempoTranscurrido;
    this->cuadroActual = 0;
    cout<<this->fps<<endl;
  }
}

void simGL::posicionCamara()
{
  //calculo de posició de cámara (Coordenadas esféricas modificadas para que phi sea respecto a y y theta respecto a z)
  //Rotación de la cámara
  this->theta+=this->incrementoTheta;
  this->phi+=this->incrementoPhi;
  this->camX=this->centroX+sin(PI*this->theta/180)*sin(PI*this->phi/180);
  this->camY=this->centroY+cos(PI*this->theta/180);
  this->camZ=this->centroZ+sin(PI*this->theta/180)*cos(PI*this->phi/180);
  //Movimientos laterales y hacia adelante
  this->centroX+=this->movAdelante*sin(PI*phi/180)+this->movLateral*cos(PI*phi/180);
  this->centroZ+=this->movAdelante*cos(PI*phi/180)-this->movLateral*sin(PI*phi/180);
  this->camX+=this->movAdelante*sin(PI*phi/180)+this->movLateral*cos(PI*phi/180);
  this->camZ+=this->movAdelante*cos(PI*phi/180)-this->movLateral*sin(PI*phi/180);
  //Movimientos verticales
  this->centroY+=movVertical;
  this->camY+=movVertical;
}

void simGL::dibujarAmbiente()
{
  glColor3f(0.0f, 0.0f, 0.0f);
  glTranslatef(0.0,-2.0,0.0);
  glColor3f(0.9f, 0.9f, 0.9f);
  glLineWidth(2.9);
  glBegin(GL_QUADS);
	  glVertex3f(-200.0f, 0.0f, -200.0f);
	  glVertex3f(-200.0f, 0.0f,  200.0f);
	  glVertex3f( 200.0f, 0.0f,  200.0f);
	  glVertex3f( 200.0f, 0.0f, -200.0f);
  glEnd();
  //Eje X
  glBegin(GL_LINES);
                glColor3f(1.0f,0.0f,0.0f);
                glVertex3f(0.0f,0.0f,0.0f);
                glVertex3f(100.0f,0.0f,0.0f);
   glEnd();
   //Eje Y
   glBegin(GL_LINES);
                glColor3f(0.0f,1.0f,0.0f); 
                glVertex3f(0.0f,0.0f,0.0f); 
                glVertex3f(0.0f,100.0f,0.0f);
   glEnd();
   //Eje Z
   glBegin(GL_LINES);
                glColor3f(0.0f,0.0f,1.0f); 
                glVertex3f(0.0f,0.0f,0.0f); 
                glVertex3f(0.0f,0.0f,100.0f);
   glEnd();
  glLineWidth(1.0);
   glRasterPos3f(100.0,0.0,0.0);
   escribirEnBitmap(GLUT_BITMAP_HELVETICA_18,(char*)"X");
   glRasterPos3f(0.0,100.0,0.0);
   escribirEnBitmap(GLUT_BITMAP_HELVETICA_18,(char*)"Y");
   glRasterPos3f(0.0,1.0,100.0);
   escribirEnBitmap(GLUT_BITMAP_HELVETICA_18,(char*)"Z");
  
}

void simGL::dibujarObjetos()
{
  double matrizTransformacion[16];
  for (int i=0;i<serpiente->getTamano();i++)
  {
    if(i%2==0)
    {
      glColor3f(1.0,0.0,0.0);
    }
    else
    {
      glColor3f(0.0,0.0,1.0);
    }
    glPushMatrix();
	  //Tomamos la posición del módulo i
	  const dReal *realP1 = dBodyGetPosition(*(serpiente->getModuloPos(i)));
	  //Tomamos la rotación del módulo i
	  const dReal *realR1 = dBodyGetRotation(*(serpiente->getModuloPos(i)));      
	  //Construcción de la matriz de transformación completa
	  matrizTransformacion[0] = realR1[0];
	  matrizTransformacion[1] = realR1[4];
	  matrizTransformacion[2] = realR1[8];
	  matrizTransformacion[3] = 0;
	  matrizTransformacion[4] = realR1[1];
	  matrizTransformacion[5] = realR1[5];
	  matrizTransformacion[6] = realR1[9];
	  matrizTransformacion[7] = 0;
	  matrizTransformacion[8] = realR1[2];
	  matrizTransformacion[9] = realR1[6];
	  matrizTransformacion[10] = realR1[10];
	  matrizTransformacion[11] = 0;
	  matrizTransformacion[12] = realP1[0];
	  matrizTransformacion[13] = realP1[1];
	  matrizTransformacion[14] = realP1[2];
	  matrizTransformacion[15] = 1;
	  //implementación de la matriz de transformación.
	  glMultMatrixd(matrizTransformacion);
	  double auxiliar;
          //glutSolidCube(0.04);
	  recursive_render((this->scene)->mRootNode);
	  //Dibujo de la segunda parte del módulo					
    glPopMatrix();
  }

  // Create an instance of the Importer class

}

void simGL::dibujarSombras()
{
  glEnable(GL_STENCIL_TEST);
  glEnable(GL_BLEND);
  glDisable(GL_LIGHTING);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDisable(GL_DEPTH_TEST);
  glPushMatrix();
  glMultMatrixf(&MatrizSombras[0][0]);
  glStencilOp(GL_KEEP, GL_KEEP, GL_ZERO);
  double matrizTransformacion[16];
  for (int i=0;i<serpiente->getTamano();i++)
  {
    glColor4f(0.3f,0.3f,0.3f,0.3f);
    glPushMatrix();
	  //Tomamos la posición del módulo i
	  const dReal *realP1 = dBodyGetPosition(*(serpiente->getModuloPos(i)));
	  //Tomamos la rotación del módulo i
	  const dReal *realR1 = dBodyGetRotation(*(serpiente->getModuloPos(i)));      
	  //Construcción de la matriz de transformación completa
	  matrizTransformacion[0] = realR1[0];
	  matrizTransformacion[1] = realR1[4];
	  matrizTransformacion[2] = realR1[8];
	  matrizTransformacion[3] = 0;
	  matrizTransformacion[4] = realR1[1];
	  matrizTransformacion[5] = realR1[5];
	  matrizTransformacion[6] = realR1[9];
	  matrizTransformacion[7] = 0;
	  matrizTransformacion[8] = realR1[2];
	  matrizTransformacion[9] = realR1[6];
	  matrizTransformacion[10] = realR1[10];
	  matrizTransformacion[11] = 0;
	  matrizTransformacion[12] = realP1[0];
	  matrizTransformacion[13] = realP1[1];
	  matrizTransformacion[14] = realP1[2];
	  matrizTransformacion[15] = 1;
	  //implementación de la matriz de transformación.
	  glMultMatrixd(matrizTransformacion);
	  double auxiliar;
          glutSolidCube(0.04);

	  //Dibujo de la segunda parte del módulo					
    glPopMatrix();
  }
  glPopMatrix();
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  glDisable(GL_BLEND);
  glDisable(GL_STENCIL_TEST);
}

void simGL::recursive_render (const aiNode* nd)
{
	unsigned int i;
	unsigned int n = 0, t;
	/*aiMatrix4x4 m = nd->mTransformation;

	// update transform
	aiTransposeMatrix4(&m);
	glPushMatrix();
	glMultMatrixf((float*)&m);*/

	// draw all meshes assigned to this node
	for (; n < nd->mNumMeshes; ++n) {
		const aiMesh* mesh = (this->scene)->mMeshes[nd->mMeshes[n]];
		for (t = 0; t < mesh->mNumFaces; ++t) {
			const aiFace* face = &mesh->mFaces[t];
			
			glBegin(GL_TRIANGLES);

			for(i = 0; i < face->mNumIndices; i++) {
				int index = face->mIndices[i];
				if(*(&mesh->mVertices[index].x)<37)
				  glColor4f(0.10f,0.10f,0.10f,1.0f);
				else
				  glColor4f(0.90f,0.90f,0.90f,1.0f);
				if(mesh->mNormals != NULL) 
					glNormal3fv(&mesh->mNormals[index].x);
				glVertex3f(*(&mesh->mVertices[index].x)/1000,*(&mesh->mVertices[index].y)/1000,*(&mesh->mVertices[index].z)/1000);
			}

			glEnd();
// 			glBegin(GL_LINES);
// 
// 			for(i = 0; i < face->mNumIndices; i++) {
// 				int index = face->mIndices[i];
// 				glColor4f(0.0f,0.0f,0.0f,1.0f);
// 				glVertex3f(*(&mesh->mVertices[index].x)/1000,*(&mesh->mVertices[index].y)/1000,*(&mesh->mVertices[index].z)/1000);
// 			}
// 
// 			glEnd();
		}

	}

	// draw all children
	for (n = 0; n < nd->mNumChildren; ++n) {
		recursive_render(nd->mChildren[n]);
	}

	//glPopMatrix();
}