#pragma once

#include "WndDesign/window/wnd_traits.h"
#include "WndDesign/figure/color.h"
#include "WndDesign/figure/shape.h"
#include "WndDesign/style/value_tag.h"
#include "WndDesign/geometry/clamp.h"
#include "WndDesign/message/mouse_tracker.h"


BEGIN_NAMESPACE(WndDesign)


struct SplitLine {
	float _width;
	Color _color;
	ValueTag _position;

	constexpr SplitLine() : SplitLine(5.0f, Color::Gray, 30pct) {}
	constexpr SplitLine(float width, Color color, ValueTag position) : _width(width), _color(color), _position(position) {}

	constexpr SplitLine& width(float width) { _width = width; return *this; }
	constexpr SplitLine& color(Color color) { _color = color; return *this; }
	constexpr SplitLine& position(ValueTag position) { _position = position; return *this; }
};


template<class Direction>
class SplitView;


template<>
class SplitView<Vertical> : public WndType<Assigned, Assigned> {
public:
	using child_ptr = child_ptr<Assigned, Assigned>;

public:
	SplitView(SplitLine split_line, child_ptr first, child_ptr second);

	// style
protected:
	SplitLine split_line;

	// child
protected:
	child_ptr first;
	child_ptr second;

	// layout
protected:
	Size size;
	float length_first = 0.0f;
	float length_second = 0.0f;
protected:
	Rect GetRegionFirst() const { return Rect(point_zero, Size(size.width, length_first)); }
	Rect GetRegionLine() const { return Rect(Point(0.0f, length_first), Size(size.width, split_line._width)); }
	Rect GetRegionSecond() const { return Rect(Point(0.0f, split_line._width + length_first), Size(size.width, length_second)); }
	Rect GetChildRegion(WndObject& child) const { return &child == first.get() ? GetRegionFirst() : GetRegionSecond(); }
protected:
	void UpdateLayout();
protected:
	virtual Size OnSizeRefUpdate(Size size_ref) override;
protected:
	virtual ref_ptr<WndObject> HitTest(Point& point) override;
	virtual Transform GetChildTransform(WndObject& child) const override;

	// paint
protected:
	virtual void OnChildRedraw(WndObject& child, Rect child_redraw_region) override;
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override;

	// message
protected:
	float mouse_down_position = 0.0f;
	MouseTracker mouse_tracker;
protected:
	virtual void OnMouseMsg(MouseMsg msg) override;
};


template<>
class SplitView<Horizontal> : public WndType<Assigned, Assigned> {
public:
	using child_ptr = child_ptr<Assigned, Assigned>;

public:
	SplitView(SplitLine split_line, child_ptr first, child_ptr second);

	// style
protected:
	SplitLine split_line;

	// child
protected:
	child_ptr first;
	child_ptr second;

	// layout
protected:
	Size size;
	float length_first = 0.0f;
	float length_second = 0.0f;
protected:
	Rect GetRegionFirst() const { return Rect(point_zero, Size(length_first, size.height)); }
	Rect GetRegionLine() const { return Rect(Point(length_first, 0.0f), Size(split_line._width, size.height)); }
	Rect GetRegionSecond() const { return Rect(Point(split_line._width + length_first, 0.0f), Size(length_second, size.height)); }
	Rect GetChildRegion(WndObject& child) const { return &child == first.get() ? GetRegionFirst() : GetRegionSecond(); }
protected:
	void UpdateLayout();
protected:
	virtual Size OnSizeRefUpdate(Size size_ref) override;
protected:
	virtual ref_ptr<WndObject> HitTest(Point& point) override;
	virtual Transform GetChildTransform(WndObject& child) const override;

	// paint
protected:
	virtual void OnChildRedraw(WndObject& child, Rect child_redraw_region) override;
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override;

	// message
protected:
	float mouse_down_position = 0.0f;
	MouseTracker mouse_tracker;
protected:
	virtual void OnMouseMsg(MouseMsg msg) override;
};


END_NAMESPACE(WndDesign)