#include "collide.h"
#include "../test_runner.h"

#include <vector>
#include <memory>

void TestAddingNewObjectOnMap() {
    using namespace geo2d;

    const std::vector<std::shared_ptr<GameObject>> game_map = {
            std::make_shared<Unit>(Point{3, 3}),
            std::make_shared<Unit>(Point{5, 5}),
            std::make_shared<Unit>(Point{3, 7}),
            std::make_shared<Fence>(Segment{{7, 3}, {9, 8}}),
            std::make_shared<Tower>(Circle{Point{9, 4}, 1}),
            std::make_shared<Tower>(Circle{Point{10, 7}, 1}),
            std::make_shared<Building>(Rectangle{{11, 4}, {14, 6}})
    };

    for (size_t i = 0; i < game_map.size(); ++i) {
        Assert(
                Collide(*game_map[i], *game_map[i]),
                "An object doesn't collide with itself: " + to_string(i)
        );

        for (size_t j = 0; j < i; ++j) {
            Assert(
                    !Collide(*game_map[i], *game_map[j]),
                    "Unexpected collision found " + to_string(i) + ' ' + to_string(j)
            );
        }
    }

    auto new_warehouse = std::make_shared<Building>(Rectangle{{4, 3}, {9, 6}});
    ASSERT(!Collide(*new_warehouse, *game_map[0]));
    ASSERT( Collide(*new_warehouse, *game_map[1]));
    ASSERT(!Collide(*new_warehouse, *game_map[2]));
    ASSERT( Collide(*new_warehouse, *game_map[3]));
    ASSERT( Collide(*new_warehouse, *game_map[4]));
    ASSERT(!Collide(*new_warehouse, *game_map[5]));
    ASSERT(!Collide(*new_warehouse, *game_map[6]));

    auto new_defense_tower = std::make_shared<Tower>(Circle{{8, 2}, 2});
    ASSERT(!Collide(*new_defense_tower, *game_map[0]));
    ASSERT(!Collide(*new_defense_tower, *game_map[1]));
    ASSERT(!Collide(*new_defense_tower, *game_map[2]));
    ASSERT( Collide(*new_defense_tower, *game_map[3]));
    ASSERT( Collide(*new_defense_tower, *game_map[4]));
    ASSERT(!Collide(*new_defense_tower, *game_map[5]));
    ASSERT(!Collide(*new_defense_tower, *game_map[6]));
}

void CollideTest() {
    TestRunner tr;
    RUN_TEST(tr, TestAddingNewObjectOnMap);
}