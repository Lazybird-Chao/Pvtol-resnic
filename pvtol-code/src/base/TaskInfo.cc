#include <TaskInfo.h>

namespace ipvtol
{

///Helper function to output the state of TaskInfo
std::ostream& operator<< (std::ostream& output, const TaskInfo& taskInfo)
{
	return 
		output <<"TaskInfo" << std::endl <<
				"\t taskId: \t" << taskInfo.taskId << std::endl <<
				"\t parentTaskId: \t"  << taskInfo.parentTaskId <<std::endl <<
				"\t threadId: \t" << taskInfo.threadId <<std::endl <<
				"\t localRank: \t"<< taskInfo.localRank <<std::endl <<
				"\t globalRank: \t" << taskInfo.globalRank <<std::endl <<
				"\t self \t" << taskInfo.self <<std::endl;				
	
}

}

