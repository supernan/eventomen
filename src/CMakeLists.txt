FILE(GLOB SOURCE_1 "${PROJECT_ROOT_PATH}/src/*.cpp")
FILE(GLOB SOURCE_2 "${PROJECT_ROOT_PATH}/thirdparty/src/*.cpp")
ADD_LIBRARY(eventomen STATIC ${SOURCE_1} ${SOURCE_2} )
TARGET_LINK_LIBRARIES(eventomen classifier pthread glog ICTCLAS50 Linear blas)
