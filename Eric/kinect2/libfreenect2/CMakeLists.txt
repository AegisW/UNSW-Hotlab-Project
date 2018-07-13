CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12.1)

if(WIN32 AND NOT MINGW)
  if(NOT DEFINED CMAKE_DEBUG_POSTFIX)
    set(CMAKE_DEBUG_POSTFIX "d")
  endif()
endif()

IF(NOT DEFINED CMAKE_BUILD_TYPE)
  # No effect for multi-configuration generators (e.g. for Visual Studio)
  SET(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Choose: RelWithDebInfo Release Debug MinSizeRel None")
ENDIF()

PROJECT(libfreenect2_tests)

SET(MY_DIR "/home/eric/libfreenect2/src")
SET(DEPENDS_DIR "/home/eric/libfreenect2/depends")

OPTION(ENABLE_OPENGL "Enable OpenGL support" ON)

# The example build system is standalone and will work out-of-tree with these files copied
SET(freenect2_ROOT_DIR ${MY_DIR}/..)
SET(flextGL_SOURCES ${freenect2_ROOT_DIR}/src/flextGL.cpp)
SET(flextGL_INCLUDE_DIRS ${freenect2_ROOT_DIR}/src) # for flextGL.h

FIND_PACKAGE(PkgConfig)    # try find PKGConfig as it will be used if found
LIST(APPEND CMAKE_MODULE_PATH ${freenect2_ROOT_DIR}/cmake_modules) # FindGLFW3.cmake

IF(TARGET freenect2)
  MESSAGE(STATUS "Using in-tree freenect2 target")
  SET(freenect2_LIBRARIES freenect2)
  SET(freenect2_DLLS ${LIBFREENECT2_DLLS})
ELSE()
  FIND_PACKAGE(freenect2 REQUIRED)
  # Out-of-tree build will have to have DLLs manually copied.
ENDIF()

find_package(OpenCV ${OCV_VERSION} COMPONENTS core highgui imgproc REQUIRED)

INCLUDE_DIRECTORIES(
  ${freenect2_INCLUDE_DIR}
  ${OpenCV_INCLUDE_DIRS}
)

SET(Test_src
  Test.cpp
)

SET(Test_LIBRARIES
  ${freenect2_LIBRARIES}
)

SET(Test_DLLS
  ${freenect2_DLLS}
)

IF(ENABLE_OPENGL)
  FIND_PACKAGE(GLFW3)
  FIND_PACKAGE(OpenGL)
  IF(GLFW3_FOUND AND OPENGL_FOUND)
    INCLUDE_DIRECTORIES(
      ${GLFW3_INCLUDE_DIRS}
      ${flextGL_INCLUDE_DIRS}
    )

    LIST(APPEND Test_DLLS ${GLFW3_DLL})
    LIST(APPEND Test_src
      viewer.cpp
      ${flextGL_SOURCES}
    )
    LIST(APPEND Test_LIBRARIES
      ${GLFW3_LIBRARIES}
      ${OPENGL_gl_LIBRARY}
    )
    ADD_DEFINITIONS(-DEXAMPLES_WITH_OPENGL_SUPPORT=1)
  ENDIF()
ENDIF(ENABLE_OPENGL)

ADD_EXECUTABLE(Test
  ${Test_src}
)

TARGET_LINK_LIBRARIES(Test
  ${Test_LIBRARIES}
  ${OpenCV_LIBRARIES}
)

IF(WIN32)
  INSTALL(TARGETS Protonect DESTINATION bin)
  LIST(REMOVE_DUPLICATES Protonect_DLLS)
  FOREACH(FILEI ${Test_DLLS})
    ADD_CUSTOM_COMMAND(TARGET Test POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${FILEI} $<TARGET_FILE_DIR:Test>
    )
  ENDFOREACH(FILEI)
  INSTALL(FILES ${Test_DLLS} DESTINATION bin)
ENDIF()
