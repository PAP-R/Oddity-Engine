#include "Syncable.h"

#include <Util/Debug.h>

namespace OddityEngine {
    void Networking::create_lobby() {
        auto call = SteamMatchmaking()->CreateLobby(k_ELobbyTypeFriendsOnly, 4);
        m_create_lobby_call_result.Set(call, this, &Networking::on_create_lobby);
        Debug::message("Creating lobby");
    }

    void Networking::get_lobby_list() {
        auto call = SteamMatchmaking()->RequestLobbyList();
        m_get_lobby_list_call_result.Set(call, this, &Networking::on_get_lobby_list);
        Debug::message("Getting lobby list");
    }

    void Networking::on_create_lobby(LobbyCreated_t *pCallback, bool bIOFailure) {
        if (bIOFailure || pCallback->m_eResult != k_EResultOK) {
            Debug::message("Failed to create lobby");
        }
        else {
            Debug::message("Created lobby {}", pCallback->m_ulSteamIDLobby);
        }
    }

    void Networking::on_get_lobby_list(LobbyMatchList_t *pCallback, bool bIOFailure) {
        Debug::message("Got {} lobbies", pCallback->m_nLobbiesMatching);
        // for (size_t i = 0; i < pCallback->m_nLobbiesMatching; i++) {
        //     auto lobby = SteamMatchmaking()->GetLobbyByIndex(i);
        //     Debug::message("Lobby {}", i);
        // }
    }

    void Networking::SessionRequest(SteamNetworkingMessagesSessionRequest_t *pParam) {
        auto ip = pParam->m_identityRemote.m_ip.m_ipv4.m_ip;
        Debug::message("Session request by {}.{}.{}.{}", ip[0], ip[1], ip[2], ip[3]);
    }

    void Networking::LobbyDataUpdate(LobbyDataUpdate_t *pParam) {
        Debug::message("{} changed", pParam->m_ulSteamIDMember);
    }


    void Syncable::update() {

    }
}