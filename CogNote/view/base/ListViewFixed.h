#pragma once

#include "WndDesign/window/wnd_traits.h"

#include <vector>


BEGIN_NAMESPACE(WndDesign)


class ListViewFixed : public WndType<Assigned, Auto> {
public:
	using child_ptr = child_ptr<Assigned, Assigned>;

public:
	ListViewFixed(float length, float gap) : length(length), gap(gap) {}

	// child
protected:
	std::vector<child_ptr> child_list;
protected:
	void SetChildData(WndObject& child, size_t index) { WndObject::SetChildData<size_t>(child, index); }
	size_t GetChildData(WndObject& child) const { return WndObject::GetChildData<size_t>(child); }
public:
	void InsertChild(size_t index, child_ptr child);
	void EraseChild(size_t begin, size_t count);
	void AppendChild(child_ptr child) { InsertChild(-1, std::move(child)); }

	// layout
protected:
	float length;
	float gap;
	Size size;
protected:
	float GetChildOffset(size_t index) const { return index * (length + gap); }
	Rect GetChildRegion(size_t index) const { return Rect(Point(0.0f, GetChildOffset(index)), Size(size.width, length)); }
	Rect GetChildRegion(WndObject& child) const { return GetChildRegion(GetChildData(child)); }
	size_t HitTestItem(float offset) const { return (size_t)(offset / (length + gap)); }
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
};


END_NAMESPACE(WndDesign)