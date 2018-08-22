# Include the directory itself as a path to include directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
  
#Create a variable called linalg_SOURCES containing all .c files:

set(testComm_Src 
    src/testComm.c
    src/do_work.c
    PARENT_SCOPE
    )

