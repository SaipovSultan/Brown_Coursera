#include "common.h"

using namespace std;

unique_ptr<IShape> MakeShape(ShapeType shape_type) {
    if (shape_type == ShapeType::Rectangle) {
        return make_unique<Rectangle>();
    } else {
        return make_unique<Ellipse>();
    }
}