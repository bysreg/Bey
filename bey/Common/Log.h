#pragma once

//temporary hack, because msvc does not support __func__ keyword
#ifdef WIN32
#define __func__ __FUNCTION__
#endif

#ifdef ENABLE_BEY_LOG
#define BEY_LOG_TEST 1
#else
#define BEY_LOG_TEST 0
#endif

#define BEY_LOG(MESSAGE)\
	do { if (BEY_LOG_TEST) std::cerr << MESSAGE << std::endl; } while (0)