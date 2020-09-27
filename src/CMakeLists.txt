add_executable(PCL_Implementation main.cpp cloud.cpp)
add_executable(trypcl trypcl.cpp)

include_directories(
    ${CMAKE_SOURCE_DIR}/include
)

include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})

target_include_directories(PCL_Implementation PUBLIC ${CMAKE_SOURCE_DIR}/include
						${OpenCV_INCLUDE_DIRS}
						${PCL_INCLUDE_DIRS}
						)

target_link_libraries (PCL_Implementation ${OpenCV_LIBS} ${PCL_LIBRARIES} ${Boost_LIBRARIES})
target_link_libraries (trypcl ${PCL_LIBRARIES})

# target_link_libraries(PCL_Implementation ${PCL_LIBRARIES} ${Boost_LIBRARIES})