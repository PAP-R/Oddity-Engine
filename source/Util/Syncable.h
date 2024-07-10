#ifndef SYNCABLE_H
#define SYNCABLE_H

#include <steam_api.h>

#include "Updateable.h"

namespace OddityEngine {
    class Networking {
        STEAM_CALLBACK(Networking, SessionRequest, SteamNetworkingMessagesSessionRequest_t);
        STEAM_CALLBACK(Networking, LobbyDataUpdate, LobbyDataUpdate_t);

        void on_create_lobby(LobbyCreated_t* pCallback, bool bIOFailure);
        CCallResult<Networking, LobbyCreated_t> m_create_lobby_call_result;

        void on_get_lobby_list(LobbyMatchList_t* pCallback, bool bIOFailure);
        CCallResult<Networking, LobbyMatchList_t> m_get_lobby_list_call_result;
     public:
        void create_lobby();
        void get_lobby_list();
    };


    class Syncable : public Updateable {
    public:
        void update() override;
    };
}

#endif //SYNCABLE_H
