#include "BlockView.h"
#include "BlockTextView.h"
#include "BlockListView.h"

#include "widget/DropDownFrame.h"


BEGIN_NAMESPACE(WndDesign)


BlockView::BlockView() : PairView{
	new DropDownFrame{
		new BlockTextView()
	},
	new BlockListView()
} {

}


END_NAMESPACE(WndDesign)