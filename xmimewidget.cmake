include_directories(${CMAKE_CURRENT_LIST_DIR})

include(${CMAKE_CURRENT_LIST_DIR}/../XMIME/xmime.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../XShortcuts/xshortcuts.cmake)

set(XMIMEWIDGET_SOURCES
    ${XMIME_SOURCES}
    ${XSHORTCUTS_SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/dialogmime.ui
    ${CMAKE_CURRENT_LIST_DIR}/xmimewidget.ui
    ${CMAKE_CURRENT_LIST_DIR}/dialogmime.cpp
    ${CMAKE_CURRENT_LIST_DIR}/xmimewidget.cpp
)
