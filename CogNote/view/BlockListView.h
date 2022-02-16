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
public:
	size_t GetChildIndex(BlockView& child);

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
	void UpdateSelectionRegion(size_t begin, size_t end);
	void SelectAll();
public:
	void SelectChild(BlockView& child);
	void DoSelect(Point point);

	// input
private:
	void Insert(wchar ch);
	void Insert(std::vector<std::wstring> text);
	void Delete();
public:
	void InsertNewFront();
	void InsertAt(size_t index, std::vector<std::wstring> text, size_t caret_pos);
	void InsertAfter(BlockView& child, std::vector<std::wstring> text, size_t caret_pos) { InsertAt(GetChildIndex(child) + 1, text, caret_pos); }

	// clipboard
private:
	void Cut();
	void Copy();
	void Paste();

	// message
private:
	virtual void OnKeyMsg(KeyMsg msg) override;
	virtual void OnNotifyMsg(NotifyMsg msg) override;
};


END_NAMESPACE(WndDesign)