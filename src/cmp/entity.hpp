#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include <picoPNG/src/picopng.hpp>
#include <fstream>
#include <iostream>
#include <cstring>
#include <cmp/physics.hpp>
#include <util/typealiases.hpp>

namespace ECS {

struct Entity_t {
    explicit Entity_t(uint32_t pw, uint32_t ph)
    : w{pw}, h{ph}
    {
        sprite.resize(w*h);
    }
    explicit Entity_t(std::string filename) {
        std::vector<unsigned char> pixels;
        unsigned long dw {0}, dh {0};
        std::ifstream file(filename, std::ios::binary);

        if(file.is_open()) {
            std::vector<unsigned char> filevec(
                std::istreambuf_iterator<char>{file},
                std::istreambuf_iterator<char>{}
            );
            decodePNG(pixels, dw, dh, filevec.data(), filevec.size());
        } else {
            //controlar error
            std::cout << "ERROR ABRIENDO EL ARCHIVO\n";
        }
        // sprite.resize(pixels.size()/4);
        // std::memcpy(sprite.data(), pixels.data(), pixels.size());
        // sprite = std::vector<uint32_t>(pixels.begin(), pixels.end());
        w = dw; h = dh;
        sprite.reserve(pixels.size()/4);
        for(auto p = pixels.begin(); p != pixels.end(); p += 4) {
            uint32_t pixel = 
                static_cast<uint32_t>(*(p+0)) << 16 
            |   static_cast<uint32_t>(*(p+1)) << 8 
            |   static_cast<uint32_t>(*(p+2))
            |   static_cast<uint32_t>(*(p+3)) << 24;
            sprite.push_back(pixel);
        }
    }
    PhysicsComponent_t* phy {nullptr};
    uint32_t w{0}, h{0};
    std::vector<uint32_t> sprite {};
    EntityID_t entityID { ++nextID };

private:
    inline static EntityID_t nextID {0};
};
} // namespace ECS
