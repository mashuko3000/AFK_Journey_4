#include "../../inc/ipc_manager.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <stdexcept>

bool compare_files(const std::string& p1, const std::string& p2)
{
    std::ifstream f1(p1, std::ios::binary), f2(p2, std::ios::binary);
    return std::equal(std::istreambuf_iterator<char>(f1), std::istreambuf_iterator<char>(),std::istreambuf_iterator<char>(f2));
}

void encrypt_file(const std::string& input_path, const std::string& output_path, const std::string& key_str)
{
    try {
        ipc_manager mgr(ipc_manager::mode::CLIENT);

        int slot_idx = mgr.find_free_slot();
        if(slot_idx == -1)
        {
            throw std::runtime_error("All server slots are busy. Try again later.");
        }

        auto& slot = mgr.get_layout()->slots[slot_idx];
        std::cout << "[CLIENT] Secured slot #" << slot_idx << "\n";

        if(key_str.empty())
        {
            throw std::runtime_error("Key cannot be empty");
        }
        slot.key_len = std::min(key_str.size(), size_t(256));
        memcpy(slot.key, key_str.data(), slot.key_len);
        std::cout << "[CLIENT] Key sent (len = " << slot.key_len << ")\n";

        slot.status = session_status::BUSY;
        mgr.signal_server(slot_idx);

        mgr.wait_server_signal(slot_idx);
        if (slot.status != session_status::DATA_READY_CLIENT)
        {
            throw std::runtime_error("Server did not acknowledge key");
        }

        std::cout << "[CLIENT] Key acknowledged by server\n";

        std::ifstream fin(input_path, std::ios::binary);
        std::ofstream fout(output_path, std::ios::binary);

        if (!fin || !fout)
        {
            throw std::runtime_error("Cannot open input/output file");
        }

        std::vector<char> buffer(SHM_BUFFER_SIZE);

        while (true)
        {
            fin.read(buffer.data(), SHM_BUFFER_SIZE);
            std::streamsize bytes_read = fin.gcount();

            if (bytes_read <= 0) break;

            memcpy(slot.buffer, buffer.data(), bytes_read);
            slot.data_len = bytes_read;
            slot.status = session_status::DATA_READY_SERVER;

            std::cout << "[CLIENT] Sending chunk (" << bytes_read << " bytes)\n";
            mgr.signal_server(slot_idx);

            mgr.wait_server_signal(slot_idx);

            if (slot.status != session_status::DATA_READY_CLIENT) {
                throw std::runtime_error("Server failed to process chunk");
            }

            fout.write(reinterpret_cast<const char*>(static_cast<const void*>(slot.buffer)), bytes_read);
        }


        std::cout << "[CLIENT] All chunks sent. Sending FINALIZE...\n";
        slot.status = session_status::FINALIZE;
        mgr.signal_server(slot_idx);

        mgr.wait_server_signal(slot_idx);

        mgr.free_slot(slot_idx);

        std::cout << "[CLIENT] Success! File processed.\n";
    }
    catch(const std::exception& e)
    {
        std::cerr << "[CLIENT ERROR] " << e.what() << "\n";
    }
}

int main(int argc, char* argv[])
{
    if (argc != 4)
    {
        std::cerr << "Usage: ./client <input_file> <output_file> <key>\n";
        std::cerr << "Example: ./client input.txt encrypted.bin mysecretkey\n";
        return 1;
    }

    encrypt_file(argv[1], argv[2], argv[3]);
    return 0;
}