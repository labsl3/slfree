
include_directories(
	${CMAKE_CURRENT_SOURCE_DIR} 
	"../include/" 
)

file(GLOB CXX_FILES "*.cc")

set(CXX_FILES 
  ${CXX_FILES}
)

add_executable( ${PROJECT_NAME} ${CXX_FILES} )
target_link_libraries( ${PROJECT_NAME} "" )
