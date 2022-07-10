#pragma once

#include "geo2d.h"
#include "game_object.h"

class Unit : public GameObject{
public:
    geo2d::Point geometry;

    explicit Unit(geo2d::Point position) : geometry(position) {}

    bool Collide(const GameObject& that) const override;

    bool CollideWith(const Unit& that) const override;

    bool CollideWith(const Building& that) const override;

    bool CollideWith(const Tower& that) const override;

    bool CollideWith(const Fence& that) const override;
};

class Building : public GameObject{
public:
    explicit Building(geo2d::Rectangle geometry) : geometry(geometry) {}

    bool Collide(const GameObject& that) const override;

    bool CollideWith(const Unit& that) const override;

    bool CollideWith(const Building& that) const override;

    bool CollideWith(const Tower& that) const override;

    bool CollideWith(const Fence& that) const override;

    geo2d::Rectangle geometry;
};

class Tower : public GameObject{
public:
    explicit Tower(geo2d::Circle geometry) : geometry(geometry) {}

    bool Collide(const GameObject& that) const override;

    bool CollideWith(const Unit& that) const override;

    bool CollideWith(const Building& that) const override;

    bool CollideWith(const Tower& that) const override;

    bool CollideWith(const Fence& that) const override;

    geo2d::Circle geometry;
};

class Fence : public GameObject{
public:
    explicit Fence(geo2d::Segment geometry) : geometry(geometry) {}

    bool Collide(const GameObject& that) const override;

    bool CollideWith(const Unit& that) const override;

    bool CollideWith(const Building& that) const override;

    bool CollideWith(const Tower& that) const override;

    bool CollideWith(const Fence& that) const override;

    geo2d::Segment geometry;
};

bool Collide(const GameObject& first, const GameObject& second);

void CollideTest();