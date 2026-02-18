//
// Created by mvideo on 14/02/26.
//

#ifndef CRYPT_1_PROTOCOL_HPP
#define CRYPT_1_PROTOCOL_HPP

#include <cstdint>
#include <sys/types.h>
#include <semaphore.h>

const int MAX_SESSIONS = 2500;

const size_t SHM_BUFFER_SIZE = 65536;

enum class session_status : uint8_t {
    FREE = 0,
    BUSY,
    DATA_READY_SERVER,
    DATA_READY_CLIENT,
    FINALIZE,
    ERR_STATE
};

#pragma pack(push, 1)
struct session_slot
{
    session_status status = session_status::FREE;

    pid_t owner_pid = 0;

    sem_t sem_server = {};
    sem_t sem_client = {};

    uint8_t key[256] = {};
    size_t key_len = 0;
    size_t data_len = 0;
    uint8_t buffer[SHM_BUFFER_SIZE] = {};
};

struct shared_memory_layout
{
    sem_t global_mutex;
    session_slot slots[MAX_SESSIONS];
};
#pragma pack(pop)



#endif //CRYPT_1_PROTOCOL_HPP
