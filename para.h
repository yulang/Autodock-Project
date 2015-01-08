//
//  para.h
//  Autodock
//
//  Created by 余浪 on 1/7/15.
//  Copyright (c) 2015 余浪. All rights reserved.
//

#ifndef PARA_H
#define PARA_H

/************************ USE FOR FILE *******************/
#define MAX_INDEX 10
#define MAX_FILENAME 50
#define MAX_PATH 200
#define MAX_CONF_SIZE 500
#define MAX_CMD_LEN 200
#define MAX_JOBS 100

typedef int type;
#define CPU 0
#define MIC 1

#define CONF 0
#define LIG 1

#define MIC_NAME "mic0"

/************************ USE FOR JOB POOL *************/

#define CPU_CORE 24
#define MIC_CORE 240
#define CMPT_RATIO 2

#define NO_JOB -1
#define GET_JOB_ERROR -2

#endif
