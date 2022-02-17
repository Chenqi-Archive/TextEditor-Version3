#include "BlockListView.h"
#include "BlockView.h"

#include "WndDesign/figure/shape.h"


BEGIN_NAMESPACE(WndDesign)

BEGIN_NAMESPACE(Anonymous)

// selection
constexpr Color selection_color = Color(Color::DimGray, 0x7f);

ref_ptr<BlockListView> selection_focus = nullptr;
size_t selection_range_begin = 0;
size_t selection_range_end = 0;

Rect selection_region;

bool HasSelection() { return selection_range_end > selection_range_begin; }

// message
bool is_ctrl_down = false;
bool is_shift_down = false;

END_NAMESPACE(Anonymous)


BlockListView::BlockListView(BlockView& block_view) : ListLayout(0.0f), block_view(block_view) {}

BlockListView::~BlockListView() {}

BlockView& BlockListView::AsBlockView(child_ptr& child) { return static_cast<BlockView&>(static_cast<WndObject&>(child)); }

std::vector<std::unique_ptr<BlockView>> BlockListView::ExtractChild(size_t begin, size_t end) {
	std::vector<std::unique_ptr<BlockView>> block_view_list; block_view_list.reserve(end - begin);
	for (size_t i = begin; i < end; ++i) {
		UnregisterChild(child_list[i].child);
		block_view_list.emplace_back(static_cast<alloc_ptr<BlockView>>(child_list[i].child.release()));
	}
	EraseChild(begin, end - begin);
	return block_view_list;
}

size_t BlockListView::GetChildIndex(BlockView& child) { return GetChildData(child); }

Size BlockListView::OnSizeRefUpdate(Size size_ref) {
	bool need_update = size.width != size_ref.width;
	ListLayout::OnSizeRefUpdate(size_ref);
	if (need_update && selection_focus == this && HasSelection()) { UpdateSelectionRegion(selection_range_begin, selection_range_end); }
	return size;
}

void BlockListView::OnDraw(FigureQueue& figure_queue, Rect draw_region) {
	ListLayout::OnDraw(figure_queue, draw_region);
	if (selection_focus == this && HasSelection()) {
		figure_queue.add(selection_region.point, new Rectangle(selection_region.size, selection_color));
	}
}

void BlockListView::RedrawSelectionRegion() {
	if (selection_focus != nullptr) { selection_focus->Redraw(selection_region); }
}

void BlockListView::ClearSelection() {
	RedrawSelectionRegion();
	selection_focus = nullptr;
}

void BlockListView::UpdateSelectionRegion(size_t begin, size_t end) {
	SetFocus();
	RedrawSelectionRegion();
	selection_focus = this;	selection_range_begin = begin; selection_range_end = end;
	selection_region.point.x = 0.0f;
	selection_region.point.y = child_list[begin].BeginOffset();
	selection_region.size.width = size.width;
	selection_region.size.height = child_list[end - 1].EndOffset() - selection_region.point.y;
	RedrawSelectionRegion();
}

void BlockListView::SelectAll() {
	if (selection_range_begin == 0 && selection_range_end == child_list.size()) { return block_view.SelectAll(); }
	UpdateSelectionRegion(0, child_list.size());
}

void BlockListView::SelectChild(BlockView& child) {
	size_t index = GetChildIndex(child);
	UpdateSelectionRegion(index, index + 1);
}

void BlockListView::DoSelect(Point point) {
	auto it = HitTestItem(point.y);
	size_t index = it - child_list.begin();
	if (selection_begin == index) {
		point.y -= it->offset;
		AsBlockView(it->child).DoSelect(point);
	} else {
		size_t begin = selection_begin, end = index; if (end < begin) { std::swap(begin, end); }
		if (selection_focus == this && selection_range_begin == begin && selection_range_end == end + 1) { return; }
		UpdateSelectionRegion(begin, end + 1);
	}
}

void BlockListView::Insert(wchar ch) {

}

void BlockListView::Delete() {
	if (!HasSelection()) { return; }
	EraseChild(selection_range_begin, selection_range_end - selection_range_begin);
	ClearSelection();
}

void BlockListView::OnTab() {
	if (!HasSelection()) { return; }
	if (is_shift_down) {
		if (block_view.IsRootBlock()) { return; }
		block_view.InsertAfterSelf(ExtractChild(selection_range_begin, selection_range_end));
	} else {
		if (selection_range_begin == 0) { return; }
		GetChild(selection_range_begin - 1).InsertBack(ExtractChild(selection_range_begin, selection_range_end));
	}
}

void BlockListView::InsertAt(size_t index, std::wstring text) {
	InsertChild(index, new BlockView(block_view, std::move(text)));
	GetChild(index).SetCaret(0);
}

void BlockListView::InsertAt(size_t index, std::vector<std::wstring> text, size_t caret_pos) {
	std::vector<child_ptr> children; children.reserve(text.size());
	for (auto it = text.begin(); it != text.end(); ++it) {
		children.emplace_back(new BlockView(block_view, std::move(*it)));
	}
	InsertChild(index, std::move(children));
	GetChild(index + text.size() - 1).SetCaret(caret_pos);
}

void BlockListView::InsertAt(size_t index, std::vector<std::unique_ptr<BlockView>> block_view_list) {
	size_t end = index + block_view_list.size();
	for (auto& it : block_view_list) { it->parent = &block_view; }
	InsertChild(index, std::move(reinterpret_cast<std::vector<child_ptr>&>(block_view_list)));
	UpdateSelectionRegion(index, end);
}

void BlockListView::Cut() {
}

void BlockListView::Copy() {
}

void BlockListView::Paste() {
}

void BlockListView::OnKeyMsg(KeyMsg msg) {
	switch (msg.type) {
	case KeyMsg::KeyDown:
		switch (msg.key) {
		case Key::Backspace: Delete(); break;
		case Key::Delete: Delete(); break;

		case Key::Tab: OnTab(); break;

		case Key::Ctrl: is_ctrl_down = true; break;
		case Key::Shift: is_shift_down = true; break;

		case CharKey('A'): if (is_ctrl_down) { SelectAll(); } break;
		case CharKey('X'): if (is_ctrl_down) { Cut(); } break;
		case CharKey('C'): if (is_ctrl_down) { Copy(); } break;
		case CharKey('V'): if (is_ctrl_down) { Paste(); } break;
		}
		break;
	case KeyMsg::KeyUp:
		switch (msg.key) {
		case Key::Ctrl: is_ctrl_down = false; break;
		case Key::Shift: is_shift_down = false; break;
		}
		break;
	case KeyMsg::Char:
		if (is_ctrl_down) { break; }
		if (!iswcntrl(msg.ch)) { Insert(msg.ch); };
		break;
	}
}

void BlockListView::OnNotifyMsg(NotifyMsg msg) {
	switch (msg) {
	case NotifyMsg::LoseFocus: is_ctrl_down = false; is_shift_down = false; block_view.ClearSelection(); break;
	}
}


END_NAMESPACE(WndDesign)