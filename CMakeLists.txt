cmake_minimum_required(VERSION 3.28)

project(linked_list LANGUAGES CXX)

add_executable(linked_list
    
    main.cpp
    
)

target_sources(linked_list
    PUBLIC
        FILE_SET CXX_MODULES
        FILES
            linked_list.cxx
    )

target_compile_features(linked_list PUBLIC cxx_std_23)
