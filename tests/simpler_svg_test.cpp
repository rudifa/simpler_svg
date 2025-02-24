
/*******************************************************************************
*  The "New BSD License" : http://www.opensource.org/licenses/bsd-license.php  *
********************************************************************************

Copyright (c) 2025, Rudolf Farkas
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the <organization> nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

******************************************************************************/

#include "../src/simpler_svg.hpp"

#include <gtest/gtest.h>

using namespace svg;

// Test the Color class
TEST(ColorTest, Constructor)
{
    Color c(100, 150, 200);
    EXPECT_EQ(c.toString(), "rgb(100,150,200)");
}

// Test the Fill class
TEST(FillTest, Constructor)
{
    Fill f(Color::Red);
    EXPECT_EQ(f.toString(), "fill=\"rgb(255,0,0)\" ");
}

// Test the Point class
TEST(PointTest, Constructor)
{
    Point p(10, 20);
    EXPECT_EQ(p.x, 10);
    EXPECT_EQ(p.y, 20);
}

// Test the Size class
TEST(SizeTest, Constructor)
{
    Size d(100, 200);
    EXPECT_EQ(d.width, 100);
    EXPECT_EQ(d.height, 200);
}

// Test the Layout class
TEST(LayoutTest, Constructor)
{
    Size d(100, 100);
    Layout l(d);
    EXPECT_EQ(l.size.width, 100);
    EXPECT_EQ(l.size.height, 100);
    EXPECT_EQ(l.origin_offset.x, 0);
    EXPECT_EQ(l.origin_offset.y, 0);
    EXPECT_EQ(l.scale, 1.0);
    Layout l2(d, 2.0, Point(10, 10));
    EXPECT_EQ(l2.size.width, 100);
    EXPECT_EQ(l2.size.height, 100);
    EXPECT_EQ(l2.origin_offset.x, 10);
    EXPECT_EQ(l2.origin_offset.y, 10);
    EXPECT_EQ(l2.scale, 2.0);
}

// Test the Stroke class
TEST(StrokeTest, Constructor)
{
    Layout l1;
    Layout l2(Size(100, 100), 3.0, Point(10, 10));

    Stroke s0;
    EXPECT_EQ(s0.toString(l1), "");
    EXPECT_EQ(s0.toString(l2), "");

    Stroke s1(1);
    EXPECT_EQ(s1.toString(l1),
              "stroke-width=\"1\" stroke=\"rgb(192,192,192)\" ");
    EXPECT_EQ(s1.toString(l2),
              "stroke-width=\"3\" stroke=\"rgb(192,192,192)\" ");

    Stroke s2(2, Color::Red);
    EXPECT_EQ(s2.toString(l1), "stroke-width=\"2\" stroke=\"rgb(255,0,0)\" ");
    EXPECT_EQ(s2.toString(l2), "stroke-width=\"6\" stroke=\"rgb(255,0,0)\" ");
}

// Test the Circle class
TEST(CircleTest, Constructor)
{
    Point center(50, 50);
    Circle c(center, 30, Fill(Color::Red), Stroke(2, Color::Blue));
    Layout l(Size(100, 100));
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
    Layout l(Size(100, 100));
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
    Layout l(Size(100, 100));
    std::string expected =
        "\t<rect x=\"10\" y=\"80\" width=\"50\" height=\"30\" "
        "fill=\"rgb(0,128,0)\" />\n";
    EXPECT_EQ(r.toString(l), expected);
}

// Test the Line class
TEST(LineTest, Constructor)
{
    Point start(0, 0);
    Point end(100, 100);
    Line line(start, end, Stroke(2, Color::Black));
    Layout l(Size(200, 200));
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
    Layout l(Size(200, 200));
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
    Layout l(Size(200, 200));
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
    Layout l(Size(100, 100));
    std::string expected =
        "\t<text x=\"10\" y=\"80\" fill=\"rgb(0,0,255)\" font-size=\"12\" "
        "font-family=\"Arial\" >Hello, SVG!</text>\n";
    EXPECT_EQ(text.toString(l), expected);

    text.setRotation(45);
    std::string expected2 =
        "\t<text x=\"10\" y=\"80\" transform=\"rotate(-45.000000 10.000000 "
        "80.000000)\" fill=\"rgb(0,0,255)\" font-size=\"12\" "
        "font-family=\"Arial\" >Hello, SVG!</text>\n";
    EXPECT_EQ(text.toString(l), expected2);
}

// Test the LineChart class
TEST(LineChartTest, Constructor)
{
    LineChart chart;
    Polyline polyline;
    polyline << Point(0, 0) << Point(100, 100) << Point(200, 200)
             << Point(300, 300) << Point(400, 400) << Point(500, 500);
    chart << polyline;

    Layout l(Size(600, 600));
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
    Size size(100, 100);
    Document doc("test.svg", Layout(size));

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
