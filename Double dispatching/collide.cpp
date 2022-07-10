#include "collide.h"

using namespace std;

bool Unit::Collide(const GameObject& that) const {
    return that.CollideWith(*this);
}

bool Unit::CollideWith(const Unit& that) const {
    return geo2d::Collide(geometry, that.geometry);
}

bool Unit::CollideWith(const Building& that) const {
    return geo2d::Collide(geometry, that.geometry);
}

bool Unit::CollideWith(const Tower& that) const {
    return geo2d::Collide(geometry, that.geometry);
}

bool Unit::CollideWith(const Fence& that) const {
    return geo2d::Collide(geometry, that.geometry);
}

bool Building::Collide(const GameObject& that) const {
    return that.CollideWith(*this);
}

bool Building::CollideWith(const Unit& that) const {
    return geo2d::Collide(geometry, that.geometry);
}

bool Building::CollideWith(const Building& that) const {
    return geo2d::Collide(geometry, that.geometry);
}

bool Building::CollideWith(const Tower& that) const {
    return geo2d::Collide(geometry, that.geometry);
}

bool Building::CollideWith(const Fence& that) const {
    return geo2d::Collide(geometry, that.geometry);
}

bool Tower::Collide(const GameObject& that) const {
    return that.CollideWith(*this);
}

bool Tower::CollideWith(const Unit& that) const {
    return geo2d::Collide(geometry, that.geometry);
}

bool Tower::CollideWith(const Building& that) const {
    return geo2d::Collide(geometry, that.geometry);
}

bool Tower::CollideWith(const Tower& that) const {
    return geo2d::Collide(geometry, that.geometry);
}

bool Tower::CollideWith(const Fence& that) const {
    return geo2d::Collide(geometry, that.geometry);
}

bool Fence::Collide(const GameObject& that) const {
    return that.CollideWith(*this);
}

bool Fence::CollideWith(const Unit& that) const {
    return geo2d::Collide(geometry, that.geometry);
}


bool Fence::CollideWith(const Building& that) const {
    return geo2d::Collide(geometry, that.geometry);
}

bool Fence::CollideWith(const Tower& that) const {
    return geo2d::Collide(geometry, that.geometry);
}

bool Fence::CollideWith(const Fence& that) const {
    return geo2d::Collide(geometry, that.geometry);
}

bool Collide(const GameObject& first, const GameObject& second) {
    return first.Collide(second);
}
