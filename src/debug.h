//
// Created by Tiane Zhu on 11/25/20.
//

#ifndef INC_15618_FINAL_PROJECT_DEBUG_H
#define INC_15618_FINAL_PROJECT_DEBUG_H

// #define DEBUG
#define VALID_CHECK
#define WORKSTEAL
#ifdef DEBUG
#define DBG_PRINTF(format, ...) printf(format, ##__VA_ARGS__);
#else
#define DBG_PRINTF(format, ...)
#endif


#endif //INC_15618_FINAL_PROJECT_DEBUG_H
