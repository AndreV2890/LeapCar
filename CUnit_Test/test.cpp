#include <CUnit/CUnit.h>
#include <CUnit/TestDB.h>
#include <CUnit/CUError.h>
#include <CUnit/TestRun.h>
#include <CUnit/Automated.h>
#include <CUnit/Basic.h>

#include "StateTable.h"
#include "Buffer.h"

StateTable sm;
Buffer* toSend;

Signal_ST getSignal(char temp, char IdtoACK, Timer* t){
	int rec_ret = 1;
	unsigned char utemp = (unsigned char) temp;

	if(rec_ret > 0){
		unsigned char app = utemp>>7;
		if(app == 0);
		else {
			app = utemp<<4;
			if(app == 0){
				app = utemp<<1;
				app = app >> 5;
				unsigned char Id = (unsigned char) IdtoACK;
				if(app == Id)
					return ack_id_correct;
				else
					return ack_id_incorrect;
			}
		}
	}

	if(t->active && t->count <= 0) return end_timer;

	if(!toSend->isEmpty()) return msg_to_send;

	return signal_tick;
}

int init_suite(void){
	printf ("\nInitializing Suite_StateTable\n");
	return 0;
}

int clean_suite(void){
	printf ("\nCompleting Suite_StateTable\n");
	return 0;
}

//Function to check if for each entries of state table the action implemented
//is equal to the desired one
void test_action(void){

	CU_ASSERT(sm.dispatch1(wait_for_ACK, ack_id_correct) == remove_msg);
	CU_ASSERT(sm.dispatch1(wait_for_ACK, ack_id_incorrect) == do_nothing);
	CU_ASSERT(sm.dispatch1(wait_for_ACK, msg_to_send) == do_nothing);
	CU_ASSERT(sm.dispatch1(wait_for_ACK, end_timer) == send_reset);
	CU_ASSERT(sm.dispatch1(wait_for_ACK, signal_tick) == do_nothing);

	CU_ASSERT(sm.dispatch1(wait_for_msg, ack_id_correct) == do_nothing);
	CU_ASSERT(sm.dispatch1(wait_for_msg, ack_id_incorrect) == do_nothing);
	CU_ASSERT(sm.dispatch1(wait_for_msg, msg_to_send) == send_start);
	CU_ASSERT(sm.dispatch1(wait_for_msg, end_timer) == do_nothing);
	CU_ASSERT(sm.dispatch1(wait_for_msg, signal_tick) == do_nothing);
}

//Function to check if for each entries of state table the transaction implemented
//is equal to the desired one
void test_transaction(void){

	CU_ASSERT(sm.dispatch2(wait_for_ACK, ack_id_correct) == wait_for_msg);
	CU_ASSERT(sm.dispatch2(wait_for_ACK, ack_id_incorrect) == wait_for_ACK);
	CU_ASSERT(sm.dispatch2(wait_for_ACK, msg_to_send) == wait_for_ACK);
	CU_ASSERT(sm.dispatch2(wait_for_ACK, end_timer) == wait_for_ACK);
	CU_ASSERT(sm.dispatch2(wait_for_ACK, signal_tick) == wait_for_ACK);

	CU_ASSERT(sm.dispatch2(wait_for_msg, ack_id_correct) == wait_for_msg);
	CU_ASSERT(sm.dispatch2(wait_for_msg, ack_id_incorrect) == wait_for_msg);
	CU_ASSERT(sm.dispatch2(wait_for_msg, msg_to_send) == wait_for_ACK);
	CU_ASSERT(sm.dispatch2(wait_for_msg, end_timer) == wait_for_msg);
	CU_ASSERT(sm.dispatch2(wait_for_msg, signal_tick) == wait_for_msg);
}

void test_getSignal(){

	//We receive a byte from the Xbee and we check if the byte is an ACK (MSB = 1)
	//and if it is an ACK we the Id inside this byte

	//TEST CASE: suppose we are waiting the ACK 101
	//CASE 1: we receive 0xD0 that it is 1101000 so we assume to receive "ack_id_correct"
	//CASE 2: we receive 0xF0 that it is 1111000 so we assume to receive "ack_id_incorrect"
	CU_ASSERT(getSignal(0xD0, 0x05, sm.timer) == ack_id_correct);
	CU_ASSERT(getSignal(0xF0, 0x05, sm.timer) == ack_id_incorrect);

	//Now we receive a byte that it isn't an ACK (MAB = 0) so we can check if there is
	//a message in the buffer or if the timer is over
	//
	//TEST CASE: we receive 0x50
	//CASE 1: there is a msg inside the buffer and the timer is not over
	//	      so we assume to receive "msg_to_send"
	//CASE 2: there is a msg inside the buffer but the timer is over
	//		   so we assume to receive "end_timer"
	//CASE 3: there isn't any msg insede the buffer and the timer in not over
	//		  so we assume to receive "signal_tick"
	CU_ASSERT(getSignal(0x50, 0x05, sm.timer) == msg_to_send);
	sm.timer->count = -1;
	CU_ASSERT(getSignal(0x50, 0x05, sm.timer) == end_timer);
	sm.timer->count = 1;
	toSend->remove();
	CU_ASSERT(getSignal(0x50, 0x05, sm.timer) == signal_tick);

}

//In this test we check the behavior of the full implementation of the dispatch function
//We put one value in the buffer and we assume to be in "wait_for_msg" state
//at the and of the simulation we assume to be in "wait_to_ACK" with an empty buffer.
//During the elaboration we check the result of the getSignal
void test_dispatch(){
	char msg[3] = {0x80,0x35,0x72};
	toSend->add(msg);

	char Id = 0x00;

	sm.current_state = wait_for_msg;
	Signal_ST s = getSignal(0x00, 0x00, sm.timer);
	sm.dispatch(s, msg, toSend, &Id);

	CU_ASSERT_EQUAL(sm.current_state, wait_for_ACK);
}

int main (void)
{
   CU_pSuite pSuite = NULL;

   sm.init();
   sm.timer->active_timer();
   sm.timer->count = 1;

   toSend = new Buffer();
   char msg[3] = {0x80,0x00,0x00};
   toSend->add(msg);

   /* Initialize CUnit test registry */
   if (CUE_SUCCESS != CU_initialize_registry())
      return CU_get_error();

   /* Add suite to registry */
   pSuite = CU_add_suite("Suite_StateTable", init_suite, clean_suite);
   if (NULL == pSuite) {
      CU_cleanup_registry();
      return CU_get_error();
      }

   /* add test to suite */
   if ((NULL == CU_add_test(pSuite, "Test_Action_StateTable", test_action)))
   {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* add test to suite */
   if ((NULL == CU_add_test(pSuite, "Test_Transiction_StateTable", test_transaction)))
   {
      CU_cleanup_registry();
      return CU_get_error();
   }
   /* add test to suite */
   if ((NULL == CU_add_test(pSuite, "Test_getSignal", test_getSignal)))
   {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* add test to suite */
   if ((NULL == CU_add_test(pSuite, "Test_dispatch", test_dispatch)))
   {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* Run tests using Basic interface */
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();

   /* Run test using Automated interface */
   CU_set_output_filename("Output_Cunit");
   CU_automated_run_tests();
   CU_list_tests_to_file();

   /* Clean up registry and return */
   CU_cleanup_registry();
   return CU_get_error();
}
