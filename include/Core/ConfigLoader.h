#pragma once
#include <string>
#include <unordered_map>

class ConfigLoader
{
public:
    // Funcion que le un fichero y devuelve los valores
    static std::unordered_map<std::string, float> load(const std::string& path);
};