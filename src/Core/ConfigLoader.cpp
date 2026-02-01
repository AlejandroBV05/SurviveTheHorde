#include "./Core/ConfigLoader.h"
#include <fstream>
#include <sstream>

// Funcion que carga archivo de configuracion
std::unordered_map<std::string, float> ConfigLoader::load(const std::string& path)
{
    // Donde se guardan los datos del archivo
    std::unordered_map<std::string, float> values;
    std::ifstream file(path);

    // El archivo se lee linea por linea
    std::string line;
    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string key;
        float value;

        if (std::getline(ss, key, '=') && ss >> value)
        {
            // Se guarda el valor
            values[key] = value;
        }
    }

    // Devuelve los datos leidos
    return values;
}