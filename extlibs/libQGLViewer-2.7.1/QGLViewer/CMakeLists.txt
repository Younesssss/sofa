cmake_minimum_required(VERSION 3.1)
project(QGLViewer)

set(HEADER_FILES
    camera.h
    frame.h
    keyFrameInterpolator.h
    manipulatedCameraFrame.h
    manipulatedFrame.h
    qglviewer.h
)

set(SOURCE_FILES
    VRender/BSPSortMethod.cpp
    VRender/BackFaceCullingOptimizer.cpp
    VRender/EPSExporter.cpp
    VRender/Exporter.cpp
    VRender/FIGExporter.cpp
    VRender/NVector3.cpp
    VRender/ParserGL.cpp
    VRender/Primitive.cpp
    VRender/PrimitivePositioning.cpp
    VRender/TopologicalSortMethod.cpp
    VRender/VRender.cpp
    VRender/Vector2.cpp
    VRender/Vector3.cpp
    VRender/VisibilityOptimizer.cpp
    VRender/gpc.cpp
    camera.cpp
    constraint.cpp
    frame.cpp
    keyFrameInterpolator.cpp
    manipulatedCameraFrame.cpp
    manipulatedFrame.cpp
    mouseGrabber.cpp
    qglviewer.cpp
    quaternion.cpp
    saveSnapshot.cpp
    vec.cpp
)

set(UI_FILES
    ImageInterface.ui
    VRenderInterface.ui
)

find_package(OpenGL REQUIRED)
find_package(Qt5 COMPONENTS Core QUIET)

if(Qt5Core_FOUND)
    message("QGLViewer: Using Qt5")
    find_package(Qt5 COMPONENTS Gui Xml OpenGL Widgets REQUIRED)

    set(QT_TARGETS Qt5::Core Qt5::Gui Qt5::Xml Qt5::OpenGL Qt5::Widgets)
    set(EXT_QT_INCLUDES ${Qt5Core_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS} ${Qt5OpenGL_INCLUDE_DIRS} ${Qt5Xml_INCLUDE_DIRS})
    set(EXT_QT_LIBRARIES ${Qt5Core_LIBRARIES} ${Qt5Widgets_LIBRARIES} ${Qt5OpenGL_LIBRARIES} ${Qt5Xml_LIBRARIES})

    qt5_wrap_cpp(MOC_FILES ${HEADER_FILES})
    qt5_wrap_ui(FORM_FILES ${UI_FILES})

    sofa_install_libraries(TARGETS ${QT_TARGETS})

    # QT INSTALL (dirty)
    # Get Qt libraries dir
    get_target_property(QtCore_location Qt5::Core LOCATION)
    get_filename_component(QT_LIBRARIES_DIR ${QtCore_location} DIRECTORY)
    # Get Qt plugins dir
    list(GET Qt5Gui_PLUGINS 1 QT_PLUGIN_ANY)
    if(TARGET ${QT_PLUGIN_ANY})
        get_target_property(QT_PLUGIN_ANY_LOCATION ${QT_PLUGIN_ANY} LOCATION)
        get_filename_component(QT_PLUGINS_SUBDIR ${QT_PLUGIN_ANY_LOCATION} DIRECTORY)
        get_filename_component(QT_PLUGINS_DIR ${QT_PLUGINS_SUBDIR} DIRECTORY)
    else()
        # Try to guess QT_PLUGINS_DIR
        if(UNIX AND NOT APPLE AND EXISTS "${QT_LIBRARIES_DIR}/qt5/plugins")
            set(QT_PLUGINS_DIR "${QT_LIBRARIES_DIR}/qt5/plugins")
        endif()
    endif()
    # Install Qt plugins
    if(EXISTS "${QT_PLUGINS_DIR}/iconengines")
        install(DIRECTORY "${QT_PLUGINS_DIR}/iconengines" DESTINATION "bin" COMPONENT applications)
    endif()
    if(EXISTS "${QT_PLUGINS_DIR}/imageformats")
        install(DIRECTORY "${QT_PLUGINS_DIR}/imageformats" DESTINATION "bin" COMPONENT applications)
    endif()
    if(EXISTS "${QT_PLUGINS_DIR}/platforms")
        install(DIRECTORY "${QT_PLUGINS_DIR}/platforms" DESTINATION "bin" COMPONENT applications)
    endif()
    if(EXISTS "${QT_PLUGINS_DIR}/styles")
        install(DIRECTORY "${QT_PLUGINS_DIR}/styles" DESTINATION "bin" COMPONENT applications)
    endif()
    # Install Qt XCB libraries
    file(GLOB QT_XCB_LIBRARIES
        "${QT_LIBRARIES_DIR}/libQt5DBus${CMAKE_SHARED_LIBRARY_SUFFIX}"
        "${QT_LIBRARIES_DIR}/libQt5XcbQpa${CMAKE_SHARED_LIBRARY_SUFFIX}"
        )
    sofa_install_libraries(LIBRARIES ${QT_XCB_LIBRARIES})
    # Install Qt ICU libraries
    file(GLOB QT_ICU_LIBRARIES
        "${QT_LIBRARIES_DIR}/libicudata${CMAKE_SHARED_LIBRARY_SUFFIX}"
        "${QT_LIBRARIES_DIR}/libicui18n${CMAKE_SHARED_LIBRARY_SUFFIX}"
        "${QT_LIBRARIES_DIR}/libicuuc${CMAKE_SHARED_LIBRARY_SUFFIX}"
        )
    sofa_install_libraries(LIBRARIES ${QT_ICU_LIBRARIES})
endif()

# These header files don't need to be processed by the moc.
list(APPEND HEADER_FILES
    VRender/AxisAlignedBox.h
    VRender/Exporter.h
    VRender/NVector3.h
    VRender/Optimizer.h
    VRender/ParserGL.h
    VRender/Primitive.h
    VRender/PrimitivePositioning.h
    VRender/SortMethod.h
    VRender/Types.h
    VRender/VRender.h
    VRender/Vector2.h
    VRender/Vector3.h
    VRender/gpc.h
    config.h
    constraint.h
    domUtils.h
    mouseGrabber.h
    quaternion.h
    vec.h
)

set(QGLVIEWER_VERSION 2.7.1)

add_library(${PROJECT_NAME} SHARED ${HEADER_FILES} ${MOC_FILES} ${FORM_FILES} ${SOURCE_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "SofaExtlibs")
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${EXT_QT_LIBRARIES})

# In CMake 2.8.11 and later versions, this command automatically adds appropriate include directories, compile definitions, the position-independent-code flag, and links to the qtmain.lib library on Windows.
# target_include_directories(${PROJECT_NAME} PRIVATE ${EXT_QT_INCLUDES})

# Contains the files generated by the Qt MOC.
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>")
target_include_directories(${PROJECT_NAME} PUBLIC "$<INSTALL_INTERFACE:include/extlibs/${PROJECT_NAME}>")

if(WIN32)
    target_compile_options(${PROJECT_NAME} PRIVATE "-DCREATE_QGLVIEWER_DLL")
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "_d")
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${QGLVIEWER_VERSION})
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${HEADER_FILES}")

include(${SOFA_KERNEL_SOURCE_DIR}/SofaFramework/SofaMacros.cmake)
sofa_generate_package(NAME ${PROJECT_NAME} VERSION ${QGLVIEWER_VERSION} TARGETS ${PROJECT_NAME} INCLUDE_INSTALL_DIR "extlibs/${PROJECT_NAME}/${PROJECT_NAME}")
