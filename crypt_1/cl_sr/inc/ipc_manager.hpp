//
// Created by mvideo on 14/02/26.
//

#ifndef CRYPT_1_IPC_MANAGER_HPP
#define CRYPT_1_IPC_MANAGER_HPP

#include "protocol.hpp"
#include "rc4_cipher.hpp"
#include <vector>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdexcept>

class ipc_manager final
{
public:
    enum class mode {SERVER, CLIENT};
    explicit ipc_manager(mode _mode);
    ~ipc_manager();

    ipc_manager(const ipc_manager&) = delete;
    ipc_manager& operator=(const ipc_manager&) = delete;

private:
    mode _mode;
    int _shm_id = -1;
    shared_memory_layout* _layout = nullptr;

    key_t generate_key() const;

public:
    int find_free_slot();
    void free_slot(int slot_idx);

    void lock_mutex();
    void unlock_mutex();

    void signal_server(int slot_idx);
    void wait_client_signal(int slot_idx);

    void signal_client(int slot_idx);
    void wait_server_signal(int slot_idx);

    shared_memory_layout* get_layout() { return _layout; }
};


#endif //CRYPT_1_IPC_MANAGER_HPP
