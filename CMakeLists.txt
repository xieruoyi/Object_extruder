
#add_executable(use_fftw use_fftw.cpp)
#target_link_libraries(use_fftw FFTW)

add_library(use_boost use_boost.cpp)
target_link_libraries(use_boost Boost)