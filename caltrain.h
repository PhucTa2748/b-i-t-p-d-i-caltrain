#include <pthread.h>
#include <stdbool.h>

struct station {
    int ghe_trong; // số ghế trống trên tàu khi tàu đến
    int khach_doi; // số khách đang đợi trong trạm
    int khach_dalen; // số khách đã lên tàu nhưng chưa ngồi
    pthread_mutex_t mutex;
    pthread_cond_t ghe_trong_hienco; // ghế trống hiện có 
    pthread_cond_t khach_da_ngoi; // toàn bộ khách đều đã ngồi
};


void station_init(struct station *station);


void station_load_train(struct station *station, int count);


void station_wait_for_train(struct station *station);


void station_on_board(struct station *station);
