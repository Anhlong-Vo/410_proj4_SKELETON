#include <mutex>
#include <atomic>
#include "../includes/baker.h"
#include "../includes/externs.h"
#include "../includes/PRINT.h"
using namespace std;

Baker::Baker(int id):id(id)
{
}

Baker::~Baker()
{
}

void Baker::bake_and_box(ORDER &anOrder) {
	PRINT4("\nBaker: ", id, " is baking and boxing order ", anOrder.order_number)
	Box box;
	atomic<int> num_donuts(0);


	for (int i = 0; i < anOrder.number_donuts; i++) {

		num_donuts++;

		PRINT5("Baker: ", id, " Inserting Donut ", num_donuts, " into box");
		DONUT d;
		box.addDonut(d);

		if (num_donuts % DOZEN == 0) {
			PRINT1("max donuts in this box... \nSwitching out boxes...");
			anOrder.boxes.push_back(box);
			box.clear();
		}
	}
	anOrder.boxes.push_back(box);

	
}

void Baker::beBaker() {

	while (true) {

		unique_lock<mutex> lck(mutex_order_inQ);

		if (b_WaiterIsFinished==true && order_in_Q.empty()) {
			break;
		}

		while (order_in_Q.empty() && b_WaiterIsFinished == false) {
			PRINT3("Baker: ", id, " waiting.... ");
			cv_order_inQ.wait(lck);
		}

		lck.unlock();
		if (!order_in_Q.empty()) {

			ORDER order;
			{
				unique_lock<mutex> lk(mutex_order_outQ);
				order = order_in_Q.front();
				order_in_Q.pop();
			}

			bake_and_box(order);
			order_out_Vector.push_back(order);
		}
		cv_order_inQ.notify_all();

	}

	cv_order_inQ.notify_all();
	PRINT3("Baker: ", id, " is done");

}
