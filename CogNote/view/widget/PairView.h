#pragma once

#include "WndDesign/window/wnd_traits.h"


BEGIN_NAMESPACE(WndDesign)


template<class Direction>
class PairView;


template<>
class PairView<Vertical> : public WndType<Assigned, Auto> {
public:
	using child_ptr = child_ptr<Assigned, Auto>;

public:
	PairView(child_ptr first, child_ptr second);

	// child
protected:
	child_ptr first;
	child_ptr second;

	// layout
protected:
	float width = 0.0f;
	float length_first = 0.0f;
	float length_second = 0.0f;
protected:
	Size GetSize() const { return Size(width, length_first + length_second); }
	Rect GetRegionFirst() const { return Rect(point_zero, Size(width, length_first)); }
	Rect GetRegionSecond() const { return Rect(Point(0.0f, length_first), Size(width, length_second)); }
	Rect GetChildRegion(WndObject& child) const { return &child == first.get() ? GetRegionFirst() : GetRegionSecond(); }
protected:
	virtual Size OnSizeRefUpdate(Size size_ref) override;
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override;
protected:
	virtual ref_ptr<WndObject> HitTest(Point& point) override;
	virtual Transform GetChildTransform(WndObject& child) const override;

	// paint
protected:
	virtual void OnChildRedraw(WndObject& child, Rect child_redraw_region) override;
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override;
};


template<>
class PairView<Horizontal> : public WndType<Auto, Assigned> {
public:
	using child_ptr = child_ptr<Auto, Assigned>;

public:
	PairView(child_ptr first, child_ptr second);

	// child
protected:
	child_ptr first;
	child_ptr second;

	// layout
protected:
	float height = 0.0f;
	float length_first = 0.0f;
	float length_second = 0.0f;
protected:
	Size GetSize() const { return Size(length_first + length_second, height); }
	Rect GetRegionFirst() const { return Rect(point_zero, Size(length_first, height)); }
	Rect GetRegionSecond() const { return Rect(Point(length_first, 0.0f), Size(length_second, height)); }
	Rect GetChildRegion(WndObject& child) const { return &child == first.get() ? GetRegionFirst() : GetRegionSecond(); }
protected:
	virtual Size OnSizeRefUpdate(Size size_ref) override;
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override;
protected:
	virtual ref_ptr<WndObject> HitTest(Point& point) override;
	virtual Transform GetChildTransform(WndObject& child) const override;

	// paint
protected:
	virtual void OnChildRedraw(WndObject& child, Rect child_redraw_region) override;
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override;
};


END_NAMESPACE(WndDesign)