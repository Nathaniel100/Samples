

include_directories(..)


add_executable(AllocatorTest allocator_test.cpp ../stl_allocator.h)

add_executable(VectorTest vector_test.cpp ../stl_allocator.h ../stl_vector.h)