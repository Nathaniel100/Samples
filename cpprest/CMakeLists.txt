

add_executable(BingSearch bing_search.cpp)
target_link_libraries(BingSearch ${Boost_LIBRARIES})
target_link_libraries(BingSearch cpprest)