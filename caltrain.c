#include <pthread.h>
#include "caltrain.h"

void station_init(struct station *station) 
{ 
   /*Khởi tạo nhà ga trống */
    // Số ghế trống trên tàu khi có chỗ trống trong trạm
    station->ghe_trong = 0;  
    // Số khách hiện đang đợi trong trạm           
    station->khach_doi = 0;     
    // Số khách đã lên tàu nhưng chưa ngồi
    station->khach_dalen = 0;     
    /* Khởi tạo khóa mutex của trạm và biến điều kiện  */
    pthread_mutex_init(&(station->mutex), NULL);
    pthread_cond_init(&(station->ghe_trong_hienco), NULL);
    pthread_cond_init(&(station->khach_da_ngoi), NULL);
}

void station_load_train(struct station *station, int count) 
{
    pthread_mutex_lock(&(station->mutex)); // khóa trước khi vào vùng critical section
    if (!(count >0)|| !(station->khach_doi >0)) { // không còn ghe trống hoặc không còn khách đợi
        pthread_mutex_unlock(&(station->mutex)); // mở khóa và rời vùng critical section
        return; 				// Tàu rời ga ngay lập tức
    }
    station->ghe_trong = count;
    pthread_cond_broadcast(&(station->ghe_trong_hienco)); // hàm thông báo khi tàu có ghế trống đến
    pthread_cond_wait(&(station->khach_da_ngoi), &(station->mutex)); //hàm wait đợi khach vào chỗ ngồi
    station->ghe_trong = 0;
    pthread_mutex_unlock(&(station->mutex)); // Mở khóa và rời vùng critical section
}

void station_wait_for_train(struct station *station)
 {
    pthread_mutex_lock(&(station->mutex)); 
    station->khach_doi++;
    while (!(station->ghe_trong >0)) {
        pthread_cond_wait(&(station->ghe_trong_hienco), &(station->mutex)); // Đợi tàu có ghế trống đến
    }
    station->khach_doi--;
    station->khach_dalen++;
    station->ghe_trong--;
    pthread_mutex_unlock(&(station->mutex)); 
}

void station_on_board(struct station *station) 
{
    pthread_mutex_lock(&(station->mutex)); 
    station->khach_dalen--;
    if (!(station->khach_dalen>0) && !((station->khach_doi>0) && (station->ghe_trong>0) )) {

// Toàn bộ khách đã lên tàu và ngồi vào chỗ, không còn ghế trống hoặc không còn khách đợi
        pthread_cond_signal(&(station->khach_da_ngoi));// Thông báo toàn bộ khách đã ngồi
	}
    pthread_mutex_unlock(&(station->mutex)); 
}
