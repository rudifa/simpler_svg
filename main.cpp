
/*******************************************************************************
*  The "New BSD License" : http://www.opensource.org/licenses/bsd-license.php  *
********************************************************************************

Copyright (c) 2010, Mark Turney
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

#include "src/simpler_svg.hpp"

using namespace svg;

// Demo page shows sample usage of the Simple SVG library.

int main()
{
    Size size(500, 500);
    Document doc("my_svg.svg", Layout(size));

    // Red image border.
    Polygon border(Stroke(1, Color::Red));
    border << Point(0, 0) << Point(size.width, 0)
           << Point(size.width, size.height) << Point(0, size.height);
    doc << border;

    // Long notation.  Local variable is created, children are added to
    // varaible.
    LineChart chart(Size(), 5.0);
    Polyline polyline_a(Stroke(.5, Color::Blue));
    Polyline polyline_b(Stroke(.5, Color::Aqua));
    Polyline polyline_c(Stroke(.5, Color::Fuchsia));
    polyline_a << Point(0, 0) << Point(40, 120) << Point(80, 160)
               << Point(120, 180) << Point(160, 176);
    polyline_b << Point(0, 40) << Point(40, 88) << Point(80, 120)
               << Point(120, 128) << Point(160, 120);
    polyline_c << Point(0, 48) << Point(40, 60) << Point(80, 56)
               << Point(120, 40) << Point(160, 8);
    chart << polyline_a << polyline_b << polyline_c;
    doc << chart;

    // Condensed notation, parenthesis isolate temporaries that are inserted
    // into parents.
    doc << (LineChart(Size(260, 20))
            << (Polyline(Stroke(.5, Color::Blue))
                << Point(0, 0) << Point(40, 32) << Point(80, 52))
            << (Polyline(Stroke(.5, Color::Orange))
                << Point(0, 40) << Point(40, 64) << Point(80, 80))
            << (Polyline(Stroke(.5, Color::Cyan))
                << Point(0, 20) << Point(40, 52) << Point(80, 64)));

    doc << Circle(Point(320, 320), 80, Fill(Color(100, 200, 120)),
                  Stroke(1, Color(200, 250, 150)));

    doc << Text(Point(20, 300), "Simple SVG", Font(40, "Verdana"),
                Fill(Color::Silver));

    doc << Text(Point(20, 300), "Simple SVG", Font(20, "Verdana"),
                Fill(Color::Silver), Stroke(), 45);

    doc << (Polygon(Fill(Color(200, 160, 220)),
                    Stroke(.5, Color(150, 160, 200)))
            << Point(80, 280) << Point(100, 288) << Point(132, 280)
            << Point(140, 240) << Point(100, 220) << Point(72, 252));

    doc << Rectangle(Point(280, 220), 80, 60, Fill(Color::Yellow));

    Group g("G1");
    g << Circle(Point(0, 0), 30, Fill(Color::Red));
    g << Rectangle(Point(0, 0), 30, 15, Fill(), Stroke(1, Color::Black));
    doc << g;

    g.offset(Point(400, 15));

    g << Rectangle(Point(0, 0), 50, 50, Fill(), Stroke(1, Color::Blue));
    g << Rectangle(Point(0, 500 - 50), 50, 50, Fill(), Stroke(1, Color::Blue));
    g << Rectangle(Point(500 - 50, 500 - 50), 50, 50, Fill(),
                   Stroke(1, Color::Blue));
    g << Rectangle(Point(500 - 50, 0), 50, 50, Fill(), Stroke(1, Color::Blue));

    g << Circle(Point(250, 450), 10, Fill(), Stroke(1, Color::Black));
    Text centered(Point(250, 450), "Centered Text", Font(20, "Arial"),
                  Fill(Color::Green));
    centered.setTextAnchor("middle");
    centered.setDominantBaseline("middle");
    g << centered;

    doc << g;

    if (doc.save())
    {
        std::cout << "Saved " << doc.filename() << std::endl;
        return 0;
    }
    else
    {
        std::cerr << "Failed to save " << doc.filename() << std::endl;
        return 1;
    }
}
