#include "TaskBarrierData.h"

namespace ipvtol
{


TaskBarrierData::TaskBarrierData() :
   readyIndicator(0)
{
   pthread_mutex_init(&tbdMutex, NULL);
   pthread_cond_init(&barrCond, NULL);
   return;
}

TaskBarrierData::~TaskBarrierData()
{
	pthread_mutex_destroy(&tbdMutex);
	pthread_cond_destroy(&barrCond);
}

}
