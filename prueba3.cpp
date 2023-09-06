#include <iterator> 
#include <iostream> 
#include <fstream> 
#include <sstream> 
#include <vector> 
#include <string> 
#include <math.h>  
#include <ctime> 

using namespace std;

void ReadMatrix(string nombre, vector <vector<int>> &matrix );
void WriteMatrix(string nombre, vector <vector<int>> &matrix,vector <vector<int>> &figuras);
void Encerrar(vector <vector<int>> &matrix,vector <vector<int>> &figuras );
double Angulo(int x1,int y1, int x2,int y2, int x3, int y3);
void Recta( vector <vector<int>> &figuras,int x1,int y1, int x2,int y2);	
void engrosar( vector <vector<int>> &figuras);	

int main(int argc, char *argv[]){
	if(argc==3){
		//Opciones programa
		cout<<"1 para engrosar linea\n";
		cout<<"0 para no engrosar linea\n";		
		int mm;
		cin>>mm;
			
		//Variables tiempo, iniciar reloj
		unsigned t0, t1;
		t0=clock();
		
		//Matriz .PGM , leer imagen 
		vector <vector<int>> matriz;		
		ReadMatrix( argv[1],matriz);
		//Matriz auxiliar
		vector <vector<int>> figuras( matriz.size() ,vector<int>(matriz[0].size(),0));
		
		//Calcular la envolvente
		Encerrar(matriz,figuras);	
		//Engrosar envolvente;
		if(mm==1){
			engrosar(figuras);
		}
			
		//Guardar nueva imagen 
		WriteMatrix( argv[2],matriz,figuras);	

		//Detener reloj, cuanto tiempo paso en segundos
		t1 = clock();		
		double time = (double(t1-t0)/CLOCKS_PER_SEC);
		cout << "Tiempo Transcurrido: " << time << endl;					
			
	}
	else{
		cout<<"-------Error, se requiere 2 parametros para ejecutar el programa-----------\n";			
		cout<<"-------Primero nombre imagen, Segundo nombre nueva imagen-----------\n";					
	}							
}

void Encerrar(vector <vector<int>> &matrix,vector <vector<int>> &figuras ){
	int fila,columna;
	int filant,columnant;
	int itemporal,jtemporal;
	double angulo=0,maxangulo=0;
	int continuar=1;
	int contador=0;
	
	//Encontrar pixel mas a la izquieda y mas arriba
	for(int i = 0; i < matrix.size(); i++) {
		for(int j = 0; j < matrix[0].size(); j++) {
			if(matrix[i][j]!=0){
				fila=i;
				columna=j;
				j=matrix[0].size();
				i=matrix.size();
			}
			
		}
	}
	//Punto anterior del primero punto encontrado es un punto a la derecha
	filant=fila;	
	columnant=columna+1;
	cout<<"Coordenadas de los puntos pertenecientes a la envolvente convexa\n";
	
	//Encontrar los vertices de la envolvente	
	while(continuar==1){
		//Actualizar punto anterior, punto actual
		if(contador>0){
			filant=fila;
			columnant=columna;
			fila=itemporal;
			columna=jtemporal;
			maxangulo=0;
		}
		//Calcular angulo entre punto anterior,actual, todos pixeles blancos
		for(int i = 0; i < matrix.size(); i++) {
			for(int j = 0; j < matrix[0].size(); j++) {
				if(matrix[i][j]!=0){
					angulo=Angulo( filant,columnant ,fila, columna, i,j);
					if(angulo>=maxangulo){
						maxangulo=angulo;
						itemporal=i;
						jtemporal=j;
					}
				}	
			}
		}
		//SI el nuevo vertice no se habia encontrado antes, unir con recta, marcar ya encontrado
		if(figuras[itemporal][jtemporal]==0){
			figuras[itemporal][jtemporal]=-1;	
			Recta(figuras,fila,columna,itemporal,jtemporal);	
		}
		//SI el nuevo vertice ya se habia encontrado, unir recta, terminamos
		else{
			Recta(figuras,fila,columna,itemporal,jtemporal);			
			continuar=0;
		}
		//Contador para numero de vertices encontrados
		contador=contador+1;
	}		
}


double Angulo(int x1,int y1, int x2,int y2, int x3, int y3){
	//Dos vectores 
	vector<int> A{x3-x2,y3-y2};
	vector<int> B{x1-x2,y1-y2};	
	
	//Producto punto,cruz,magnitud
	double cruz = A[0]*B[1] - A[1]*B[0];
	double punto = A[0]*B[0] + A[1]*B[1];
	double ma = sqrt(A[0]*A[0] + A[1]*A[1]);
	double mb = sqrt(B[0]*B[0] + B[1]*B[1]);

	//Regresar angulo
	if(cruz >= 0){
		if(punto >= 0){
			return asin(cruz/(ma*mb));
		}
		else{
			return M_PI - asin(cruz/(ma*mb));
		}	
	}
	else{
		if(punto < 0){
			return M_PI - asin(cruz/(ma*mb));
		}
		else{
			return 2*M_PI + asin(cruz/(ma*mb));
		}
	}
}

void Recta( vector <vector<int>> &figuras,int x1,int y1, int x2,int y2){
	//cout<<"Recta de "<<"["<<x1<<","<<y1<<"]"<<"  "<<"["<<x2<<","<<y2<<"]\n";
	
	//Si pixeles estan en la misma fila
	if(y1==y2){
		if(x1<x2){
			for (int i=x1;i<x2;i++){
				figuras[i][y1]=-1;
				cout<<"["<<i<<","<<y1<<"]\n";		
			}
		}
		else if(x2<x1){
			for (int i=x2;i<x1;i++){
				figuras[i][y1]=-1;
				cout<<"["<<i<<","<<y1<<"]\n";	
			}
		}		
	}
	//Si pixeles estan en la misma columna	
	else if(x1==x2){
		if(y1<y2){
			for (int i=y1;i<y2;i++){
				figuras[x1][i]=-1;
				cout<<"["<<x1<<","<<i<<"]\n";		
			}
		}
		else if(y2<y1){
			for (int i=y2;i<y1;i++){
				figuras[x1][i]=-1;
				cout<<"["<<x1<<","<<i<<"]\n";	
			}
		}		
	}
	// SI no estan en la misma fila ni columna
	else if(x1!=x2 && y1!=y2 ){
		//Si deferencia entre cordenadas x's mayor a la de y's
		if(abs(x2-x1)>abs(y2-y1)){
			int yint;
			double yi;
			if(x1<x2){
				for (int i=x1;i<x2;i++){
					yi=(double)(y1-y2);
					yi=(yi/(x1-x2))*(i-x1);
					yi=yi+y1;
					yi=round(yi);
					yint=(int)yi;
					figuras[i][yint]=-1;
					cout<<"["<<i<<","<<yint<<"]\n";
				}


			}
			else if(x2<x1){
				for (int i=x2;i<x1;i++){
					yi=(double)(y1-y2);
					yi=(yi/(x1-x2))*(i-x1);
					yi=yi+y1;
					yi=round(yi);
					yint=(int)yi;
					figuras[i][yint]=-1;
					cout<<"["<<i<<","<<yint<<"]\n";				
				}
			}	
		}
		//Si deferencia entre cordenadas y's mayor a la de x's		
		else if(abs(x2-x1)<=abs(y2-y1)){
			int yint;
			double yi;
			if(y1<y2){

				for (int i=y1;i<y2;i++){
					yi=(double)(x1-x2);
					yi=(yi/(y1-y2))*(i-y1);
					yi=yi+x1;
					yi=round(yi);
					yint=(int)yi;
					figuras[yint][i]=-1;
					cout<<"["<<i<<","<<yint<<"]\n";
				}


			}
			else if(y2<y1){
				for (int i=y2;i<y1;i++){
					yi=(double)(x1-x2);
					yi=(yi/(y1-y2))*(i-y1);
					yi=yi+x1;
					yi=round(yi);
					yint=(int)yi;
					figuras[yint][i]=-1;
					cout<<"["<<i<<","<<yint<<"]\n";				
				}
			}		
		
		
		}
	}	
	
}

void ReadMatrix(string nombre, vector <vector<int>> &matrix) {
	//Accdere archivo 
	ifstream myfile(nombre);
	int m,n;
	//Si el archivo existe
	if(myfile.is_open()){	
		//Leer primera fila
		string line, word;
		getline (myfile, line);
		if(line.compare("P2")!=0){
			cout << "Error archivo incorrecto\n";
		}
		//Leer segunda fila
		getline (myfile, line);
		stringstream s(line);
		getline (s, word, ' ');		
		//Saltar comentario si existe		
		if(word=="#"){
			getline (myfile, line);						
		}
		//Leer ancho y alto
		stringstream ss(line);
		getline (ss, word, ' ');		
		n=stoi(word);
		getline (ss, word, ' ');
		m=stoi(word);
		//Leer valor maximo
		getline (myfile, line);
		//Modificar tamaño matriz		
		matrix.assign(m,vector<int>(n,0));
		int I=0,J=0;			
		//Leer las otras filas del archivo (Datos)
		for(int i = 0; i < m*n; i++) {
			getline (myfile, line);
			stringstream ss(line);
			getline (ss, word,' ');
			if(stoi(word)>=255-stoi(word)){
				matrix[I][J]=255;
			}
			else
				matrix[I][J]=0;	
			J=J+1;
			if(J==n){
				J=0;
				I=I+1;
			}	
		}
		//Cerrar archivo
		myfile.close();
		
	}
	
	//Si el archivo no existe		
	else{
		cout << "Error archivo dado no existe\n";
		
	}	
	
}

void engrosar(vector <vector<int>> &matrix){
	//Visitar 8 vecinos, si existen asignarles -2 para no visitarlos, solo visitar los -1,
	for(int i = 0; i < matrix.size(); i++) {
		for(int j = 0; j < matrix[0].size(); j++) {

			if(matrix[i][j]==-1){
				if( i-1>=0 && j-1>=0 ){
					matrix[i-1][j-1]=-2;
				}
				
				if( i-1>=0 ){
					matrix[i-1][j]=-2;
				}	
				
				if( i-1>=0 && j+1<=matrix[0].size()-1 ){
					matrix[i-1][j+1]=-2;	
				}
				
				if( j-1>=0 ){
					matrix[i][j-1]=-2;	
				}
				
				if(  j+1<=matrix[0].size()-1 ){
					matrix[i][j+1]=-2;	
				}
				
				if( i+1<=matrix.size()-1 && j-1>=0 ){
					matrix[i+1][j-1]=-2;	
				}
				
				if( i+1<=matrix.size()-1){
					matrix[i+1][j]=-2;	
				}
				
				if( i+1<=matrix.size()-1 && j+1<=matrix[0].size()-1 ){
					matrix[i+1][j+1]=-2;		
				}
			}	
		}
	}			
}

void WriteMatrix(string nombre, vector <vector<int>> &matrix,vector <vector<int>> &figuras){
	//Escribir nueva imagen .PGM
	ofstream file;	
	int m =matrix.size();
	int n =matrix[0].size();
	int I=0,J=0;
	
	//Escribir encabezados		
	file.open(nombre);
	file << "P2\n";
	file << n <<" "<<m<<"\n" ;
	file << "255\n";
	
	//Escribir datos
	for(int i = 0; i < m*n; i++) {
		if(matrix[I][J]==0 && figuras[I][J]==0){
			file << "0\n";
		}
		else if(matrix[I][J]==0 && figuras[I][J]!=0){
			file << "150\n";
		}
		else if (matrix[I][J]!=0 && figuras[I][J]!=0){
			file <<"150\n" ;	
		}		
		else if (matrix[I][J]!=0 && figuras[I][J]==0){
			file <<"255\n" ;	
		}		
		J=J+1;
		if(J==n){
			J=0;
			I=I+1;
		}	
	}	
	file.close();

}





