#include <pthread.h>
#include "caltrain.h"



void
station_init(struct station *station)
{
    pthread_mutex_init(&station->mutex, NULL);
    pthread_cond_init(&station->thereIsTrain, NULL);
    pthread_cond_init(&station->allSeated, NULL);

    station->waitingPassengers=0;
    station->availableSeats=0;
    station->onBoardPassengers=0;
}

void
station_load_train(struct station *station, int count)
{
    pthread_mutex_lock(&station->mutex);
    station->availableSeats=count;
    station->trainExists = 1;
    //If there are available seats and there are waiting passengers
    //let passengers enter the train
    if (station->availableSeats > 0 && station->waitingPassengers > 0) {
        //notify all passengers they can enter the train
        pthread_cond_broadcast(&station->thereIsTrain);
        //wait on the condition that all passengers are seated
        pthread_cond_wait(&station->allSeated, &station->mutex);
    }
    station->availableSeats=0;
    station->trainExists = 0;
    pthread_mutex_unlock(&station->mutex);

}

void
station_wait_for_train(struct station *station)
{
    pthread_mutex_lock(&station->mutex);
    station->waitingPassengers++;
    //If there is no train in the station
    //or there is one but it is full, passenger has to wait.
    while (station->trainExists == 0 || (station->onBoardPassengers == station->availableSeats)) {
        pthread_cond_wait(&station->thereIsTrain, &station->mutex);
    }
    //passenger is boarded on the train
    station->onBoardPassengers++;
    pthread_mutex_unlock(&station->mutex);

}

void
station_on_board(struct station *station)
{
    pthread_mutex_lock(&station->mutex);
    station->waitingPassengers--;
    station->onBoardPassengers--;
    station->availableSeats--;
    //If all passengers are boarded ==> on_board was called for all passengers and
    //no more available seats or all waiting passengers are boarded
    //then the train can go.
    if((station->availableSeats==0 || station->waitingPassengers==0) && station->onBoardPassengers==0){
        pthread_cond_signal(&station->allSeated);
    }
    pthread_mutex_unlock(&station->mutex);

}