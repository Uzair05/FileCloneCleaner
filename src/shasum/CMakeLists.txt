find_package(OpenSSL REQUIRED)
add_library(${SHA_LIBRARY_NAME} STATIC shasum.cpp)
target_link_libraries(${SHA_LIBRARY_NAME} PRIVATE OpenSSL::Crypto)