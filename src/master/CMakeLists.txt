project(RDDMaster)

set(MASTER_SOURCE_FILES
        rdd_context.cc
        rdd_stub.cc
        key_value_rdd_stub.cc
        key_values_rdd_stub.cc
        text_file_rdd_stub.cc)

add_library(RDDMaster STATIC ${MASTER_SOURCE_FILES})

target_link_libraries(RDDMaster msgpack jubatus_mpio jubatus_msgpack-rpc)