#include "SplitView.h"


BEGIN_NAMESPACE(WndDesign)


SplitView<Vertical>::SplitView(SplitLine split_line, child_ptr first, child_ptr second) :
	split_line(split_line), first(std::move(first)), second(std::move(second)) {
	cursor = Cursor::Resize90;
	RegisterChild(this->first);
	RegisterChild(this->second);
}

void SplitView<Vertical>::UpdateLayout() {
	ValueTag split_position_tag = split_line._position;
	float split_position = split_position_tag.ConvertToPixel(size.height).value();
	length_first = clamp(split_position - split_line._width / 2, 0.0f, max(0.0f, size.height - split_line._width));
	length_second = max(0.0f, size.height - length_first - split_line._width);
	split_position = length_first + split_line._width / 2;
	split_line.position(split_line._position.IsPercent() ? pct(split_position * 100 / size.height) : px(split_position));
	UpdateChildSizeRef(first, GetRegionFirst().size);
	UpdateChildSizeRef(second, GetRegionSecond().size);
}

Size SplitView<Vertical>::OnSizeRefUpdate(Size size_ref) {
	if (size != size_ref) { size = size_ref; UpdateLayout(); }
	return size;
}

ref_ptr<WndObject> SplitView<Vertical>::HitTest(Point& point) {
	float offset = point.y;
	if (offset < 0.0f) { return nullptr; }
	if (offset < length_first) { return first; }
	if (offset < length_first + split_line._width) { return this; }
	if (offset < size.height) { point.y -= length_first + split_line._width; return second; }
	return nullptr;
}

Transform SplitView<Vertical>::GetChildTransform(WndObject& child) const {
	return GetChildRegion(child).point - point_zero;
}

void SplitView<Vertical>::OnChildRedraw(WndObject& child, Rect child_redraw_region) {
	Rect child_region = GetChildRegion(child);
	Redraw(child_region.Intersect(child_redraw_region + (child_region.point - point_zero)));
}

void SplitView<Vertical>::OnDraw(FigureQueue& figure_queue, Rect draw_region) {
	DrawChild(first, GetRegionFirst(), figure_queue, draw_region);
	DrawChild(second, GetRegionSecond(), figure_queue, draw_region);
	Rect region_line = GetRegionLine();
	figure_queue.add(region_line.point, new Rectangle(region_line.size, split_line._color));
}

void SplitView<Vertical>::OnMouseMsg(MouseMsg msg) {
	switch (msg.type) {
	case MouseMsg::LeftDown: mouse_down_position = split_line._position.value(); SetCapture(); break;
	case MouseMsg::LeftUp: ReleaseCapture(); break;
	}
	switch (mouse_tracker.Track(msg)) {
	case MouseTrackMsg::LeftDrag:
		split_line._position.value() = split_line._position.IsPercent() ?
			mouse_down_position + (msg.point.y - mouse_tracker.mouse_down_position.y) * 100 / size.height :
			mouse_down_position + msg.point.y - mouse_tracker.mouse_down_position.y;
		UpdateLayout(); Redraw(region_infinite);
	}
}


SplitView<Horizontal>::SplitView(SplitLine split_line, child_ptr first, child_ptr second) : split_line(split_line), first(std::move(first)), second(std::move(second)) {
	cursor = Cursor::Resize0;
	RegisterChild(this->first);
	RegisterChild(this->second);
}

void SplitView<Horizontal>::UpdateLayout() {
	ValueTag split_position_tag = split_line._position;
	float split_position = split_position_tag.ConvertToPixel(size.width).value();
	length_first = clamp(split_position - split_line._width / 2, 0.0f, max(0.0f, size.width - split_line._width));
	length_second = max(0.0f, size.width - length_first - split_line._width);
	split_position = length_first + split_line._width / 2;
	split_line.position(split_line._position.IsPercent() ? pct(split_position * 100 / size.width) : px(split_position));
	UpdateChildSizeRef(first, GetRegionFirst().size);
	UpdateChildSizeRef(second, GetRegionSecond().size);
}

Size SplitView<Horizontal>::OnSizeRefUpdate(Size size_ref) {
	if (size != size_ref) { size = size_ref; UpdateLayout(); }
	return size;
}

ref_ptr<WndObject> SplitView<Horizontal>::HitTest(Point& point) {
	float offset = point.x;
	if (offset < 0.0f) { return nullptr; }
	if (offset < length_first) { return first; }
	if (offset < length_first + split_line._width) { return this; }
	if (offset < size.width) { point.x -= length_first + split_line._width; return second; }
	return nullptr;
}

Transform SplitView<Horizontal>::GetChildTransform(WndObject& child) const {
	return GetChildRegion(child).point - point_zero;
}

void SplitView<Horizontal>::OnChildRedraw(WndObject& child, Rect child_redraw_region) {
	Rect child_region = GetChildRegion(child);
	Redraw(child_region.Intersect(child_redraw_region + (child_region.point - point_zero)));
}

void SplitView<Horizontal>::OnDraw(FigureQueue& figure_queue, Rect draw_region) {
	DrawChild(first, GetRegionFirst(), figure_queue, draw_region);
	DrawChild(second, GetRegionSecond(), figure_queue, draw_region);
	Rect region_line = GetRegionLine();
	figure_queue.add(region_line.point, new Rectangle(region_line.size, split_line._color));
}

void SplitView<Horizontal>::OnMouseMsg(MouseMsg msg) {
	switch (msg.type) {
	case MouseMsg::LeftDown: mouse_down_position = split_line._position.value(); SetCapture(); break;
	case MouseMsg::LeftUp: ReleaseCapture(); break;
	}
	switch (mouse_tracker.Track(msg)) {
	case MouseTrackMsg::LeftDrag:
		split_line._position.value() = split_line._position.IsPercent() ?
			mouse_down_position + (msg.point.x - mouse_tracker.mouse_down_position.x) * 100 / size.width :
			mouse_down_position + msg.point.x - mouse_tracker.mouse_down_position.x;
		UpdateLayout(); Redraw(region_infinite);
	}
}


END_NAMESPACE(WndDesign)