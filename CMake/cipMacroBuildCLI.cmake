# SlicerExecutionModel
find_package(SlicerExecutionModel NO_MODULE REQUIRED GenerateCLP)
include(${SlicerExecutionModel_USE_FILE})
include(CIP_AddTests)

#FIND_PACKAGE( CIP REQUIRED )

macro(cipMacroBuildCLI)
  set(options
    NO_INSTALL VERBOSE
  )
  
  set (oneValueArgs
     NAME 
     LOGO_HEADER
  )
  
  set(multiValueArgs
    ADDITIONAL_TARGET_LIBRARIES
    ADDITIONAL_INCLUDE_DIRECTORIES
    SRCS
  )
  
  CMAKE_PARSE_ARGUMENTS(MY_CIP
    "${options}"
    "${oneValueArgs}"
    "${multiValueArgs}"  
    ${ARGN}
    )

  set ( INCLUDE_DIRECTORIES
    ${MY_CIP_ADDITIONAL_INCLUDE_DIRECTORIES}
    ${CIP_INCLUDE_DIRECTORIES}
   )
   
 
  set(TARGET_LIBRARIES
  ${MY_CIP_ADDITIONAL_TARGET_LIBRARIES}
  ${ITK_LIBRARIES}
  ${CIP_LIBRARIES}
  )

  
  if(${CIP_BUILD_CLI_EXECUTABLEONLY})
       set(PASS_EXECUTABLE_ONLY EXECUTABLE_ONLY)
  endif()
  
  SEMMacroBuildCLI(
       NAME ${MODULE_NAME}
       LOGO_HEADER ${MY_CIP_LOGO_HEADER}
       TARGET_LIBRARIES ${TARGET_LIBRARIES}
       INCLUDE_DIRECTORIES ${INCLUDE_DIRECTORIES}
       ADDITIONAL_SRCS ${SRCS}
       LIBRARY_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH}
       RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}
       ARCHIVE_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH}
       INSTALL_RUNTIME_DESTINATION ${CIP_CLI_INSTALL_RUNTIME_DESTINATION}
       INSTALL_LIBRARY_DESTINATION ${CIP_CLI_INSTALL_LIBRARY_DESTINATION}
       INSTALL_ARCHIVE_DESTINATION ${CIP_CLI_INSTALL_ARCHIVE_DESTINATION}
       ${PASS_EXECUTABLE_ONLY}
  )  
 
 
  if(${BUILD_TESTING})  
  	SET (INCLUDE_DIRECTORIES
  	      ${INCLUDE_DIRECTORIES}
  	      ${CIP_SOURCE_DIR}/CommandLineTools/Testing
  	 )
  	# Default directories for input and ouput data for the tests
  	SET (INPUT_DATA_DIR ${CIP_SOURCE_DIR}/Testing/Data/Input)      # Input files
  	SET (BASELINE_DATA_DIR ${CIP_SOURCE_DIR}/CommandLineTools/${MODULE_NAME}/Data/Baseline)    # Expected output files
	SET (BASELINE_DATA_DIR_LEGACY ${CIP_SOURCE_DIR}/CommandLineTools/LegacyCLIs/${MODULE_NAME}/Data/Baseline)    # Expected output files	
        SET (OUTPUT_DATA_DIR ${CIP_BINARY_DIR}/CommandLineTools/Testing/Output)                    # Testing output files  	
  		
  	FILE(MAKE_DIRECTORY "${BASELINE_DATA_DIR}")
    FILE(MAKE_DIRECTORY "${OUTPUT_DATA_DIR}")

  	INCLUDE_DIRECTORIES(${INCLUDE_DIRECTORIES})
  	ADD_EXECUTABLE(${MODULE_NAME}Test ./Testing/${MODULE_NAME}Test.cxx) 
  	TARGET_LINK_LIBRARIES(${MODULE_NAME}Test ${MODULE_NAME}Lib ${TARGET_LIBRARIES})
  	SET_TARGET_PROPERTIES(${MODULE_NAME}Test PROPERTIES LABELS ${MODULE_NAME} RUNTIME_OUTPUT_DIRECTORY ${CIP_BINARY_DIR}/CommandLineTools/Testing/bin)
  endif()
 
 
endmacro()






  

