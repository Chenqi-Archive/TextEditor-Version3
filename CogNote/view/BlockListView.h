#pragma once

#include "WndDesign/layout/ListLayout.h"


BEGIN_NAMESPACE(WndDesign)

class BlockView;


class BlockListView : public ListLayout<Vertical> {
private:
	friend class BlockView;

private:
	BlockListView();

	// paint
private:
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override;

	// selection
private:
	static void RedrawSelectionRegion();
private:
	void SelectChildRange(size_t begin, size_t end);

	// message
private:
	virtual void OnKeyMsg(KeyMsg msg) override;
};


END_NAMESPACE(WndDesign)