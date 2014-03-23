#pragma once

#ifdef ENABLE_BEY_LOG
#define BEY_LOG_TEST 1
#else
#define BEY_LOG_TEST 0
#endif

#define BEY_LOG(MESSAGE)\
	do { if (BEY_LOG_TEST) std::cerr << MESSAGE << " (" << __FILE__ << ":" << __LINE__ <<  ")" << std::endl; } while (0)