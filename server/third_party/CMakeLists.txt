set(PROJECT_FILE log.c)
add_library(log ${PROJECT_FILE})
add_sanitizers(log)

