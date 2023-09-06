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
void WriteMatrix(string nombre, vector <vector<int>> &matrix);
int Recorrdio(vector <vector<int>> &matrix,vector <vector<int>> &figuras);
void Recorrdio2(vector <vector<int>> &matrix,vector <vector<int>> &figuras,int i,int j,int imagen);

void perimetro(vector <vector<int>> &matrix);

int main(int argc, char *argv[]){
	if(argc==3){
		//Variables tiempo, iniciar reloj
		unsigned t0, t1;
		t0=clock();
		
		//Matriz .PGM , leer imagen 
		vector <vector<int>> matriz;		
		ReadMatrix( argv[1],matriz);
		//Matriz auxiliar
		vector <vector<int>> figuras( matriz.size() ,vector<int>(matriz[0].size(),0));
		
		//Numero conjuntos, separar conjuntos
		int contador;	
		contador=Recorrdio(matriz,figuras);
		cout<<"------TOTAL DE "<<contador<<" IMAGENES ------"<<endl;
		
 		//Perimetro de las figuras		
		perimetro(figuras);
		
		//Guardar nueva imagen 
		WriteMatrix( argv[2],figuras);	

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

int Recorrdio(vector <vector<int>> &matrix,vector <vector<int>> &figuras){
	int imagen=1;
	//Recorrer la matriz desde 0,0 
	for(int i = 0; i < matrix.size(); i++) {
		for(int j = 0; j < matrix[0].size(); j++) {
			//Si valor del pixel diferente de 0 y no lo eh visitado antes, iniciar algoritmo
			if(matrix[i][j]!=0 && figuras[i][j]==0){
				figuras[i][j]=imagen;
				Recorrdio2(matrix,figuras,i,j,imagen);
				imagen=imagen+1;
			}
		
		}
	}	
	return imagen-1;
}

void Recorrdio2(vector <vector<int>> &matrix,vector <vector<int>> &figuras,int i,int j,int imagen){
	//Visitar 8 vecinos, marcar los que ya visite para no volver a ellos,

	if( i-1>=0 && j-1>=0 ){
		if(matrix[i-1][j-1]!=0 && figuras[i-1][j-1]==0){
			figuras[i-1][j-1]=imagen;
			Recorrdio2(matrix,figuras,i-1,j-1,imagen);	
		}	
	}
	
	if( i-1>=0 ){
		if(matrix[i-1][j]!=0 && figuras[i-1][j]==0){
			figuras[i-1][j]=imagen;
			Recorrdio2(matrix,figuras,i-1,j,imagen);	
		}	
	}	
	
	if( i-1>=0 && j+1<=matrix[0].size()-1 ){
		if(matrix[i-1][j+1]!=0 && figuras[i-1][j+1]==0){
			figuras[i-1][j+1]=imagen;
			Recorrdio2(matrix,figuras,i-1,j+1,imagen);	
		}	
	}
	
	if( j-1>=0 ){
		if(matrix[i][j-1]!=0 && figuras[i][j-1]==0){
			figuras[i][j-1]=imagen;
			Recorrdio2(matrix,figuras,i,j-1,imagen);	
		}	
	}
	
	if(  j+1<=matrix[0].size()-1 ){
		if(matrix[i][j+1]!=0 && figuras[i][j+1]==0){
			figuras[i][j+1]=imagen;
			Recorrdio2(matrix,figuras,i,j+1,imagen);	
		}	
	}
	
	if( i+1<=matrix.size()-1 && j-1>=0 ){
		if(matrix[i+1][j-1]!=0 && figuras[i+1][j-1]==0){
			figuras[i+1][j-1]=imagen;
			Recorrdio2(matrix,figuras,i+1,j-1,imagen);	
		}	
	}
	
	if( i+1<=matrix.size()-1){
		if(matrix[i+1][j]!=0 && figuras[i+1][j]==0){
			figuras[i+1][j]=imagen;
			Recorrdio2(matrix,figuras,i+1,j,imagen);	
		}	
	}
	
	if( i+1<=matrix.size()-1 && j+1<=matrix[0].size()-1 ){
		if(matrix[i+1][j+1]!=0 && figuras[i+1][j+1]==0){
			figuras[i+1][j+1]=imagen;
			Recorrdio2(matrix,figuras,i+1,j+1,imagen);	
		}		
	}
			
}

void perimetro(vector <vector<int>> &matrix){
	//Visitar 8 vecinos, marcar los que ya visite para no volver a ellos,
	int negro;
	for(int i = 0; i < matrix.size(); i++) {
		for(int j = 0; j < matrix[0].size(); j++) {
			negro=0;
			if(matrix[i][j]!=0){
				if( i-1>=0 && j-1>=0 ){
					if(matrix[i-1][j-1]==0 ){
						negro=negro+1;		
					}	
				}
				
				if( i-1>=0 ){
					if(matrix[i-1][j]==0 ){
						negro=negro+1;								
					}	
				}	
				
				if( i-1>=0 && j+1<=matrix[0].size()-1 ){
					if(matrix[i-1][j+1]==0 ){
						negro=negro+1;		
					}	
				}
				
				if( j-1>=0 ){
					if(matrix[i][j-1]==0 ){
						negro=negro+1;		
					}	
				}
				
				if(  j+1<=matrix[0].size()-1 ){
					if(matrix[i][j+1]==0 ){
						negro=negro+1;		
					}	
				}
				
				if( i+1<=matrix.size()-1 && j-1>=0 ){
					if(matrix[i+1][j-1]==0 ){
						negro=negro+1;		
					}	
				}
				
				if( i+1<=matrix.size()-1){
					if(matrix[i+1][j]==0){
						negro=negro+1;		
					}	
				}
				
				if( i+1<=matrix.size()-1 && j+1<=matrix[0].size()-1 ){
					if(matrix[i+1][j+1]==0){
						negro=negro+1;		
					}		
				}
				if(negro>0)
					matrix[i][j]=-1;
			}	
		}
	}			
}

void WriteMatrix(string nombre, vector <vector<int>> &matrix){
	//Escribir nueva imagen .PGM
	ofstream file;	
	int m =matrix.size();
	int n =matrix[0].size();
	int I=0,J=0;
			
	file.open(nombre);
	file << "P2\n";
	file << n <<" "<<m<<"\n" ;
	file << "255\n";
	
	for(int i = 0; i < m*n; i++) {
		if(matrix[I][J]==0){
			file << "0\n";
		}
		else if(matrix[I][J]==-1){
			file << "150\n";
		}
		else{
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





