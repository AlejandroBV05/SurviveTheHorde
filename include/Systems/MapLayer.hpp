#pragma once
#include <SFML/Graphics.hpp>
#include <tmxlite/Map.hpp>

class MapLayer : public sf::Drawable
{
public:
    MapLayer(const tmx::Map& map, std::size_t layerIndex);

private:
    sf::VertexArray m_vertices;
    sf::Texture m_tileset;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};