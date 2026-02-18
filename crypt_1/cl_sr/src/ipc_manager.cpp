#include"../inc/ipc_manager.hpp"

#include <sys/mman.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <cstring>

ipc_manager::ipc_manager(mode m) : _mode(m)
{
    const char* key_file = "/tmp/rc4_protocol_key";

    int fd = open(key_file, O_CREAT | O_RDWR, 0666);
    if (fd == -1)
    {
        throw std::runtime_error("open key file failed: " + std::string(strerror(errno)));
    }
    close(fd);

    key_t key = ftok(key_file, 'R');
    if (key == -1)
    {
        throw std::runtime_error("ftok failed: " + std::string(strerror(errno)));
    }

    size_t shm_size = sizeof(shared_memory_layout);

    if (_mode == mode::SERVER)
    {
        _shm_id = shmget(key, shm_size, IPC_CREAT | IPC_EXCL | 0666);
        if (_shm_id == -1 && errno == EEXIST)
        {
            int old_id = shmget(key, 0, 0);
            if (old_id != -1)
            {
                std::cout << "[SERVER] Ghost memory detected. Forcing cleanup...\n";
                shmctl(old_id, IPC_RMID, nullptr);
                _shm_id = shmget(key, shm_size, IPC_CREAT | IPC_EXCL | 0666);
            }
        }

        if (_shm_id == -1)
        {
            throw std::runtime_error("CRITICAL: Server lock failed: " + std::string(strerror(errno)));
        }

        _layout = reinterpret_cast<shared_memory_layout*>(shmat(_shm_id, nullptr, 0));

        std::memset(_layout, 0, shm_size);

        if (sem_init(&_layout->global_mutex, 1, 1) == -1)
        {
            throw std::runtime_error("sem_init global_mutex failed: " + std::string(strerror(errno)));
        }

        for (int i = 0; i < MAX_SESSIONS; ++i)
        {
            auto& slot = _layout->slots[i];
            slot.status = session_status::FREE;
            slot.owner_pid = 0;
            slot.key_len = 0;
            slot.data_len = 0;

            if (sem_init(&slot.sem_server, 1, 0) == -1)
            {
                throw std::runtime_error("sem_init sem_server[" + std::to_string(i) + "] failed: " + std::string(strerror(errno)));
            }
            if (sem_init(&slot.sem_client, 1, 0) == -1)
            {
                throw std::runtime_error("sem_init sem_client[" + std::to_string(i) + "] failed: " + std::string(strerror(errno)));
            }
        }

        std::cout << "[SERVER] IPC resources initialized from scratch.\n";
    }
    else
    {
        _shm_id = shmget(key, 0, 0666);
        if (_shm_id == -1)
        {
            throw std::runtime_error("shmget (client) failed - server not running? " + std::string(strerror(errno)));
        }

        _layout = reinterpret_cast<shared_memory_layout*>(shmat(_shm_id, nullptr, 0));
        if (_layout == reinterpret_cast<shared_memory_layout*>(-1))
        {
            throw std::runtime_error("shmat (client) failed: " + std::string(strerror(errno)));
        }

        std::cout << "[CLIENT] Attached to shared memory successfully.\n";
    }
}

ipc_manager::~ipc_manager()
{
    if (_mode == mode::SERVER && _shm_id != -1)
    {
        if(_layout != nullptr && _layout != (void*)-1)
        {
            if(sem_destroy(&_layout->global_mutex) == -1)
            {
                std::cerr << "[SERVER] sem_destroy global_mutex failed: " << strerror(errno) << "\n";
            }

            for(int i = 0; i < MAX_SESSIONS; ++i)
            {
                if (sem_destroy(&_layout->slots[i].sem_server) == -1 ||
                    sem_destroy(&_layout->slots[i].sem_client) == -1)
                {
                    std::cerr << "[SERVER] sem_destroy slot " << i << " failed: " << strerror(errno) << "\n";
                }
            }
        }

        if (_layout != (void*)-1 && shmdt(_layout) == -1)
        {
            std::cerr << "[SERVER] shmdt failed: " << strerror(errno) << "\n";
        }

        if (shmctl(_shm_id, IPC_RMID, nullptr) == -1)
        {
            if (errno != EIDRM && errno != ENOENT)
            {
                std::cerr << "[SERVER] shmctl IPC_RMID failed: " << strerror(errno) << "\n";
            }
        }

        std::cout << "[SERVER] IPC resources destroyed. Memory cleaned up.\n";
    }
    else if (_mode == mode::CLIENT)
    {
        if (_layout != (void*)-1 && shmdt(_layout) == -1)
        {
            std::cerr << "[CLIENT] shmdt failed: " << strerror(errno) << "\n";
        }

        std::cout << "[CLIENT] Disconnected from shared memory.\n";
    }
    _layout = nullptr;
    _shm_id = -1;
}

key_t ipc_manager::generate_key() const
{
    const char* key_file = "/tmp/rc4_protocol_key";
    key_t key = ftok(key_file, 'R');
    if (key == -1)
    {
        throw std::runtime_error("ftok failed: " + std::string(strerror(errno)));
    }
    return key;
}
int ipc_manager::find_free_slot()
{
    if(_mode != mode::CLIENT) return -1;
    lock_mutex();
    for (int i = 0; i < MAX_SESSIONS; ++i)
    {
        if (_layout->slots[i].status == session_status::FREE)
        {
            _layout->slots[i].status = session_status::BUSY;
            _layout->slots[i].owner_pid = getpid();

            while (sem_trywait(&_layout->slots[i].sem_server) == 0);
            while (sem_trywait(&_layout->slots[i].sem_client) == 0);

            unlock_mutex();
            return i;
        }
    }
    unlock_mutex();
    return -1;
}

void ipc_manager::free_slot(int slot_idx)
{
    if (slot_idx < 0 || slot_idx >= MAX_SESSIONS) return;
    lock_mutex();
    _layout->slots[slot_idx].status = session_status::FREE;
    _layout->slots[slot_idx].owner_pid = 0;
    unlock_mutex();
}

inline void safe_sem_wait(sem_t* sem, const char* context = "sem_wait") {
    while (sem_wait(sem) == -1)
    {
        if (errno == EINTR) continue;
        throw std::runtime_error(std::string(context) + " failed: " + strerror(errno));
    }
}

inline void safe_sem_post(sem_t* sem, const char* context = "sem_post")
{
    if (sem_post(sem) == -1) {
        std::cerr << context << " failed: " << strerror(errno) << "\n";
    }
}

inline void safe_sem_trywait(sem_t* sem, const char* context = "sem_trywait")
{
    if (sem_trywait(sem) == -1)
    {
        if (errno != EAGAIN)
        {
            throw std::runtime_error(std::string(context) + " failed (not EAGAIN): " + strerror(errno));
        }
    }
}

inline int safe_sem_getvalue(sem_t* sem, int* value, const char* context = "sem_getvalue")
{
    if (sem_getvalue(sem, value) == -1)
    {
        throw std::runtime_error(std::string(context) + " failed: " + strerror(errno));
    }
    return *value;
}

void ipc_manager::signal_server(int slot_idx)
{
    safe_sem_post(&_layout->slots[slot_idx].sem_server, "signal_server");
}

void ipc_manager::wait_client_signal(int slot_idx)
{
    safe_sem_wait(&_layout->slots[slot_idx].sem_server, "wait_client_signal");
}

void ipc_manager::signal_client(int slot_idx)
{
    safe_sem_post(&_layout->slots[slot_idx].sem_client, "signal_client");
}

void ipc_manager::wait_server_signal(int slot_idx)
{
    safe_sem_wait(&_layout->slots[slot_idx].sem_client, "wait_server_signal");
}


void ipc_manager::lock_mutex()
{
    safe_sem_wait(&_layout->global_mutex, "lock_mutex");
}

void ipc_manager::unlock_mutex()
{
    safe_sem_post(&_layout->global_mutex, "unlock_mutex");
}



