#include "common.h"
#include "textures.h"

#include <iostream>
#include <map>

class Canvas {
public:
    using ShapeId = size_t;

    void SetSize(Size size) {
        size_ = size;
    }

    ShapeId AddShape(ShapeType shape_type, Point position, Size size,
                     std::unique_ptr<ITexture> texture) {
        auto shape = MakeShape(shape_type);
        shape->SetPosition(position);
        shape->SetSize(size);
        shape->SetTexture(move(texture));
        return InsertShape(move(shape));
    }

    ShapeId DuplicateShape(ShapeId source_id, Point target_position) {
        auto shape = GetShapeNodeById(source_id)->second->Clone();
        shape->SetPosition(target_position);
        return InsertShape(move(shape));
    }

    void RemoveShape(ShapeId id) {
        shapes_.erase(GetShapeNodeById(id));
    }

    void MoveShape(ShapeId id, Point position) {
        GetShapeNodeById(id)->second->SetPosition(position);
    }

    void ResizeShape(ShapeId id, Size size) {
        GetShapeNodeById(id)->second->SetSize(size);
    }

    int GetShapesCount() const {
        return static_cast<int>(shapes_.size());
    }

    void Print(std::ostream& output) const {
        Image image(size_.height, std::string(size_.width, ' '));

        for (const auto& [id, shape] : shapes_) {
            shape->Draw(image);
        }

        output << '#' << std::string(size_.width, '#') << "#\n";
        for (const auto& line : image) {
            output << '#' << line << "#\n";
        }
        output << '#' << std::string(size_.width, '#') << "#\n";
    }

private:
    using Shapes = std::map<ShapeId, std::unique_ptr<IShape>>;

    Shapes::iterator GetShapeNodeById(ShapeId id) {
        auto it = shapes_.find(id);
        if (it == shapes_.end()) {
            throw std::out_of_range("No shape with given ID");
        }
        return it;
    }
    ShapeId InsertShape(std::unique_ptr<IShape> shape) {
        shapes_[current_id_] = move(shape);
        return current_id_++;
    }

    Size size_ = {};
    ShapeId current_id_ = 0u;
    Shapes shapes_;
};