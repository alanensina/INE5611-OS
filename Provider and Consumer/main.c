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
  {"Cardify","Sergio","Jazz",33.2},
  {"Aerified","Abbey","Pop",44.5},
  {"HomeIng","Gaspard","Jazz",62.5},
  {"Sonsing","Alecia","Funk",74.2},
  {"Voyatouch","Stephanus","Funk",23.6},
  {"Namfix","Johnathon","Jazz",24.8},
  {"SubEx","Lola","Funk",53.5},
  {"Treeflex","Whit","Rock",93.7},
  {"Latlux","Sheffield","Jazz",89.4},
  {"Fixflex","Pietro","Rock",83.5},
  {"Daltfresh","Blair","Pop",90.3},
  {"Pannier","Artemus","Pop",37.8},
  {"Job","Jaimie","HipHop",95.9},
  {"Tresom","Urson","Rock",90.5},
  {"Ronstring","Gherardo","Rock",76.4},
  {"TresZap","Barrie","Funk",64.6},
  {"Zathin","Remy","Jazz",18.9},
  {"Tempsoft","Katusha","HipHop",15.5},
  {"Domainer","Ilyssa","Rock",91.2},
  {"Tempsoft","Sabine","Pop",49.2}
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

void* Provider(void* data) {

  // while (1) {
    music_t* music = (music_t*)data;  

    //Lock the queue mutex to make sure that adding data to the queue happens correctly
    pthread_mutex_lock(&queueMutex);

    //Push new data to the queue
    enqueue(&queue, &music);
    
    //Signal the condition variable that new data is available in the queue
    pthread_cond_signal(&queueCond);

    //Done, unlock the mutex
    pthread_mutex_unlock(&queueMutex); 

    // printMusic(&music);
    // sleep(2);
  // }  

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

  pthread_t consumer;
  pthread_create(&consumer, NULL, Consumer, NULL);

  pthread_t providers[3];

  for (int i = 0; i < 3; ++i) {
    pthread_create(&providers[i], NULL, Provider, &musics[rand() % 20]);    
  }

  // Wait all provider threads by joining them
  for (int i = 0; i < 3; i++) {
    pthread_join(providers[i], NULL);  
  }  

  pthread_join(consumer, NULL);  

  return EXIT_SUCCESS;
}