#pragma once

#include "WndDesign/layout/ListLayout.h"


BEGIN_NAMESPACE(WndDesign)

class BlockView;


class BlockListView : public ListLayout<Vertical> {
public:
	BlockListView(BlockView& block_view);
	~BlockListView();

	// context
private:
	BlockView& block_view;

	// child
private:
	BlockView& AsBlockView(child_ptr& child);
	BlockView& GetChild(size_t index) { return AsBlockView(child_list[index].child); }
	std::vector<std::unique_ptr<BlockView>> ExtractChild(size_t begin, size_t end);
public:
	size_t GetChildIndex(BlockView& child);

	// layout
private:
	virtual Size OnSizeRefUpdate(Size size_ref) override;
	virtual ref_ptr<WndObject> HitTest(Point& point) override;

	// paint
private:
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override;

	// selection
private:
	static void RedrawSelectionRegion();
public:
	static void ClearSelection();
public:
	size_t selection_begin = 0;
private:
	bool HitTestSelection(Point point);
	void UpdateSelectionRegion(size_t begin, size_t end);
	void SelectAll();
public:
	void SelectChild(BlockView& child);
	void DoSelect(Point point);

	// drag and drop
private:
	static void RedrawDragDropCaretRegion();
public:
	ref_ptr<BlockView> DoTextDragDrop(Point point);
	ref_ptr<BlockView> DoDragDrop(Point point);
	void FinishDragDrop(BlockListView& list_view);
	void CancelDragDrop();

	// input
private:
	void Insert(wchar ch);
	void Delete();
private:
	void OnTab();
private:
	void InsertAt(size_t index, std::wstring text);
	void InsertAt(size_t index, std::vector<std::wstring> text, size_t caret_pos);
	void InsertAt(size_t index, std::vector<std::unique_ptr<BlockView>> block_view_list);
public:
	void InsertFront(std::wstring text) { InsertAt(0, text); }
	void InsertFront(std::vector<std::wstring> text, size_t caret_pos) { InsertAt(0, text, caret_pos); }
	void InsertBack(std::vector<std::unique_ptr<BlockView>> block_view_list) { InsertAt(child_list.size(), std::move(block_view_list)); }
	void InsertAfter(BlockView& child, std::wstring text) { InsertAt(GetChildIndex(child) + 1, text); }
	void InsertAfter(BlockView& child, std::vector<std::wstring> text, size_t caret_pos) { InsertAt(GetChildIndex(child) + 1, text, caret_pos); }
	void InsertAfter(BlockView& child, std::vector<std::unique_ptr<BlockView>> block_view_list) { InsertAt(GetChildIndex(child) + 1, std::move(block_view_list)); }

	// clipboard
private:
	void Cut();
	void Copy();
	void Paste();

	// message
private:
	virtual void OnMouseMsg(MouseMsg msg) override;
	virtual void OnKeyMsg(KeyMsg msg) override;
	virtual void OnNotifyMsg(NotifyMsg msg) override;
};


END_NAMESPACE(WndDesign)