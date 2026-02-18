#include "../../inc/ipc_manager.hpp"
#include <thread>
#include <vector>
#include <csignal>
#include <atomic>
#include<iostream>

std::atomic<bool> server_running(true);

void signal_handler(int)
{
    std::cout << "\n[SERVER] Shutdown signal received. Exiting...\n";
    server_running = false;
}

void worker(int slot_idx, ipc_manager* mgr)
{
    auto slot = mgr->get_layout()->slots + slot_idx;
    rc4_cipher cipher;
    bool key_initialized = false;

    while (server_running)
    {
        try
        {
            mgr->wait_client_signal(slot_idx);
            if (!server_running) break;

            if (slot->status == session_status::BUSY)
            {
                if (!key_initialized)
                {
                    std::vector<uint8_t> key_vec(slot->key, slot->key + slot->key_len);
                    cipher.set_key(key_vec);
                    key_initialized = true;
                    std::cout << "[WORKER " << slot_idx << "] Key initialized" << std::endl;

                    slot->status = session_status::DATA_READY_CLIENT;
                    mgr->signal_client(slot_idx);
                }
                continue;
            }

            if (slot->status == session_status::DATA_READY_SERVER)
            {
                if (!key_initialized)
                {
                    slot->status = session_status::ERR_STATE;
                    mgr->signal_client(slot_idx);
                    continue;
                }
                cipher.process(slot->buffer, slot->data_len);
                slot->status = session_status::DATA_READY_CLIENT;
                mgr->signal_client(slot_idx);
                continue;
            }
            if (slot->status == session_status::FINALIZE)
            {
                key_initialized = false;
                mgr->signal_client(slot_idx);
                continue;
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << "[WORKER " << slot_idx << "] Fatal error: " << e.what() << "\n";
            break;
        }
    }
}

int main()
{
    std::signal(SIGINT, signal_handler);
    try
    {
        ipc_manager mgr(ipc_manager::mode::SERVER);
        std::vector<std::thread> workers;
        workers.reserve(MAX_SESSIONS);

        for (int i = 0; i < MAX_SESSIONS; ++i)
        {
            workers.emplace_back(worker, i, &mgr);
        }

        std::cout << "[SERVER] Ready for 100 parallel sessions.\n";
        std::cout << "[SERVER] Press ENTER to shutdown gracefully...\n";

        while (server_running)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }

        std::cout << "[SERVER] Waking up workers for termination...\n";

        for (int i = 0; i < MAX_SESSIONS; ++i)
        {
            mgr.signal_server(i);
        }

        for (auto& t : workers)
        {
            if (t.joinable()) t.join();
        }

        std::cout << "[SERVER] All workers stopped. Cleaning up IPC resources...\n";


    } catch (const std::exception& e)
    {
        std::cerr << "Server fatal error: " << e.what() << "\n";
    }
    return 0;
}