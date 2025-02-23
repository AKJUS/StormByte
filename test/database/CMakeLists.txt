add_executable(SQLiteTests sqlite_test.cxx)
target_link_libraries(SQLiteTests StormByte)
add_test(NAME SQLiteTests COMMAND SQLiteTests)