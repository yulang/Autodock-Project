#ifndef LOG_H
#define LOG_H

#define print(fmt, args...) printf(fmt, ##args)

#define test(fmt, args...) printf(fmt, ##args);\
exit(0)

#define pause() getchar()
#endif