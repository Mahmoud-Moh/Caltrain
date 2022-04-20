#include <pthread.h>

struct station {
    // FILL ME IN
    pthread_mutex_t mutex;
    pthread_cond_t thereIsTrain;
    pthread_cond_t allSeated;
    //number of people in station
    int waitingPassengers;
    //available seats in train
    int availableSeats;
    //passengers on board
    int onBoardPassengers;
    //Boolean indicates if there is a train in the station
    int trainExists;
};

void station_init(struct station *station);

void station_load_train(struct station *station, int count);

void station_wait_for_train(struct station *station);

void station_on_board(struct station *station);