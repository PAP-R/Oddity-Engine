include_directories(
        public/steam
)

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(STEAM_LIBRARIES
            ${CMAKE_CURRENT_LIST_DIR}/redistributable_bin/linux64/libsteam_api.so
    )

    FILE(INSTALL ${CMAKE_CURRENT_LIST_DIR}/redistributable_bin/win64/steam_api64.dll DESTINATION ${OUTPUT_DIR})

    add_compile_definitions(ODDITY_ENGINE_STEAM)
    message("Steam for Linux")

elseif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(STEAM_LIBRARIES
            ${CMAKE_CURRENT_LIST_DIR}/redistributable_bin/win64/steam_api64.lib
    )

    FILE(INSTALL ${CMAKE_CURRENT_LIST_DIR}/redistributable_bin/win64/steam_api64.dll DESTINATION ${OUTPUT_DIR})

    add_compile_definitions(ODDITY_ENGINE_STEAM)
    message("Steam for Windows")

elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(STEAM_LIBRARIES
            ${CMAKE_CURRENT_LIST_DIR}/redistributable_bin/osx/libsteam_api.dylib
    )

    FILE(INSTALL ${CMAKE_CURRENT_LIST_DIR}/redistributable_bin/win64/steam_api64.dll DESTINATION ${OUTPUT_DIR})

    add_compile_definitions(ODDITY_ENGINE_STEAM)
    message("Steam for Mac")

endif ()

FILE(INSTALL ${CMAKE_CURRENT_LIST_DIR}/steam_appid.txt DESTINATION ${OUTPUT_DIR}) #TODO Remove at launch