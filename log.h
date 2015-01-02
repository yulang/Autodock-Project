#ifndef LOG_H
#define LOG_H

#define print(fmt, args...) printf(fmt, ##args)
#define test(args) printf("%d\n", args)

#endif