#include "pintos_thread.h"


void make_water();

struct reaction {
	int h_atoms; 				//Khai báo biến Hidro				
	pthread_cond_t h_moi_den ;		// thông báo 1 hidro mới được tạo ra
	pthread_cond_t h_xayra_phanung;	// Hidro đã xảy ra phản ứng
	pthread_mutex_t mutex;
};

void reaction_init(struct reaction *reaction)
{

	// Khởi tạo khóa mutex và biến điều kiện
	pthread_mutex_init(&reaction->mutex,NULL);
    	pthread_cond_init(&reaction->h_moi_den ,NULL);
    	pthread_cond_init(&reaction->h_xayra_phanung,NULL);
	  
	reaction->h_atoms = 0;
}

void reaction_h(struct reaction *reaction)
{
	pthread_mutex_lock(&(reaction->mutex));		//Khóa trước khi vào vùng critical section
	reaction->h_atoms++;				//tăng Hidro
	pthread_cond_signal(&reaction->h_moi_den );  // Hàm signal thông báo hidro mới sinh ra
	pthread_cond_wait(&reaction->h_xayra_phanung,&reaction->mutex); //Hàm wait đợi Hidro đang xảy ra phản ứng
	pthread_mutex_unlock(&(reaction->mutex));	//Mở khóa và rời critical section
}
void reaction_o(struct reaction *reaction)
{
	pthread_mutex_lock(&(reaction->mutex));
	
	while (reaction->h_atoms < 2) {
		pthread_cond_wait(&reaction->h_moi_den ,&reaction->mutex);	
	}
	make_water(); 					// Gọi hàm make water trong main
	reaction->h_atoms-=2;				// mỗi lần gọi O thì số Hidro giảm 2
	pthread_cond_signal(&reaction->h_xayra_phanung);// Thông báo 2 Hidro xảy ra phản ứng xong
	pthread_cond_signal(&reaction->h_xayra_phanung);// giải phóng 2 thread H
	pthread_mutex_unlock(&(reaction->mutex));
}

