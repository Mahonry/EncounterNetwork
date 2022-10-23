#include<cstdlib>
#include<fstream> 
#include<iostream>
#include<vector>
#include<random>
#include <time.h>
#include<cmath>
using namespace std;

//Provisionalmente definimos una gamma
double  gamma_ = 0;

//Declaramos el numero de pasos
int months = 1;

//Declaramos el numero de empresas
const int Nv = 10;

//Declaramos el numero de caminantes 
const int Rws = 2;

//Declaramos matriz de adjacencia (t) en ceros
int matA [Nv][Nv] = {0,0}; 

//Declaramos los vecinos
vector<vector<int>> Neighbors(Nv);	

//Declaramos los grados
vector<int> Degrees(Nv,0);  

//Declarmos el vector donde se guarda el historial de visitas
vector<vector<int>> Visits(Rws);

//Random Integer
int RandomInteger(int val)
{
	return rand()%(val);
}

//Inicializacion de los Rw en nodos aleatorios
void initialization_rw(vector<vector<int>>& Visits, int Rws, int Nv)
{

    for(int i = 0; i <Rws; i++)
    {
        Visits[i].push_back(RandomInteger(Nv));
    }
}   

//funcion auxiliar para verificacion
void print_neighbords(vector<vector<int>>& Neighbors)
{
    cout<<"Print Newighbords"<<endl;
    for(int i = 0; i<Nv;i++)
    {
        int max = Neighbors[i].size();
        for(int j = 0; j<max;j++)
        {
            double a = Neighbors[i][j];
            cout<<a<<endl;
        }
        
    }
    cout<<"___"<<endl;
}


//Funcion que actualiza Neighbors
void update_neighbords(vector<vector<int>>& Neighbors,int pos_init, int new_position)
{
    print_neighbords(Neighbors);
    if(pos_init != new_position)
    {
        Neighbors[pos_init].push_back(new_position);
	    Neighbors[new_position].push_back(pos_init);
    }
    print_neighbords(Neighbors);
}

//Funcion que actualiza los grados
void update_degree(vector<int>& Degrees, vector<vector<int>> Neighbors, int Nv)
{
    for(int i = 0; i < Nv; i++)
    {
        Degrees[i] = Neighbors[i].size();
    }
}

//Moneda
int Coin(double p)
{
	double val = 1.0*rand()/RAND_MAX;
	int out;	
	
	if (val<=p){
		out = 1;
	}
	else{
		out = 0;
	}	
	return out;
}

//Funcion que genera la transiscion
int Transition(double gamma, int Nv, int pos_init, vector<vector<int>> Neighbors, vector<int> Degrees)
{
    int new_position;
    int selection = Coin(gamma);
    cout<<"Selection "<<selection<<endl;
    if (selection == 1)
    {
        if (Degrees[pos_init] == 0)
        {
            new_position = pos_init;
        }
        else
        {
            new_position =  Neighbors[pos_init][RandomInteger(Degrees[pos_init])];
        }
    }
    else
    {
         new_position = RandomInteger(Nv);
    }
    return new_position;
}

int main()
{
    //Inicializamos el generador de numeros aleatorios
    srand(time(NULL));

    //Inicializamos la empresa inicial en aletorio
    initialization_rw(Visits, Rws, Nv);

    //Verificamos la inicializacion de  visitas
    

    cout<<"Visits"<<endl;
    for(int Rw = 0; Rw<Rws;Rw++)
    {
        cout<<"RW "<<Rw<<endl;
        double max = Visits[Rw].size();
        for(int i = 0; i<max; i++)
        {
            double a = Visits[Rw][i];
            cout<<a<<endl;
        }
    }
    cout<<"___"<<endl;

    cout<<"Degrees"<<endl;
    for(int i = 0; i<Nv; i++)
    {
        double b = Degrees[i];
        cout<<b<<endl;
    }
    cout<<"___"<<endl;

    for(int month = 0; month<months; month++)
        {          
            for(int Rw = 0; Rw<Rws; Rw++)
            {
                //Hacamos el paso
                int pos_init = Visits[Rw][month];
                int new_position = Transition(gamma_,Nv,pos_init,Neighbors,Degrees);

                //Actualizamos la visita
                Visits[Rw].push_back(new_position);
                
                cout<<pos_init<<" "<<new_position<<endl;
   
                //Actualizamos vecinos
                update_neighbords(Neighbors,pos_init,new_position);

                
                
                //Actualizamos el grado
                update_degree(Degrees, Neighbors, Nv);

                cout<<"AVANCE"<<endl;

                

                //Revisamos de nuevo los grados
                cout<<"New_grades"<<endl;
                for(int cont1 = 0; cont1<Nv; cont1++)
                {
                    double b = Degrees[cont1];
                    cout<<b<<endl;
                }
                cout<<"___"<<endl;
      



                
                
                cout<<"New Visits"<<endl;
                for(int cont2 = 0; cont2<Rws;cont2++)
                {
                    cout<<"RW "<<cont2<<endl;
                    double max = Visits[cont2].size();
                    for(int cont3 = 0; cont3<max; cont3++)
                    {
                        double a = Visits[cont2][cont3];
                        cout<<a<<endl;
                    }
                }

            }
            //Revisamos las nuevas visitas
        }

//Guardamos las visitas
/* 
    for(int Rw = 0; Rw<Rws; Rw++)
       {
            string name = "./Visits_Rw/Rw_" + to_string(Rw) + ".dat";
			ofstream Rw_data (name);

            //Creamos encabezado de los datos 
            Rw_data<<"Rw,"<<"N_visited,"<<"t"<<endl;
            for (int month = 0; month<months; month++)
            {
                cout<<"funciona"<<endl;
                Rw_data<<Rw<<","<<Visits[Rw][month]<<","<<month<<endl;
            }

            Rw_data.close();

        }
*/
    return 0;
}