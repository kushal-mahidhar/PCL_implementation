add_executable(PCL_Implementation main.cpp)
include_directories(
    ${CMAKE_SOURCE_DIR}/include
)

add_definitions(${PCL_DEFINITIONS})
# link_directories(${PCL_LIBRARY_DIRS})

target_include_directories(PCL_Implementation PUBLIC ${CMAKE_SOURCE_DIR}/include
						${OpenCV_INCLUDE_DIRS}
						${PCL_INCLUDE_DIRS}
						)

target_link_libraries(PCL_Implementation ${OpenCV_LIBS} ${PCL_LIBRARIES} ${Boost_LIBRARIES})

# target_link_libraries(PCL_Implementation ${PCL_LIBRARIES} ${Boost_LIBRARIES})