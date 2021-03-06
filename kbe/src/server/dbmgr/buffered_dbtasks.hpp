/*
This source file is part of KBEngine
For the latest info, see http://www.kbengine.org/

Copyright (c) 2008-2012 KBEngine.

KBEngine is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

KBEngine is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.
 
You should have received a copy of the GNU Lesser General Public License
along with KBEngine.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __BUFFERED_DBTASKS_H__
#define __BUFFERED_DBTASKS_H__

// common include	
// #define NDEBUG
#include "dbtasks.hpp"
#include "cstdkbe/cstdkbe.hpp"
#include "cstdkbe/memorystream.hpp"
#include "thread/threadtask.hpp"
#include "helper/debug_helper.hpp"

namespace KBEngine{ 

/*
	数据库线程任务buffer
*/

class Buffered_DBTasks
{
public:
	typedef std::multimap<DBID, EntityDBTask*> DBID_TASKS_MAP;  
	typedef std::multimap<ENTITY_ID, EntityDBTask*> ENTITYID_TASKS_MAP;  
	
	Buffered_DBTasks();
	virtual ~Buffered_DBTasks();
	
	bool hasTask(DBID dbid);
	bool hasTask(ENTITY_ID entityID);

	void addTask(EntityDBTask* pTask);

	void onFiniTask(EntityDBTask* pTask);
protected:
	DBID_TASKS_MAP dbid_tasks_;
	ENTITYID_TASKS_MAP entityid_tasks_;
};

}
#endif
