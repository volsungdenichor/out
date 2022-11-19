set(TARGET_NAME tests)

set(UNIT_TEST_SOURCE_LIST
  unit_tests.cpp
)

add_executable(${TARGET_NAME} main.cpp ${UNIT_TEST_SOURCE_LIST})

include_directories(
  "${PROJECT_SOURCE_DIR}/include")

add_test(
  NAME ${TARGET_NAME}
  COMMAND ${TARGET_NAME} -o report.xml -r junit)
