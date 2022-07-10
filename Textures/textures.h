#pragma once

#include <vector>
#include <memory>

struct Point {
    int x;
    int y;
};

struct Size {
    int width;
    int height;
};

inline bool IsPointInEllipse(Point p, Size size) {
    double x = (p.x + 0.5) / (size.width / 2.0) - 1;
    double y = (p.y + 0.5) / (size.height / 2.0) - 1;
    return x * x + y * y <= 1;
}

using Image = std::vector<std::string>;

enum class ShapeType { Rectangle, Ellipse };

class ITexture {
public:
    virtual ~ITexture() = default;

    virtual Size GetSize() const = 0;

    virtual const Image& GetImage() const = 0;
};

class Texture : public ITexture {
public:
    Texture(Image image);

    Size GetSize() const override;

    const Image& GetImage() const override;

private:
    Image image_;
};

std::unique_ptr<ITexture> MakeTextureSolid(Size size, char pixel);

std::unique_ptr<ITexture> MakeTextureCheckers(Size size, char pixel1, char pixel2);

std::unique_ptr<ITexture> MakeTextureCow();
