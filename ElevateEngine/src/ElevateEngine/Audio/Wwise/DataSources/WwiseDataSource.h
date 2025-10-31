#pragma once

#include <vector>

struct WwiseItem;

class WwiseDataSource
{
public:
	virtual void InitializeSource() = 0;
	virtual void RefreshSource() = 0;
	virtual std::weak_ptr<WwiseItem> GetItems() const { return m_treeRoot; }

protected:
	WwiseDataSource() = default;

	std::shared_ptr<WwiseItem> m_treeRoot;
};