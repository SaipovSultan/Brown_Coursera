#pragma once
#include "textures.h"

#include <memory>
#include <string>
#include <vector>



class IShape {
public:
    virtual ~IShape() = default;

    virtual std::unique_ptr<IShape> Clone() const = 0;

    virtual void SetPosition(Point) = 0;
    virtual Point GetPosition() const = 0;

    virtual void SetSize(Size) = 0;
    virtual Size GetSize() const = 0;

    virtual void SetTexture(std::shared_ptr<ITexture>) = 0;
    virtual ITexture* GetTexture() const = 0;

    virtual void Draw(Image&) const = 0;
};

class Shape : public IShape {
    void SetPosition(Point position) override{
        this->position = position;
    }

    Point GetPosition() const override {
        return position;
    }

    void SetSize(Size size) override {
        this->size = size;
    }

    Size GetSize() const override {
        return size;
    }

    void SetTexture(std::shared_ptr<ITexture> texture) override {
        this->texture = std::move(texture);
    }

    ITexture* GetTexture() const override {
        return texture.get();
    }

    void Draw(Image& image) const override {
        for(int shapeY = 0;(shapeY + position.y) < image.size() && shapeY < size.height;++shapeY) {
            for(int shapeX = 0;(shapeX + position.x) < image[shapeY].size() && shapeX < size.width;++shapeX) {
                if(IsPointInShape({shapeX, shapeY}, size)) {
                    image[shapeY + position.y][shapeX + position.x] = '.';
                }
            }
        }
        ITexture* texture = GetTexture();
        if(!texture) {
            return;
        }
        Size textureSize = texture->GetSize();
        const Image& textureImage = texture->GetImage();
        for(int shapeY = 0;(shapeY + position.y) < image.size() && shapeY < size.height && shapeY < textureSize.height;++shapeY) {
            for(int shapeX = 0;(shapeX + position.x) < image[shapeY].size() && shapeX < size.width && shapeX < textureSize.width;++shapeX) {
                if(IsPointInShape({shapeX, shapeY}, size)) {
                    image[shapeY + position.y][shapeX + position.x] = textureImage[shapeY][shapeX];
                }
            }
        }
    }

private:
    virtual bool IsPointInShape(Point point, Size size) const = 0;
protected:
    std::shared_ptr<ITexture> texture;
    Point position;
    Size size;
};

class Rectangle : public Shape {
public:
    std::unique_ptr<IShape> Clone() const override {
        auto rectangle = std::make_unique<Rectangle>();
        rectangle->position = position;
        rectangle->size = size;
        rectangle->texture = texture;
        return rectangle;
    }
private:
    bool IsPointInShape(Point point, Size size) const override {
        return (point.x >= 0 && point.x < size.width) && (point.y >= 0 && point.y < size.height);
    }
};

class Ellipse : public Shape {
public:
    std::unique_ptr<IShape> Clone() const override {
        auto ellipse = std::make_unique<Ellipse>();
        ellipse->position = position;
        ellipse->size = size;
        ellipse->texture = texture;
        return ellipse;
    }
private:
    bool IsPointInShape(Point point, Size size) const override {
        return IsPointInEllipse(point, size);
    }
};

std::unique_ptr<IShape> MakeShape(ShapeType shape_type);