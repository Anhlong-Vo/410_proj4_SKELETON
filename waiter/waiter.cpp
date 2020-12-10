#include <string>
#include "stdlib.h"

#include "../includes/waiter.h"
#include "../includes/externs.h"
#include "../includes/PRINT.h"

using namespace std;


Waiter::Waiter(int id,std::string filename):id(id),myIO(filename){
}

Waiter::~Waiter()
{
}

//gets next Order(s) from file_IO
int Waiter::getNext(ORDER &anOrder){
	int available_order = myIO.getNext(anOrder);
	return available_order;
}

void Waiter::beWaiter() {


	ORDER order;
	int available_order = getNext(order);
	b_WaiterIsFinished = false;
	while (available_order == SUCCESS) {

		PRINT5("\nWAITER: ", id, " Inserting Order ", order.order_number, " into queue ");

		{
			unique_lock<mutex> lck(mutex_order_outQ);
			order_in_Q.push(order);
		}


		cv_order_inQ.notify_all();
		available_order = getNext(order);

	}
	cv_order_inQ.notify_all();
	b_WaiterIsFinished = true;


}

