#include<cstdlib>
#include<fstream> 
#include<iostream>
#include<vector>
#include<random>
#include <time.h>
#include<cmath>
using namespace std;

//Provisionalmente definimos una gamma 
double  gamma_ = 0.90;

//Declaramos el numero de pasos
const int months = 155;

//Declaramos el numero de empresas
const int Nv = 848;

//Declaramos el numero de caminantes 
const int Rws = 386;

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

//Funcion que actualiza Neighbors
void update_neighbords(vector<vector<int>>& Neighbors,int pos_init, int new_position)
{
    if(pos_init != new_position)
    {
        Neighbors[pos_init].push_back(new_position);
	    Neighbors[new_position].push_back(pos_init);
    }
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

void Rw_move(double gamma_, int iteration)
{
    //Declaramos los vecinos
    vector<vector<int>> Neighbors(Nv);	
    //Declaramos los grados
    vector<int> Degrees(Nv,0);  
    //Declarmos el vector donde se guarda el historial de visitas
    vector<vector<int>> Visits(Rws);
    //Inicializamos la empresa inicial en aletorio
    initialization_rw(Visits, Rws, Nv);

    for(int month = 0; month<months; month++)
        {          
            for(int Rw = 0; Rw<Rws; Rw++)
            {
                //Hacamos el paso
                int pos_init = Visits[Rw][month];
                int new_position = Transition(gamma_,Nv,pos_init,Neighbors,Degrees);

                //Actualizamos la visita
                Visits[Rw].push_back(new_position);

                //Actualizamos vecinos
                update_neighbords(Neighbors,pos_init,new_position);

                //Actualizamos el grado
                update_degree(Degrees, Neighbors, Nv);
            }
        }

    //Guardamos las visitas
     string name = "./Visits_Rw/Rw_" + to_string(gamma_) + ".dat";
	ofstream Rw_data (name);
    //Creamos encabezado de los datos 
    Rw_data<<"Rw,"<<"N_visited,"<<"t"<<endl;
    for(int Rw = 0; Rw<Rws; Rw++)
       {
            for (int month = 0; month<=months; month++)
            {
                Rw_data<<Rw<<","<<Visits[Rw][month]<<","<<month<<endl;
                cout<<Rw<<","<<Visits[Rw][month]<<","<<month<<endl;
            }


        }
    Rw_data.close();
}

int main()
{
    //Inicializamos el generador de numeros aleatorios
    srand(time(NULL));
    for(gamma_ = 0.90, gamma_ <=; gamma += 0.01)
    {
        Rw_move(gamma_, 1);
    }
    
    return 0;
}