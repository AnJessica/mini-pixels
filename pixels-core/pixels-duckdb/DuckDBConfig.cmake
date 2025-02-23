# Config file for DuckDB package
# It defines the following variables
#
#  DuckDB_INCLUDE_DIRS - include directories for DuckDB
#  DuckDB_LIBRARIES    - libraries to link against

# Compute paths
get_filename_component(DuckDB_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
set(DuckDB_INCLUDE_DIRS "/home/nika/mini-pixels/pixels-duckdb/src/include")

if(NOT TARGET duckdb AND NOT DuckDB_BINARY_DIR)
    include("${DuckDB_CMAKE_DIR}/DuckDBExports.cmake")
endif()

if(DuckDB_USE_STATIC_LIBS)
    set(DuckDB_LIBRARIES duckdb_static)
else()
    set(DuckDB_LIBRARIES duckdb)
endif()
