#if defined(ENABLE_BEY_LOG)
#define BEY_LOG(LOG_TAG, MESSAGE) printf(MESSAGE)
#else
#define BEY_LOG(LOG_TAG, MESSAGE) ((void)0)
#endif