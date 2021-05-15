#include <CQPropertyViewFilter.h>
#include <CQPropertyViewTree.h>
#include <CQPropertyViewModel.h>
#include <CQPropertyViewItem.h>
#include <cassert>

CQPropertyViewFilter::
CQPropertyViewFilter(CQPropertyViewTree *view) :
 view_(view) {
}

bool
CQPropertyViewFilter::
filterAcceptsRow(int row, const QModelIndex &parent) const
{
  if (filter_.length() == 0)
    return true;

  //auto *parentItem = view_->getModelItem(parent, /*map*/false);
  //if (! parentItem) return true;

  auto *model = view_->propertyModel();

  auto child = model->index(row, 0, parent);

  auto *item = view_->getModelItem(child, /*map*/false);
  if (! item) return false;

  if (! item->object()) {
    if (! anyChildMatch(child))
      return false;

    expand_.insert(child);

    //view_->setExpanded(child, true);

    return true;
  }
  else {
    return acceptsItem(child);
  }
}

bool
CQPropertyViewFilter::
acceptsItem(const QModelIndex &ind) const
{
  auto p = matches_.find(ind);

  if (p != matches_.end())
    return (*p).second;

  auto *item = view_->getModelItem(ind, /*map*/false);
  if (! item) return false;

  bool match = true;

  if (item->object())
    match = itemMatch(item);
//std::cerr << "Match: " << (match ? "Y " : "N ") << item->path(".").toStdString() << std::endl;

  matches_[ind] = match;

  return match;
}

bool
CQPropertyViewFilter::
itemMatch(CQPropertyViewItem *item) const
{
  QString path;

  if (isMatchPath())
    path = item->path(".", /*alias*/true);
  else
    path = item->aliasName();

  return regexp_.exactMatch(path);
}

bool
CQPropertyViewFilter::
anyChildMatch(const QModelIndex &parent) const
{
  auto *model = view_->propertyModel();

  for (int i = 0; i < model->rowCount(parent); ++i) {
    auto child = model->index(i, 0, parent);

    auto *item = view_->getModelItem(child, /*map*/false);
    assert(item);

    if (! item->object()) {
      if (anyChildMatch(child))
        return true;
    }
    else {
      if (acceptsItem(child))
        return true;
    }
  }

  return false;
}

#if 0
void
CQPropertyViewFilter::
expandMatches()
{
  auto *filterModel = view_->filterModel();

  std::vector<QModelIndex> inds;

  for (const auto &ind : expand_) {
    auto ind1 = filterModel->mapFromSource(ind);

    inds.push_back(ind1);
  }

  for (int i = inds.size() - 1; i >= 0; --i) {
    view_->setExpanded(inds[i], true);
  }
}
#endif

void
CQPropertyViewFilter::
setFilter(const QString &filter)
{
  matches_.clear();
  expand_ .clear();

  filter_ = filter;

  if (filter_.length() && filter_[filter_.length() - 1] != '*')
    filter_ += "*";

  if (filter_.length() && filter_[0] != '*')
    filter_ = "*" + filter_;

  regexp_ = QRegExp(filter_, Qt::CaseSensitive, QRegExp::Wildcard);

  invalidateFilter();
  //invalidate();

  view_->expandAll();

  //expandMatches();
}
