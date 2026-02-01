#include "./Systems/MapLayer.hpp"
#include <tmxlite/TileLayer.hpp>
#include <iostream>

MapLayer::MapLayer(const tmx::Map& map, std::size_t layerIndex)
{
    // Obtiene las capas del mapa TMX
    const auto& layers = map.getLayers();

    // Comprueba si existe el layerIndex
    if (layerIndex >= layers.size())
        return;

    // Si la capa no es de tipo Tile no se dibuja
    if (layers[layerIndex]->getType() != tmx::Layer::Type::Tile)
        return;

    // Se convierte la capa en TileLayer
    const auto& tileLayer = layers[layerIndex]->getLayerAs<tmx::TileLayer>();
    const auto& tiles = tileLayer.getTiles();

    //Usa el primer tileset
    const auto& tileset = map.getTilesets().front();

    // Carga de la textura
    if (!m_tileset.loadFromFile(tileset.getImagePath()))
    {
        std::cout << "Failed to load tileset texture\n";
        return;
    }

    sf::Vector2u tileSize(map.getTileSize().x, map.getTileSize().y);

    // Tamaño del mapa
    std::size_t width = map.getTileCount().x;
    std::size_t height = map.getTileCount().y;

    m_vertices.setPrimitiveType(sf::Quads);
    m_vertices.resize(width * height * 4);

    // Funcion For que recore los tiles
    for (std::size_t y = 0; y < height; ++y)
    {
        for (std::size_t x = 0; x < width; ++x)
        {
            std::size_t index = x + y * width;
            auto tileID = tiles[index].ID;

            if (tileID == 0)
                continue;

            tileID -= tileset.getFirstGID();

            // Calcula la posicion del tile en el tileset
            std::size_t tu = tileID % (m_tileset.getSize().x / tileSize.x);
            std::size_t tv = tileID / (m_tileset.getSize().x / tileSize.x);

            // Obtenemos 4 vertices del quad
            sf::Vertex* quad = &m_vertices[static_cast<unsigned int>(index * 4)];

            // Posicion del tile en el mundo
            float px = static_cast<float>(x * tileSize.x);
            float py = static_cast<float>(y * tileSize.y);

            quad[0].position = { px, py };
            quad[1].position = { px + tileSize.x, py };
            quad[2].position = { px + tileSize.x, py + tileSize.y };
            quad[3].position = { px, py + tileSize.y };

            // Coordenadas de la textura en el tileset
            float tx = static_cast<float>(tu * tileSize.x);
            float ty = static_cast<float>(tv * tileSize.y);

            quad[0].texCoords = { tx, ty };
            quad[1].texCoords = { tx + tileSize.x, ty };
            quad[2].texCoords = { tx + tileSize.x, ty + tileSize.y };
            quad[3].texCoords = { tx, ty + tileSize.y };
        }
    }
}

// funcion que dibujo el tileMap
void MapLayer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.texture = &m_tileset;
    target.draw(m_vertices, states);
}