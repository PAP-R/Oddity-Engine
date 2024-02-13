include_directories(
        public/steam
)

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(STEAM_LIBRARY
            ${CMAKE_CURRENT_SOURCE_DIR}/redistributable_bin/linux64/libsteam_api.so
            PARENT_SCOPE
    )
    FILE(INSTALL redistributable_bin/win64/steam_api64.dll DESTINATION ${OUTPUT_DIR})
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(STEAM_LIBRARY
            ${CMAKE_CURRENT_SOURCE_DIR}/redistributable_bin/win64/steam_api64.lib
            PARENT_SCOPE
    )
    FILE(INSTALL redistributable_bin/win64/steam_api64.dll DESTINATION ${OUTPUT_DIR})
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(STEAM_LIBRARY
            ${CMAKE_CURRENT_SOURCE_DIR}/redistributable_bin/osx/libsteam_api.dylib
            PARENT_SCOPE
    )
    FILE(INSTALL redistributable_bin/win64/steam_api64.dll DESTINATION ${OUTPUT_DIR})
endif ()

FILE(INSTALL steam_appid.txt DESTINATION ${OUTPUT_DIR})#TODO Remove at launch