#include "ListViewFixed.h"


BEGIN_NAMESPACE(WndDesign)


void ListViewFixed::InsertChild(size_t index, child_ptr child) {
	if (index > child_list.size()) { index = child_list.size(); }
	RegisterChild(child); SetChildData(child, index);
	auto it = child_list.emplace(child_list.begin() + index, std::move(child));
	UpdateChildSizeRef(*it, Size(size.width, length));
	for (; it != child_list.end(); ++it) { SetChildData(*it, it - child_list.begin()); }
	UpdateLayout();
}

void ListViewFixed::EraseChild(size_t begin, size_t count) {
	if (begin > child_list.size() || count == 0) { return; }
	size_t end = begin + count; if (end > child_list.size()) { end = child_list.size(); }
	auto it = child_list.erase(child_list.begin() + begin, child_list.begin() + end);
	for (; it != child_list.end(); ++it) { SetChildData(*it, it - child_list.begin()); }
	UpdateLayout();
}

void ListViewFixed::UpdateLayout() {
	size.height = child_list.size() * (length + gap) - (child_list.empty() ? 0 : gap);
	SizeUpdated(size);
}

Size ListViewFixed::OnSizeRefUpdate(Size size_ref) {
	if (size.width != size_ref.width) {
		size.width = size_ref.width;
		for (auto& child : child_list) { UpdateChildSizeRef(child, Size(size.width, length)); }
	}
	return size;
}

ref_ptr<WndObject> ListViewFixed::HitTest(Point& point) {
	float offset = point.y;
	if (offset < 0.0f || offset >= size.height) { return this; }
	size_t index = HitTestItem(offset); offset -= GetChildOffset(index);
	if (offset >= length) { return this; }
	point.y = offset; return child_list[index];
}

Transform ListViewFixed::GetChildTransform(WndObject& child) const {
	return GetChildRegion(child).point - point_zero;
}

void ListViewFixed::OnChildRedraw(WndObject& child, Rect child_redraw_region) {
	Rect child_region = GetChildRegion(child);
	Redraw(child_region.Intersect(child_redraw_region + (child_region.point - point_zero)));
}

void ListViewFixed::OnDraw(FigureQueue& figure_queue, Rect draw_region) {
	draw_region = draw_region.Intersect(Rect(point_zero, size)); if (draw_region.IsEmpty()) { return; }
	size_t begin = HitTestItem(draw_region.top()), end = HitTestItem(ceilf(draw_region.bottom()) - 1.0f);
	for (size_t index = begin; index <= end; ++index) {
		DrawChild(child_list[index], GetChildRegion(index), figure_queue, draw_region);
	}
}


END_NAMESPACE(WndDesign)