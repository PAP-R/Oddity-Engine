set(STEAM_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})

include_directories(
        public/steam
)

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(STEAM_LIBRARIES
            ${STEAM_DIRECTORY}/redistributable_bin/linux64/libsteam_api.so
    )

    FILE(INSTALL ${STEAM_DIRECTORY}/redistributable_bin/win64/steam_api64.dll DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

    add_compile_definitions(ODDITY_ENGINE_STEAM)
    message("Steam for Linux")

elseif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(STEAM_LIBRARIES
            ${STEAM_DIRECTORY}/redistributable_bin/win64/steam_api64.lib
    )

    FILE(INSTALL ${STEAM_DIRECTORY}/redistributable_bin/win64/steam_api64.dll DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

    add_compile_definitions(ODDITY_ENGINE_STEAM)
    message("Steam for Windows")

elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(STEAM_LIBRARIES
            ${STEAM_DIRECTORY}/redistributable_bin/osx/libsteam_api.dylib
    )

    FILE(INSTALL ${STEAM_DIRECTORY}/redistributable_bin/win64/steam_api64.dll DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

    add_compile_definitions(ODDITY_ENGINE_STEAM)
    message("Steam for Mac")

endif ()

FILE(INSTALL ${STEAM_DIRECTORY}/steam_appid.txt DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}) #TODO Remove at launch
