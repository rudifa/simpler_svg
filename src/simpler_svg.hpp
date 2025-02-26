
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

#ifndef SIMPLE_SVG_HPP
#define SIMPLE_SVG_HPP

#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

namespace svg
{
// Utility XML/String Functions.
template <typename T>
std::string attribute(std::string const &attribute_name, T const &value,
                      std::string const &unit = "")
{
    std::stringstream ss;
    ss << attribute_name << "=\"" << value << unit << "\" ";
    return ss.str();
}
std::string elemStart(std::string const &element_name)
{
    return "\t<" + element_name + " ";
}
std::string elemEnd(std::string const &element_name)
{
    return "</" + element_name + ">\n";
}
std::string emptyElemEnd() { return "/>\n"; }

struct Size
{
    Size(double width, double height) : width(width), height(height) {}
    explicit Size(double combined = 0) : width(combined), height(combined) {}
    double width;
    double height;
};

struct Point
{
    explicit Point(double x = 0, double y = 0) : x(x), y(y) {}
    double x;
    double y;
};
std::optional<Point> getMinPoint(std::vector<Point> const &points)
{
    if (points.empty()) return std::nullopt;

    Point min = points[0];
    for (unsigned i = 0; i < points.size(); ++i)
    {
        if (points[i].x < min.x) min.x = points[i].x;
        if (points[i].y < min.y) min.y = points[i].y;
    }
    return min;
}
std::optional<Point> getMaxPoint(std::vector<Point> const &points)
{
    if (points.empty()) return std::nullopt;

    Point max = points[0];
    for (unsigned i = 0; i < points.size(); ++i)
    {
        if (points[i].x > max.x) max.x = points[i].x;
        if (points[i].y > max.y) max.y = points[i].y;
    }
    return max;
}

// Defines the size, scale, origin, and origin offset of the document.
struct Layout
{
    explicit Layout(Size const &size = Size(400, 300), double scale = 1,
                    Point const &origin_offset = Point(0, 0))
        : size(size), scale(scale), origin_offset(origin_offset)
    {
    }
    Size size;
    double scale;
    Point origin_offset;
};

// Convert coordinates in user space to SVG native space.
double translateX(double x, Layout const &layout)
{
    return (layout.origin_offset.x + x) * layout.scale;
}

double translateY(double y, Layout const &layout)
{
    return layout.size.height - ((y + layout.origin_offset.y) * layout.scale);
}
double translateScale(double dimension, Layout const &layout)
{
    return dimension * layout.scale;
}

class Serializeable
{
   public:
    Serializeable() {}
    virtual ~Serializeable() {};
    virtual std::string toString(Layout const &layout) const = 0;
};

class Color : public Serializeable
{
   public:
    enum Defaults
    {
        Transparent = -1,
        Aqua,
        Black,
        Blue,
        Brown,
        Cyan,
        Fuchsia,
        Green,
        Lime,
        Magenta,
        Orange,
        Purple,
        Red,
        Silver,
        White,
        Yellow
    };

    Color(int r, int g, int b) : transparent(false), red(r), green(g), blue(b)
    {
    }
    explicit Color(Defaults color)
        : transparent(false), red(0), green(0), blue(0)
    {
        switch (color)
        {
            case Aqua:
                assign(0, 255, 255);
                break;
            case Black:
                assign(0, 0, 0);
                break;
            case Blue:
                assign(0, 0, 255);
                break;
            case Brown:
                assign(165, 42, 42);
                break;
            case Cyan:
                assign(0, 255, 255);
                break;
            case Fuchsia:
                assign(255, 0, 255);
                break;
            case Green:
                assign(0, 128, 0);
                break;
            case Lime:
                assign(0, 255, 0);
                break;
            case Magenta:
                assign(255, 0, 255);
                break;
            case Orange:
                assign(255, 165, 0);
                break;
            case Purple:
                assign(128, 0, 128);
                break;
            case Red:
                assign(255, 0, 0);
                break;
            case Silver:
                assign(192, 192, 192);
                break;
            case White:
                assign(255, 255, 255);
                break;
            case Yellow:
                assign(255, 255, 0);
                break;
            default:
                transparent = true;
                break;
        }
    }
    virtual ~Color() override {}
    std::string toString(Layout const &layout = Layout()) const override
    {
        std::stringstream ss;
        if (transparent)
            ss << "transparent";
        else
            ss << "rgb(" << red << "," << green << "," << blue << ")";
        return ss.str();
    }

   private:
    bool transparent;
    int red;
    int green;
    int blue;

    void assign(int r, int g, int b)
    {
        red = r;
        green = g;
        blue = b;
    }
};

class Fill : public Serializeable
{
   public:
    explicit Fill(Color::Defaults color = Color::Transparent)
        : color(Color(color))
    {
    }
    explicit Fill(Color color) : color(color) {}
    std::string toString(Layout const &layout = Layout()) const override
    {
        std::stringstream ss;
        ss << attribute("fill", color.toString());
        return ss.str();
    }

   private:
    Color color;
};

class Stroke : public Serializeable
{
   public:
    explicit Stroke(double width = 0, Color::Defaults color = Color::Silver)
        : width(width), color(Color(color))
    {
    }
    explicit Stroke(double width, Color color) : width(width), color(color) {}
    std::string toString(Layout const &layout) const override
    {
        // If stroke width is invalid.
        if (width <= 0) return std::string();

        std::stringstream ss;
        ss << attribute("stroke-width", translateScale(width, layout))
           << attribute("stroke", color.toString());
        return ss.str();
    }

   private:
    double width;
    Color color;
};

class Font : public Serializeable
{
   public:
    explicit Font(double size = 12, std::string const &family = "Verdana")
        : size(size), family(family)
    {
    }
    std::string toString(Layout const &layout) const override
    {
        std::stringstream ss;
        ss << attribute("font-size", translateScale(size, layout))
           << attribute("font-family", family);
        return ss.str();
    }

   private:
    double size;
    std::string family;
};

class Shape : public Serializeable
{
   public:
    Shape(Fill const &fill = Fill(), Stroke const &stroke = Stroke())
        : fill(fill), stroke(stroke)
    {
    }
    virtual ~Shape() override {}
    virtual std::string toString(Layout const &layout) const override = 0;
    virtual void offset(Point const &offset) = 0;
    virtual std::unique_ptr<Shape> clone() const = 0;

   protected:
    Fill fill;
    Stroke stroke;
};
template <typename T>
std::string vectorToString(std::vector<T> collection, Layout const &layout)
{
    std::string combination_str;
    for (unsigned i = 0; i < collection.size(); ++i)
        combination_str += collection[i].toString(layout);

    return combination_str;
}

class Circle : public Shape
{
   public:
    Circle(Point const &center, double diameter, Fill const &fill,
           Stroke const &stroke = Stroke())
        : Shape(fill, stroke), center(center), radius(diameter / 2)
    {
    }
    std::string toString(Layout const &layout) const override
    {
        std::stringstream ss;
        ss << elemStart("circle")
           << attribute("cx", translateX(center.x, layout))
           << attribute("cy", translateY(center.y, layout))
           << attribute("r", translateScale(radius, layout))
           << fill.toString(layout) << stroke.toString(layout)
           << emptyElemEnd();
        return ss.str();
    }
    void offset(Point const &offset) override
    {
        center.x += offset.x;
        center.y += offset.y;
    }

    virtual std::unique_ptr<Shape> clone() const override
    {
        return std::make_unique<Circle>(*this);
    }

   private:
    Point center;
    double radius;
};

class Elipse : public Shape
{
   public:
    Elipse(Point const &center, double width, double height,
           Fill const &fill = Fill(), Stroke const &stroke = Stroke())
        : Shape(fill, stroke),
          center(center),
          radius_width(width / 2),
          radius_height(height / 2)
    {
    }
    std::string toString(Layout const &layout) const override
    {
        std::stringstream ss;
        ss << elemStart("ellipse")
           << attribute("cx", translateX(center.x, layout))
           << attribute("cy", translateY(center.y, layout))
           << attribute("rx", translateScale(radius_width, layout))
           << attribute("ry", translateScale(radius_height, layout))
           << fill.toString(layout) << stroke.toString(layout)
           << emptyElemEnd();
        return ss.str();
    }
    void offset(Point const &offset) override
    {
        center.x += offset.x;
        center.y += offset.y;
    }

    virtual std::unique_ptr<Shape> clone() const override
    {
        return std::make_unique<Elipse>(*this);
    }

   private:
    Point center;
    double radius_width;
    double radius_height;
};

class Rectangle : public Shape
{
   public:
    Rectangle(Point const &edge, double width, double height,
              Fill const &fill = Fill(), Stroke const &stroke = Stroke())
        : Shape(fill, stroke), edge(edge), width(width), height(height)
    {
    }
    std::string toString(Layout const &layout) const override
    {
        std::stringstream ss;
        ss << elemStart("rect") << attribute("x", translateX(edge.x, layout))
           << attribute("y", translateY(edge.y, layout) - height)
           << attribute("width", translateScale(width, layout))
           << attribute("height", translateScale(height, layout))
           << fill.toString(layout) << stroke.toString(layout)
           << emptyElemEnd();
        return ss.str();
    }
    void offset(Point const &offset) override
    {
        edge.x += offset.x;
        edge.y += offset.y;
    }

    virtual std::unique_ptr<Shape> clone() const override
    {
        return std::make_unique<Rectangle>(*this);
    }

   private:
    Point edge;
    double width;
    double height;
};

class Line : public Shape
{
   public:
    Line(Point const &start_point, Point const &end_point,
         Stroke const &stroke = Stroke())
        : Shape(Fill(), stroke), start_point(start_point), end_point(end_point)
    {
    }
    std::string toString(Layout const &layout) const override
    {
        std::stringstream ss;
        ss << elemStart("line")
           << attribute("x1", translateX(start_point.x, layout))
           << attribute("y1", translateY(start_point.y, layout))
           << attribute("x2", translateX(end_point.x, layout))
           << attribute("y2", translateY(end_point.y, layout))
           << stroke.toString(layout) << emptyElemEnd();
        return ss.str();
    }
    void offset(Point const &offset) override
    {
        start_point.x += offset.x;
        start_point.y += offset.y;

        end_point.x += offset.x;
        end_point.y += offset.y;
    }

    virtual std::unique_ptr<Shape> clone() const override
    {
        return std::make_unique<Line>(*this);
    }

   private:
    Point start_point;
    Point end_point;
};

class Polygon : public Shape
{
   public:
    explicit Polygon(Fill const &fill = Fill(), Stroke const &stroke = Stroke())
        : Shape(fill, stroke)
    {
    }
    explicit Polygon(Stroke const &stroke)
        : Shape(Fill(Color::Transparent), stroke)
    {
    }
    Polygon &operator<<(Point const &point)
    {
        points.push_back(point);
        return *this;
    }
    std::string toString(Layout const &layout) const override
    {
        std::stringstream ss;
        ss << elemStart("polygon");

        ss << "points=\"";
        for (unsigned i = 0; i < points.size(); ++i)
            ss << translateX(points[i].x, layout) << ","
               << translateY(points[i].y, layout) << " ";
        ss << "\" ";

        ss << fill.toString(layout) << stroke.toString(layout)
           << emptyElemEnd();
        return ss.str();
    }
    void offset(Point const &offset) override
    {
        for (unsigned i = 0; i < points.size(); ++i)
        {
            points[i].x += offset.x;
            points[i].y += offset.y;
        }
    }

    virtual std::unique_ptr<Shape> clone() const override
    {
        return std::make_unique<Polygon>(*this);
    }

   private:
    std::vector<Point> points;
};

class Polyline : public Shape
{
   public:
    explicit Polyline(Fill const &fill = Fill(),
                      Stroke const &stroke = Stroke())
        : Shape(fill, stroke)
    {
    }
    explicit Polyline(Stroke const &stroke)
        : Shape(Fill(Color::Transparent), stroke)
    {
    }
    explicit Polyline(std::vector<Point> const &points,
                      Fill const &fill = Fill(),
                      Stroke const &stroke = Stroke())
        : Shape(fill, stroke), points(points)
    {
    }
    Polyline &operator<<(Point const &point)
    {
        points.push_back(point);
        return *this;
    }
    std::string toString(Layout const &layout) const override
    {
        std::stringstream ss;
        ss << elemStart("polyline");

        ss << "points=\"";
        for (unsigned i = 0; i < points.size(); ++i)
            ss << translateX(points[i].x, layout) << ","
               << translateY(points[i].y, layout) << " ";
        ss << "\" ";

        ss << fill.toString(layout) << stroke.toString(layout)
           << emptyElemEnd();
        return ss.str();
    }
    void offset(Point const &offset) override
    {
        for (unsigned i = 0; i < points.size(); ++i)
        {
            points[i].x += offset.x;
            points[i].y += offset.y;
        }
    }

    virtual std::unique_ptr<Shape> clone() const override
    {
        return std::make_unique<Polyline>(*this);
    }

    std::vector<Point> points;
};

class Text : public Shape
{
   public:
    Text(Point const &origin, std::string const &content,
         Fill const &fill = Fill(), Font const &font = Font(),
         Stroke const &stroke = Stroke(), double rotation = 0)
        : Shape(fill, stroke),
          origin(origin),
          content(content),
          font(font),
          rotation(rotation)
    {
    }
    std::string toString(Layout const &layout) const override
    {
        std::stringstream ss;
        ss << elemStart("text") << attribute("x", translateX(origin.x, layout))
           << attribute("y", translateY(origin.y, layout));

        if (rotation != 0)
        {
            ss << attribute(
                "transform",
                "rotate(" + std::to_string(-rotation) + " " +
                    std::to_string(translateX(origin.x, layout)) + " " +
                    std::to_string(translateY(origin.y, layout)) + ")");
        }

        ss << fill.toString(layout) << stroke.toString(layout)
           << font.toString(layout) << ">" << content << elemEnd("text");
        return ss.str();
    }
    void offset(Point const &offset) override
    {
        origin.x += offset.x;
        origin.y += offset.y;
    }

    virtual std::unique_ptr<Shape> clone() const override
    {
        return std::make_unique<Text>(*this);
    }

    void setRotation(double angle) { rotation = angle; }

   private:
    Point origin;
    std::string content;
    Font font;
    double rotation;  // Rotation in degrees
};

// Sample charting class.
class LineChart : public Shape
{
   public:
    explicit LineChart(Size margin = Size(), double scale = 1,
                       Stroke const &axis_stroke = Stroke(.5, Color::Purple))
        : axis_stroke(axis_stroke), margin(margin), scale(scale)
    {
    }
    LineChart &operator<<(Polyline const &polyline)
    {
        if (polyline.points.empty()) return *this;

        polylines.push_back(polyline);
        return *this;
    }
    std::string toString(Layout const &layout) const override
    {
        if (polylines.empty()) return "";

        std::string ret;
        for (unsigned i = 0; i < polylines.size(); ++i)
            ret += polylineToString(polylines[i], layout);

        return ret + axisString(layout);
    }
    void offset(Point const &offset) override
    {
        for (unsigned i = 0; i < polylines.size(); ++i)
            polylines[i].offset(offset);
    }

    virtual std::unique_ptr<Shape> clone() const override
    {
        return std::make_unique<LineChart>(*this);
    }

   private:
    Stroke axis_stroke;
    Size margin;
    double scale;
    std::vector<Polyline> polylines;

    std::optional<Size> getSize() const
    {
        if (polylines.empty()) return std::nullopt;

        std::optional<Point> min = getMinPoint(polylines[0].points);
        std::optional<Point> max = getMaxPoint(polylines[0].points);
        for (unsigned i = 0; i < polylines.size(); ++i)
        {
            if (getMinPoint(polylines[i].points)->x < min->x)
                min->x = getMinPoint(polylines[i].points)->x;
            if (getMinPoint(polylines[i].points)->y < min->y)
                min->y = getMinPoint(polylines[i].points)->y;
            if (getMaxPoint(polylines[i].points)->x > max->x)
                max->x = getMaxPoint(polylines[i].points)->x;
            if (getMaxPoint(polylines[i].points)->y > max->y)
                max->y = getMaxPoint(polylines[i].points)->y;
        }

        return Size(max->x - min->x, max->y - min->y);
    }
    std::string axisString(Layout const &layout) const
    {
        std::optional<Size> size = getSize();
        if (!size) return "";

        // Make the axis 10% wider and higher than the data points.
        double width = size->width * 1.1;
        double height = size->height * 1.1;

        // Draw the axis.
        Polyline axis(Fill(Color::Transparent), axis_stroke);
        axis << Point(margin.width, margin.height + height)
             << Point(margin.width, margin.height)
             << Point(margin.width + width, margin.height);

        return axis.toString(layout);
    }
    std::string polylineToString(Polyline const &polyline,
                                 Layout const &layout) const
    {
        Polyline shifted_polyline = polyline;
        shifted_polyline.offset(Point(margin.width, margin.height));

        std::vector<Circle> vertices;
        for (unsigned i = 0; i < shifted_polyline.points.size(); ++i)
            vertices.push_back(Circle(shifted_polyline.points[i],
                                      getSize()->height / 30.0,
                                      Fill(Color::Black)));

        return shifted_polyline.toString(layout) +
               vectorToString(vertices, layout);
    }
};

class Group : public Shape
{
   public:
    explicit Group(std::string const &id = "") : id(id) {}

    // Copy constructor that performs a deep copy
    Group(const Group &other) : id(other.id)
    {
        for (const auto &child : other.shapes)
        {
            shapes.push_back(child->clone());
        }
    }

    std::string toString(Layout const &layout) const override
    {
        std::stringstream ss;
        ss << elemStart("g");
        if (!id.empty())
        {
            ss << attribute("id", id);
        }
        ss << ">\n";

        for (const auto &child : shapes)
        {
            ss << "\t" << child->toString(layout);
        }
        ss << "\t" << elemEnd("g");
        return ss.str();
    }

    void offset(Point const &offset) override
    {
        for (auto &child : shapes)
        {
            child->offset(offset);
        }
    }

    virtual std::unique_ptr<Shape> clone() const override
    {
        return std::make_unique<Group>(*this);
    }

    Group &operator<<(Shape const &shape)
    {
        shapes.push_back(shape.clone());
        return *this;
    }

    size_t size() const { return shapes.size(); }

    bool empty() const { return shapes.empty(); }

   private:
    std::string id;
    std::vector<std::unique_ptr<Shape>> shapes;
};

class Document
{
   public:
    explicit Document(std::string const &file_name,
                      const Layout &layout = Layout())
        : file_name(file_name), layout(layout)
    {
    }

    Document &operator<<(Shape const &shape)
    {
        body_nodes_str += shape.toString(layout);
        return *this;
    }
    std::string toString() const
    {
        std::stringstream ss;
        ss << "<?xml " << attribute("version", "1.0")
           << attribute("standalone", "no")
           << "?>\n<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" "
           << "\"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n<svg "
           << attribute("width", layout.size.width, "px")
           << attribute("height", layout.size.height, "px")
           << attribute("xmlns", "http://www.w3.org/2000/svg")
           << attribute("version", "1.1") << ">\n"
           << body_nodes_str << elemEnd("svg");
        return ss.str();
    }
    bool save() const
    {
        std::ofstream ofs(file_name.c_str());
        if (!ofs.good()) return false;

        ofs << toString();
        ofs.close();
        return true;
    }

    const std::string &filename() const { return file_name; }

   private:
    std::string file_name;
    Layout layout;

    std::string body_nodes_str;
};
}  // namespace svg

#endif
