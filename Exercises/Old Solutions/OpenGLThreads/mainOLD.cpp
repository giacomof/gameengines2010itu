/*

#include <SDL.h>
#include <SDL_thread.h>
#include <stdio.h>
#include <stdlib.h>


using namespace std;

int account_value = 0;		//shared variable
int total = 0;			//shared variable
SDL_mutex *value_mutex;		//mutex to lock variables
bool quit = false;

//This thread reads account_value and total
int reader ( void *data )
{
  char *tname = ( char * )data;
  
  while ( !quit ) {
    printf("I am %s: ", tname );
    SDL_mutexP ( value_mutex );		//lock to read value
    //now you can sefely access account_value and total
    printf(" My account value and total are: %d, %d.\n", 
		account_value, total );
    //release the lock 
    SDL_mutexV ( value_mutex );

    //delay for a random amount of time
    SDL_Delay ( rand() % 1000 );
  }
  printf("%s is quiting.\n", tname );  
      
  return 0;
}


//This thread  writes value
int writer ( void *data )
{
  char *tname = ( char * )data;

  while ( !quit ) {
    int a = rand() % 100;	//get a random number
    printf("I am %s: ", tname );
    
    SDL_mutexP ( value_mutex ); //lock before upgrading 

    //now you can sefely access values
    account_value += a;
    total += a;
    printf(" I deposited an amount of %d\n", a );

    //release the lock 
    SDL_mutexV ( value_mutex );

    //delay for a random amount of time
    SDL_Delay ( rand() % 2000 );
  }
  printf("%s  is quiting.\n", tname );

  return 0;
}

int main (int argc, char *argv[])
{
  SDL_Thread *id1, *id2;			//thread identifiers
  char *tnames[2] = { "Reader", "Writer" };	//names of threads
  
  value_mutex = SDL_CreateMutex();
  //create the threads
  id1 = SDL_CreateThread ( reader, tnames[0] );
  id2 = SDL_CreateThread ( writer, tnames[1] );

  //experiment with 10 seconds
  for ( int i = 0; i < 5; ++i )
      SDL_Delay ( 2000 );

  quit = true;				//signal the threads to return
  //wait for the threads to exit
  SDL_WaitThread ( id1, NULL );
  SDL_WaitThread ( id2, NULL );

  SDL_DestroyMutex ( value_mutex );	//release the resources

  system("PAUSE");
  return 0;  
} 
*/