
include_directories(
	${CMAKE_CURRENT_SOURCE_DIR} 
	"../include/" 
)

file(GLOB CXX_FILES "*.cc")

set(CXX_FILES 
  ${CXX_FILES}
)

add_library( ${PROJECT_NAME} ${CXX_FILES} )
