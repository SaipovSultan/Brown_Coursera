#include "canvas.h"
#include "../test_runner.h"

#include <sstream>

void TestSimple() {
    Canvas canvas;
    canvas.SetSize({5, 3});

    canvas.AddShape(ShapeType::Rectangle, {1, 0}, {3, 3}, nullptr);

    std::stringstream output;
    canvas.Print(output);

    const auto answer =
            "#######\n"
            "# ... #\n"
            "# ... #\n"
            "# ... #\n"
            "#######\n";

    ASSERT_EQUAL(answer, output.str());
}

void TestSmallTexture() {
    Canvas canvas;
    canvas.SetSize({6, 4});

    canvas.AddShape(ShapeType::Rectangle, {1, 1}, {4, 2},
                    MakeTextureSolid({3, 1}, '*'));

    std::stringstream output;
    canvas.Print(output);

    const auto answer =
            "########\n"
            "#      #\n"
            "# ***. #\n"
            "# .... #\n"
            "#      #\n"
            "########\n";

    ASSERT_EQUAL(answer, output.str());
}

void TestCow() {
    Canvas canvas;
    canvas.SetSize({18, 5});

    canvas.AddShape(ShapeType::Rectangle, {1, 0}, {16, 5}, MakeTextureCow());

    std::stringstream output;
    canvas.Print(output);
    const auto answer =
            R"(####################)""\n"
            R"(# ^__^             #)""\n"
            R"(# (oo)\_______     #)""\n"
            R"(# (__)\       )\/\ #)""\n"
            R"(#     ||----w |    #)""\n"
            R"(#     ||     ||    #)""\n"
            R"(####################)""\n";

    ASSERT_EQUAL(answer, output.str());
}

void TestCpp() {
    Canvas canvas;
    canvas.SetSize({77, 17});

    canvas.AddShape(ShapeType::Ellipse, {2, 1}, {30, 15},
                    MakeTextureCheckers({100, 100}, 'c', 'C'));
    canvas.AddShape(ShapeType::Ellipse, {8, 4}, {30, 9},
                    MakeTextureSolid({100, 100}, ' '));

    auto h1 = canvas.AddShape(ShapeType::Rectangle, {54, 7}, {22, 3},
                              MakeTextureSolid({100, 100}, '+'));
    auto h2 = canvas.DuplicateShape(h1, {30, 7});

    auto v1 = canvas.DuplicateShape(h1, {62, 3});
    canvas.ResizeShape(v1, {6, 11});
    auto v2 = canvas.DuplicateShape(v1, {38, 3});

    std::stringstream output;
    canvas.Print(output);

    const auto answer =
            "###############################################################################\n"
            "#                                                                             #\n"
            "#            cCcCcCcCcC                                                       #\n"
            "#        CcCcCcCcCcCcCcCcCc                                                   #\n"
            "#      cCcCcCcCcCcCcCcCcCcCcC          ++++++                  ++++++         #\n"
            "#    CcCcCcCcCcCc                      ++++++                  ++++++         #\n"
            "#   CcCcCcCcC                          ++++++                  ++++++         #\n"
            "#   cCcCcCc                            ++++++                  ++++++         #\n"
            "#  cCcCcC                      ++++++++++++++++++++++  ++++++++++++++++++++++ #\n"
            "#  CcCcCc                      ++++++++++++++++++++++  ++++++++++++++++++++++ #\n"
            "#  cCcCcC                      ++++++++++++++++++++++  ++++++++++++++++++++++ #\n"
            "#   cCcCcCc                            ++++++                  ++++++         #\n"
            "#   CcCcCcCcC                          ++++++                  ++++++         #\n"
            "#    CcCcCcCcCcCc                      ++++++                  ++++++         #\n"
            "#      cCcCcCcCcCcCcCcCcCcCcC          ++++++                  ++++++         #\n"
            "#        CcCcCcCcCcCcCcCcCc                                                   #\n"
            "#            cCcCcCcCcC                                                       #\n"
            "#                                                                             #\n"
            "###############################################################################\n";

    ASSERT_EQUAL(answer, output.str());
}

void TestTextures() {
    TestRunner tr;
    RUN_TEST(tr, TestSimple);
    RUN_TEST(tr, TestSmallTexture);
    RUN_TEST(tr, TestCow);
    RUN_TEST(tr, TestCpp);
}