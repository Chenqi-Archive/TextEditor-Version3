#pragma once

#include "DropDownButton.h"


BEGIN_NAMESPACE(WndDesign)


class DropDownFrame : public WndType<Assigned, Auto> {
public:
	using child_ptr = child_ptr<Assigned, Auto>;

public:
	DropDownFrame(child_ptr child) : child(std::move(child)) {
		RegisterChild(button); RegisterChild(this->child);
		length_first = UpdateChildSizeRef(button, size_empty).width;
	}

	// child
protected:
	DropDownButton button;
	child_ptr child;

	// layout
private:
	Size size;
	float length_first = 0.0f;
	float length_second = 0.0f;
private:
	Rect GetRegionFirst() const { return Rect(point_zero, Size(length_first, size.height)); }
	Rect GetRegionSecond() const { return Rect(Point(length_first, 0.0f), Size(length_second, size.height)); }
	Rect GetChildRegion(WndObject& child) const { return &child == &button ? GetRegionFirst() : GetRegionSecond(); }
protected:
	virtual Size OnSizeRefUpdate(Size size_ref) override {
		if (size.width != size_ref.width) {
			size.width = size_ref.width;
			length_second = max(0.0f, size.width - length_first);
			size.height = UpdateChildSizeRef(child, Size(length_second, length_max)).height;
			UpdateChildSizeRef(button, Size(length_first, size.height));
		}
		return size;
	}
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override {
		if (&child == this->child.get() && size.height != child_size.height) {
			size.height = child_size.height;
			UpdateChildSizeRef(button, Size(length_first, size.height));
			SizeUpdated(size);
		}
	}
protected:
	virtual ref_ptr<WndObject> HitTest(Point& point) override {
		if (point.x < length_first) { return &button; }
		point.x -= length_first;
		if (point.x < length_second) { return child; }
		return nullptr;
	}
	virtual Transform GetChildTransform(WndObject& child) const override {
		return GetChildRegion(child).point - point_zero;
	}

	// paint
protected:
	virtual void OnChildRedraw(WndObject& child, Rect child_redraw_region) override {
		Rect child_region = GetChildRegion(child);
		Redraw(child_region.Intersect(child_redraw_region + (child_region.point - point_zero)));
	}
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override {
		DrawChild(button, GetRegionFirst(), figure_queue, draw_region);
		DrawChild(child, GetRegionSecond(), figure_queue, draw_region);
	}

	// message
private:
	virtual void OnNotifyMsg(NotifyMsg msg) override {
		switch (msg) {
		case NotifyMsg::MouseEnter: button.is_mouse_over = true; button.Redraw(region_infinite); break;
		case NotifyMsg::MouseLeave: button.is_mouse_over = false; button.Redraw(region_infinite); break;
		}
	}
};


END_NAMESPACE(WndDesign)