#include "PairView.h"


BEGIN_NAMESPACE(WndDesign)


PairView<Vertical>::PairView(child_ptr first, child_ptr second) :
	first(std::move(first)), second(std::move(second)) {
	RegisterChild(this->first); RegisterChild(this->second);
}

Size PairView<Vertical>::OnSizeRefUpdate(Size size_ref) {
	if (width != size_ref.width) {
		width = size_ref.width;
		length_first = UpdateChildSizeRef(first, Size(width, length_min)).height;
		length_second = UpdateChildSizeRef(second, Size(width, length_min)).height;
	}
	return GetSize();
}

void PairView<Vertical>::OnChildSizeUpdate(WndObject& child, Size child_size) {
	if (&child == first.get()) {
		if (length_first != child_size.height) { length_first = child_size.height; SizeUpdated(GetSize()); }
	} else {
		if (length_second != child_size.height) { length_second = child_size.height; SizeUpdated(GetSize()); }
	}
}

ref_ptr<WndObject> PairView<Vertical>::HitTest(Point& point) {
	if (point.y < length_first) { return first; }
	point.y -= length_first; return second;
}

Transform PairView<Vertical>::GetChildTransform(WndObject& child) const {
	return GetChildRegion(child).point - point_zero;
}

void PairView<Vertical>::OnChildRedraw(WndObject& child, Rect child_redraw_region) {
	Rect child_region = GetChildRegion(child);
	Redraw(child_region.Intersect(child_redraw_region + (child_region.point - point_zero)));
}

void PairView<Vertical>::OnDraw(FigureQueue& figure_queue, Rect draw_region) {
	DrawChild(first, GetRegionFirst(), figure_queue, draw_region);
	DrawChild(second, GetRegionSecond(), figure_queue, draw_region);
}


PairView<Horizontal>::PairView(child_ptr first, child_ptr second) :
	first(std::move(first)), second(std::move(second)) {
	RegisterChild(this->first);	RegisterChild(this->second);
}

Size PairView<Horizontal>::OnSizeRefUpdate(Size size_ref) {
	if (height != size_ref.height) {
		height = size_ref.height;
		length_first = UpdateChildSizeRef(first, Size(length_min, height)).width;
		length_second = UpdateChildSizeRef(second, Size(length_min, height)).width;
	}
	return GetSize();
}

void PairView<Horizontal>::OnChildSizeUpdate(WndObject& child, Size child_size) {
	if (&child == first.get()) {
		if (length_first != child_size.width) { length_first = child_size.width; SizeUpdated(GetSize()); }
	} else {
		if (length_second != child_size.width) { length_second = child_size.width; SizeUpdated(GetSize()); }
	}
}

ref_ptr<WndObject> PairView<Horizontal>::HitTest(Point& point) {
	if (point.x < length_first) { return first; }
	point.x -= length_first; return second;
}

Transform PairView<Horizontal>::GetChildTransform(WndObject& child) const {
	return GetChildRegion(child).point - point_zero;
}

void PairView<Horizontal>::OnChildRedraw(WndObject& child, Rect child_redraw_region) {
	Rect child_region = GetChildRegion(child);
	Redraw(child_region.Intersect(child_redraw_region + (child_region.point - point_zero)));
}

void PairView<Horizontal>::OnDraw(FigureQueue& figure_queue, Rect draw_region) {
	DrawChild(first, GetRegionFirst(), figure_queue, draw_region);
	DrawChild(second, GetRegionSecond(), figure_queue, draw_region);
}


END_NAMESPACE(WndDesign)