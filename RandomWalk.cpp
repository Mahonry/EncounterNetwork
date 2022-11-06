#include<cstdlib>
#include<fstream> 
#include<iostream>
#include<vector>
#include<random>
#include <time.h>
#include<cmath>
using namespace std;

//Provisionalmente definimos una gamma 
double  gamma_ = 0.976;

//Provisionalmente definimos una q
double q = 100;

//Probabilidad de crear una conexion
double conexion = 1.00;

//Declaramos el numero de pasos
const int months = 155;

//Declaramos el numero de empresas
const int Nv = 775;

//Declaramos el numero de caminantes 
const int Rws = 386;


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

//Random Integer
int RandomInteger(int val)
{
	return rand()%(val);
}

//Random Choice
int RandomChoice(vector<double> Freqs,mt19937 &gen)
{
	discrete_distribution<> d(Freqs.begin(),Freqs.end());
	return d(gen);	
}

//Probabilidades de transicion para Random Choice, Transition_1
vector<double> TransitionP_generation(double P, int grade) 
{
    double prob = (1-P)/grade;
    vector<double> Transition;
    //cout<<"inside Transition"<<endl;
    //cout<<"Grade "<<grade<<" TransitionP "<<Transition.size()<<endl;
    for(int i = 0; i < grade; i++)
    {
        Transition.push_back(prob);
    }
    //cout<<"Grade "<<grade<<" TransitionP "<<Transition.size()<<endl;
    return Transition;
}

//Probabilidades de transicion para Random Choice, Transition_2
vector<double> TransitionP_generation_2(int grade) 
{
    double prob = grade/(grade+1);
    vector<double> Transition;
    for(int i = 0; i < grade; i++)
    {
        Transition.push_back(prob);
    }
    return Transition;
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



//Funcion que genera la transiscion
int Transition_1(double gamma, 
                double q, 
                int Nv, 
                int pos_init, 
                vector<vector<int>> Neighbors, 
                vector<int> Degrees,
                mt19937 &gen)
{
    int new_position;
    int selection = Coin(gamma);
    if (selection == 1)
    {
        if (Degrees[pos_init] == 0)
        {
            //cout<<"opcion 1"<<endl;
            new_position = pos_init;
        }
        else
        {
            double degree_i = Degrees[pos_init];
            double prob = pow(q,degree_i);
            int selection = Coin(prob);
            
            if(selection ==1)
            {
                //cout<<"opcion 2"<<endl;
                new_position = pos_init;
            }
            else
            {
                //cout<<"opcion 3"<<endl;
                vector<double> TransitionP = TransitionP_generation(prob, degree_i);
                //cout<<"Degres "<<degree_i<<" Tamaño Neighbors "<<Neighbors[pos_init].size()<<" Tamaño TransitionP "<<TransitionP.size()<<endl;
                new_position = Neighbors[pos_init][RandomChoice(TransitionP,gen)];
            }

        }
    }
    else
    {
        //cout<<"opcion 4"<<endl;
         new_position = RandomInteger(Nv);
    }
    return new_position;
}

int Transition_2(double gamma, 
                int Nv, 
                int pos_init, 
                vector<vector<int>> Neighbors, 
                vector<int> Degrees,
                mt19937 &gen)
{
    int new_position;
    int selection = Coin(gamma);
    if (selection == 1)
    {
        new_position = pos_init;
    }
    else
    {
        double degree_i = Degrees[pos_init];
        double p;
        if( degree_i == 0)
        {
            double p = 0;
        }
        else
        {
            double p = Degrees[pos_init]/(Degrees[pos_init]+1);
        }
        
        int selection = Coin(p);

        if(selection == 1)
        {                    
                vector<double> TransitionP = TransitionP_generation_2(degree_i);
                new_position = Neighbors[pos_init][RandomChoice(TransitionP,gen)];
        }
        else
        {
            new_position = RandomInteger(Nv);
        }

    }
    return new_position;
}

int Transition(double gamma, int Nv, int pos_init, vector<vector<int>> Neighbors, vector<int> Degrees)
{
    int new_position;
    int selection = Coin(gamma);
    if (selection == 1)
    {
        if (Degrees[pos_init] == 0 )
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

void Rw_move(double gamma_, double q,int iteration,mt19937 &gen)
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
                //Hacemos el paso
                int pos_init = Visits[Rw][month];
                int new_position = Transition_2(gamma_,Nv,pos_init,Neighbors,Degrees,gen);//Transition_1(gamma_,q,Nv,pos_init,Neighbors,Degrees,gen);
                //cout<<Rw<<" "<<pos_init<<" "<<new_position<<endl;
                //Actualizamos la visita
                Visits[Rw].push_back(new_position);


                double p_conexion = Coin(conexion);
                if (p_conexion == 1)
                {
                    //Actualizamos vecinos
                    update_neighbords(Neighbors,pos_init,new_position);

                    //Actualizamos el grado
                    update_degree(Degrees, Neighbors, Nv);
                }
                else
                {
                    new_position = pos_init;
                    //Actualizamos vecinos
                    update_neighbords(Neighbors,pos_init,new_position);

                    //Actualizamos el grado
                    update_degree(Degrees, Neighbors, Nv);
                }
            }
        }

    //Guardamos las visitas
    string name = "./Visits_Rw/Rw_" + to_string(gamma_) + "_" + to_string(q) + "_" + to_string(iteration) + "_.dat";
	//string name = "./Visits_Rw/Prueba_" + to_string(iteration) + ".dat";
    ofstream Rw_data (name);
    //Creamos encabezado de los datos 
    Rw_data<<"Rw,"<<"N_visited,"<<"t,"<<"q,"<<"gamma"<<endl;
    for(int Rw = 0; Rw<Rws; Rw++)
       {
            for (int month = 0; month<=months; month++)
            {
                Rw_data<<Rw<<","<<Visits[Rw][month]<<","<<month<<","<<q<<","<<gamma_<<endl;
            }


        }
    Rw_data.close();
}

int main()
{
    //Inicializamos el generador de numeros aleatorios
    srand(time(NULL));
    //Generador para el Random Choice
    random_device rd;
	mt19937 gen(rd());
    
    //Provisionalmete definimos la iteracion. Transition_1
    /*int iteration = 4;
    
    for(double gamma_ = .98; gamma_<1; gamma_ += 0.001)
    {
        for(double q = .98; q < 1; q += 0.001)
        {
            Rw_move(gamma_, q, iteration, gen);
        }
    }*/

    //Generamos muchas corridas para la Transition_1 con valor de gamma = .997 y q = .991
    for(int iteration = 0; iteration<200; iteration++)
    {
        cout<<iteration<<endl;
        Rw_move(gamma_,q, iteration, gen);
    }


    //Transition_2
    //q provisional para Rw
    /*double q = 100;
    int iteration = 4;
    for(double gamma_ = .90; gamma_<1; gamma_ += 0.001)
    {
        Rw_move(gamma_, q, iteration, gen);
    }    
    return 0;*/
}