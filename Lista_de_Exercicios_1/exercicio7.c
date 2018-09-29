thread passageiro {
  while (!fechouParque) {
        entraNoCarro(); 
        esperaVoltaAcabar(); 
        saiDoCarro(); 
        passeiaPeloParque(); // tempo variavel
        }
}
As threads carro executam o seguinte pseudo-código:
process carro { 
   while (existemPassageirosNoParque) { 
         esperaEncher(); 
         daUmaVolta(); 
         esperaEsvaziar(); 
         volta++; // serve como parametro para fechar o parque 
   } 
}


#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
int parquefechou=0,volta=0;
pthread_t pass,car;

void *passageiro()
{
	
}

void *carro()
{	
	
}

int main(void)
{
	pthread_create(&pass,NULL,passageiro,NULL);
	pthread_create(&car,NULL,carro,NULL);
	pthread_join(pass,NULL);
	pthread_join(car,NULL);

}
