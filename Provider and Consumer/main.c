/*
Sistemas de Informação - UFSC
INE5611 - Sistemas Operacionais
Trabalho 01 - IPC
Alan Vinicius Cezar Ensina - 16206354
Thiago Machado Raskopf - 16200921
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "queue.h"


/* Music structure */
typedef struct {    
  char* name;            /* Music name */
  char* author;          /* Music author */
  char* genre;           /* Music genre */    
  float duration;           /* Music duration */    
} music_t;

pthread_mutex_t queueMutex;
pthread_cond_t queueCond;

Queue queue;

music_t musics[] = {
  {"Paradise City","Guns N' Roses","Hard Rock",6.45},
  {"Welcome to the Jungle","Guns N' Roses","Hard Rock",4.32},
  {"Nightrain","Guns N' Roses","Hard Rock",4.26},
  {"Estranged","Guns N' Roses","Hard Rock",9.23},
  {"War Pigs","Black Sabbath","Heavy Metal",7.54},
  {"Iron Man","Black Sabbath","Heavy Metal",5.54},
  {"Paranoid","Black Sabbath","Heavy Metal",2.48},
  {"Snowblind","Black Sabbath","Classic Rock",5.26},
  {"The Rover","Led Zeppelin","Classic Rock",5.36},
  {"Black Dog","Led Zeppelin","Classic Rock",4.55},
  {"Dazed and Confused","Led Zeppelin","Classic Rock",6.26},
  {"Kashmir","Led Zeppelin","Classic Rock",8.28},
  {"Wheels","Foo Fighters","Alternative Rock",4.38},
  {"Everlong","Foo Fighters","Alternative Rock",4.10},
  {"Walk","Foo Fighters","Alternative Rock",4.15},
  {"Best of You","Foo Fighters","Alternative Rock",4.15},
  {"Don't Stop Me Now","Queen","Hard Rock",3.29},
  {"Under Pressure","Queen","Hard Rock",4.08},
  {"Somebody to Love","Queen","Hard Rock",4.56},
  {"Bohemian Rhapshody","Queen","Hard Rock",5.54},
  {"Come Together","The Beatles","Classic Rock",4.20},
  {"Here Comes the Sun","The Beatles","Classic Rock",3.06},
  {"Yesterday","The Beatles","Classic Rock",2.05},
  {"Hey Jude","The Beatles","Classic Rock",7.05},
  {"Gimme Shelter","The Rolling Stones","Classic Rock",4.30},
  {"Sympathy For The Devil","The Rolling Stones","Classic Rock",6.18},
  {"(I Can't Get No) Satisfaction","The Rolling Stones","Classic Rock",4.30},
  {"Start Me Up","The Rolling Stones","Classic Rock",3.33},
  {"Little By Little","Oasis","British Rock",37.8},
  {"Wonderwall","Oasis","British Rock",4.18},
  {"Champagne Supernova","Oasis","British Rock",7.31},
  {"Don't Go Away","Oasis","British Rock",4.48}
};

void printMusic(music_t* music) {
  printf("Music: %s\n"
    "Author: %s\n"
    "Genre: %s\n"
    "Duration: %f\n--------------------\n",    
    music->name,
    music->author, 
    music->genre,
    music->duration
  );
}

void Initialize() {
  //Initialize the mutex and the condition variable
  pthread_mutex_init(&queueMutex, NULL);
  pthread_cond_init(&queueCond, NULL);
}

void* Provider() {

  while (1) {
    music_t* music = &musics[rand() % 32];  

    //Lock the queue mutex to make sure that adding data to the queue happens correctly
    pthread_mutex_lock(&queueMutex);

    //Push new data to the queue
    enqueue(&queue, &music);
    
    //Signal the condition variable that new data is available in the queue
    pthread_cond_signal(&queueCond);

    //Done, unlock the mutex
    pthread_mutex_unlock(&queueMutex); 
    
    sleep(2);
  }  

  return NULL;
}


void* Consumer() {
  
  //Run the consumer loop
  while(1) {
    //Start by locking the queue mutex
    pthread_mutex_lock(&queueMutex);

    //As long as the queue is empty,
    while(!queueSize(&queue)) {
      // - wait for the condition variable to be signalled
      //Note: This call unlocks the mutex when called and
      //relocks it before returning!
      pthread_cond_wait(&queueCond, &queueMutex);
    }

    music_t* music;
    dequeue(&queue, &music);

    //Now unlock the mutex
    pthread_mutex_unlock(&queueMutex);

    printMusic(music);
  }

  return NULL;
}


int main(int argc, char* argv[]) {  
  
  Initialize();
  queueInit(&queue, sizeof(music_t));

  // pthread_t consumer;
  // pthread_create(&consumer, NULL, Consumer, NULL);

  pthread_t provider;
  pthread_create(&provider, NULL, Provider, NULL);

  pthread_t consumers[3];

  for (int i = 0; i < 3; ++i) {    
    // Creating a thread in detached mode passing the song list lenght as argument
    pthread_create(&consumers[i], NULL, Consumer, NULL);   
  }

  // Wait all consumer threads by joining them
  for (int i = 0; i < 3; i++) {
    pthread_join(consumers[i], NULL);  
  }  

  pthread_join(provider, NULL);  

  return EXIT_SUCCESS;
}