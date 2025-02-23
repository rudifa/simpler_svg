#include "../src/simpler_svg.hpp"

#include <gtest/gtest.h>

using namespace svg;

// Test the Color class
TEST(ColorTest, Constructor)
{
    Layout layout = Layout(Dimensions(100, 100),
                           Layout::BottomLeft);  // default is BottomLeft
    Color c(100, 150, 200);
    EXPECT_EQ(c.toString(layout), "rgb(100,150,200)");
}

// Test the Point class
TEST(PointTest, Constructor)
{
    Point p(10, 20);
    EXPECT_EQ(p.x, 10);
    EXPECT_EQ(p.y, 20);
}

// Test the Dimensions class
TEST(DimensionsTest, Constructor)
{
    Dimensions d(100, 200);
    EXPECT_EQ(d.width, 100);
    EXPECT_EQ(d.height, 200);
}

// Test the Layout class
TEST(LayoutTest, Constructor)
{
    Dimensions d(100, 100);
    Layout l(d, Layout::BottomLeft);
    EXPECT_EQ(l.dimensions.width, 100);
    EXPECT_EQ(l.dimensions.height, 100);
    EXPECT_EQ(l.origin, Layout::BottomLeft);
}

// Test the Circle class
TEST(CircleTest, Constructor)
{
    Point center(50, 50);
    Circle c(center, 30, Fill(Color::Red), Stroke(2, Color::Blue));
    Layout l(Dimensions(100, 100), Layout::BottomLeft);
    std::string expected =
        "\t<circle cx=\"50\" cy=\"50\" r=\"15\" fill=\"rgb(255,0,0)\" "
        "stroke-width=\"2\" stroke=\"rgb(0,0,255)\" />\n";
    EXPECT_EQ(c.toString(l), expected);
}

// Test the Elipse class
TEST(ElipseTest, Constructor)
{
    Point center(50, 50);
    Elipse e(center, 30, 40, Fill(Color::Red), Stroke(2, Color::Blue));
    Layout l(Dimensions(100, 100), Layout::BottomLeft);
    std::string expected =
        "\t<ellipse cx=\"50\" cy=\"50\" rx=\"15\" ry=\"20\" "
        "fill=\"rgb(255,0,0)\" "
        "stroke-width=\"2\" stroke=\"rgb(0,0,255)\" />\n";
    EXPECT_EQ(e.toString(l), expected);
}

// Test the Rectangle class
TEST(RectangleTest, Constructor)
{
    Point origin(10, 20);
    Rectangle r(origin, 50, 30, Fill(Color::Green));
    Layout l(Dimensions(100, 100), Layout::TopLeft);
    std::string expected =
        "\t<rect x=\"10\" y=\"20\" width=\"50\" height=\"30\" "
        "fill=\"rgb(0,128,0)\" />\n";
    EXPECT_EQ(r.toString(l), expected);
}

// Test the Line class
TEST(LineTest, Constructor)
{
    Point start(0, 0);
    Point end(100, 100);
    Line line(start, end, Stroke(2, Color::Black));
    Layout l(Dimensions(200, 200), Layout::BottomLeft);
    std::string expected =
        "\t<line x1=\"0\" y1=\"200\" x2=\"100\" y2=\"100\" stroke-width=\"2\" "
        "stroke=\"rgb(0,0,0)\" />\n";
    EXPECT_EQ(line.toString(l), expected);
}

// Test the Polygon class
TEST(PolygonTest, Constructor)
{
    Polygon polygon;
    polygon << Point(0, 0);
    polygon << Point(100, 0);
    polygon << Point(100, 100);
    polygon << Point(0, 100);
    Layout l(Dimensions(200, 200), Layout::BottomLeft);
    std::string expected =
        "\t<polygon points=\"0,200 100,200 100,100 0,100 \" "
        "fill=\"transparent\" />\n";
    EXPECT_EQ(polygon.toString(l), expected);
}

// Test the Polyline class
TEST(PolylineTest, Constructor)
{
    Polyline polyline;
    polyline << Point(0, 0);
    polyline << Point(100, 0);
    polyline << Point(100, 100);
    polyline << Point(0, 100);
    Layout l(Dimensions(200, 200), Layout::BottomLeft);
    std::string expected =
        "\t<polyline points=\"0,200 100,200 100,100 0,100 \" "
        "fill=\"transparent\" />\n";
    EXPECT_EQ(polyline.toString(l), expected);
}

// Test the Text class
TEST(TextTest, Constructor)
{
    Point pos(10, 20);
    Text text(pos, "Hello, SVG!", Fill(Color::Blue), Font(12, "Arial"));
    Layout l(Dimensions(100, 100), Layout::TopLeft);
    std::string expected =
        "\t<text x=\"10\" y=\"20\" fill=\"rgb(0,0,255)\" font-size=\"12\" "
        "font-family=\"Arial\" >Hello, SVG!</text>\n";
    EXPECT_EQ(text.toString(l), expected);
}

// Test the LineChart class
TEST(LineChartTest, Constructor)
{
    LineChart chart;
    Polyline polyline;
    polyline << Point(0, 0) << Point(100, 100) << Point(200, 200)
             << Point(300, 300) << Point(400, 400) << Point(500, 500);
    chart << polyline;

    Layout l(Dimensions(600, 600), Layout::BottomLeft);
    std::string expected =
        "\t<polyline points=\"0,600 100,500 200,400 300,300 400,200 500,100 \" "
        "fill=\"transparent\" />\n\t<circle cx=\"0\" cy=\"600\" r=\"8.33333\" "
        "fill=\"rgb(0,0,0)\" />\n\t<circle cx=\"100\" cy=\"500\" r=\"8.33333\" "
        "fill=\"rgb(0,0,0)\" />\n\t<circle cx=\"200\" cy=\"400\" r=\"8.33333\" "
        "fill=\"rgb(0,0,0)\" />\n\t<circle cx=\"300\" cy=\"300\" r=\"8.33333\" "
        "fill=\"rgb(0,0,0)\" />\n\t<circle cx=\"400\" cy=\"200\" r=\"8.33333\" "
        "fill=\"rgb(0,0,0)\" />\n\t<circle cx=\"500\" cy=\"100\" r=\"8.33333\" "
        "fill=\"rgb(0,0,0)\" />\n\t<polyline points=\"0,50 0,600 550,600 \" "
        "fill=\"transparent\" stroke-width=\"0.5\" stroke=\"rgb(128,0,128)\" "
        "/>\n";
    EXPECT_EQ(chart.toString(l), expected);
}

// Test the Document class
TEST(DocumentTest, SaveAndLoad)
{
    Dimensions dimensions(100, 100);
    Document doc("test.svg", Layout(dimensions, Layout::BottomLeft));

    doc << Circle(Point(50, 50), 30, Fill(Color::Red));

    EXPECT_TRUE(doc.save());

    // Check if the file exists
    std::ifstream file("test.svg");
    EXPECT_TRUE(file.good());
    file.close();

    // Clean up
    std::remove("test.svg");
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
