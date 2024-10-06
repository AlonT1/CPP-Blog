#include <iostream>
#include <vector>


/*

THIS IS NOT STATIC POLYMORPHISM BECAUSE WE UTILIZE RTTI!

Note: the full explanation path (project names): 
static_dynamic_dispatch, double dispatch (here + diagram of visitor pattern), crtp, std::vist

****************************************************************************************
The best way to tackle visitor pattern /double dispatch is either via the example
shown below or with std::visit
****************************************************************************************

Explanation:
Since double dispatch is the main mechanism on top of which Visitor pattern is built - 
they will be explained together:


tl;dr
visitor pattern enables adding new functionality to classes with minimal changes
to those functions. We want to export info about shapes to xml,
but don't want to implement export() in each shape, risk breaking existing shape classes. 
naive solution suggests having seperate XMLExporter class with xml_exporter.export(shape)
but cpp (and other programming languages) can't utilize the run-time type of
polymorphic ptrs/refs as arguments: xml_exporter.export(shape): assuming shape is 
a pointer to Circle the call will always default to Shape! (although not sliced,
calling shape->draw() will still invoke draw of Circle).

**************************************************************************************
by using the Visitor Pattern which utilizes Double dispatch we switch POV'S and supply the 
exporter to the shape:  shape.accept_visitor(v) (1st dispatch):
Thanks to RTTI, shape is now deciphered  as Circle - we reach the Circle class.
v is the actual exporter object  (it "visits" the Rect/Circle).
From circle we invoke v.visit_circle(*this) (2nd dispatch), a function of the xmlexporter,
essentially we supply the shape back to the exporter with *this which is 100% a circle object.
from there visit_circle of xmlexporter simply invokes exporter(shape& s).
**************************************************************************************


Long version:
Visitor is a behavioral design pattern that can introduce a new behavior to a class without modifying that
class (or at least minimally affecting it...)
The diagram explained:
Circle and Rectangle inherit from Shape. We want to export these shapes (their size) to an XML.
The main purpose of Shape objects is to be drawn on screen with various sizes, thus exportXML() 
will feel alien in these classes. And what if we want to implement other export 
options such as exportJPG? these classes will explode.

Naive Solution (that doesn't work):
assume XmlExporter has 3 functions: export(Shape&), export(Circle&), export(Rectangle&)/
for each shape in array of Shape* containing Rectangles and Circles
xml_exporter.export(*shape)
where export accepts a polymorphic shape reference (can accept Rectangle/Circle...)

This doesn't work. Why? because functions are bound at compile time, where the real time type of "shape" 
(Rectangle/Circle, retrievable by RTTI mechanism) isn't available. Therefore the compiler defaults back to 
the underlying type of shape (type Shape) that IS available at compile time. So instead of calling export(Circle),
the compiler calls export(Shape s).  Note that the object isn't sliced, the 
"Circle-ness" is still there, calling s->draw()
will still invoke draw of Circle.

Remember, in cpp:
RTTI can be utilized to call the correct function through a polymorphic pointer
 e.g: Shape* shape = &Circle;  shape->draw() invokes draw() of Circle, and not draw() of Shape
(assuming Circle inherits from Shape + Circle overrides virtual func draw() of Shape)
RTTI cannot be utilized when the argument is a polymorphic ptr/ref - as seen above with shape ptr that 
can represent multiple symbols (Rectangle, Circle).

Solution:
instead of supplying a polymorphic pointer to xml_exporter (has limitiations as described above), we will
supply the xml_exporter to the polymoprhic pointer!
so instead of xml_exporter.export(Shape), we will perform shape->accept_visitor(Visitor& v). Explained:
1. first of all RTTI WILL work now (described above), therefre we will call the 
correct run-time type of shape (Rectangle or Circle)
2. the visitor object is any object that implements the Visitor interface, essentially any class 
that wants to add functionality to Rect/Circle, without breaking existing code. In our case XmlExporterVisitor.
3. Our Shapes Retc/Circle implement accept_visitor where they can accept any Visitor object. 
Once we are in Circle/Shape - we call the visitor via v.visit_circle(*this). Note: once we are in Circle 
class then *this is always a Circle! the XmlExporterVisitor now knows that it deals with a Circle Object.
4. back in XmlExporterVisitor, visit_circle(Circle& c)  simply invokes export_shape(c).

This is also called double dispatch:
in the 1st dispatch (1) we deciphered the true identity of shape using RTTI - we are now either
in Circle or Rectangle.
in the 2nd dispatch (3), once we know we are handling a Circle/Rectangle object, we make contact
with xml exporter and supplying him with "*this" which is 100% either circle or rectangle,
now xmlexporter can export the shape.

Notes:
1. CPP implements this pattern via std::visit.
2. A shorter version of the one displayed above can be achieved via crtp
*/


struct Circle;
struct Rectangle;

struct Visitor
{
	virtual void visit_circle(Circle& c) = 0;
	virtual void visit_rectangle(Rectangle& c) = 0;
};

struct Shape
{
	int size = 0;
	virtual void accept_visitor(Visitor& v) = 0;
	virtual ~Shape() {}
};

struct Circle : public Shape
{
	//2nd dispatch - here we know they type of the shape "Circle"
	void accept_visitor(Visitor& v) override { v.visit_circle(*this); } 
};

struct Rectangle : public Shape
{
	void accept_visitor(Visitor& v) override { v.visit_rectangle(*this); } //2nd dispatch
};


struct XmlExporterVisitor : public Visitor
{
	void visit_circle(Circle& c) override { export_shape(c); }
	void visit_rectangle(Rectangle& r) override { export_shape(r); }
	void export_shape(Shape& s) { std::cout << "exporting " << typeid(s).name() << '\n'; }
};

int main()
{
	// we flipped the positions: instead of the exporter getting objects 
	// and exporting them (problematic because polymorphic arguments aren't allowed in cpp, i.e
	// exporter can't accept Shape pointer that points to Circle or Rectangle - when the pointer
	// is passed, only its compile time remains (Shape*).
	// Instead, The objects get the exporter as a visitor
	// (the visitor visits the object), and by working from the perspective of the objects, their
	// types can be revealed simply by using "this".
	XmlExporterVisitor xml_exporter_visitor;

	std::vector<Shape*> shapes{ new Rectangle{}, new Circle{} };
	for (Shape* s : shapes)
	{
		s->accept_visitor(xml_exporter_visitor); //1st dispatch
	}
}