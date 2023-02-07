#ifndef CQPropertyViewFilter_H
#define CQPropertyViewFilter_H

#include <QSortFilterProxyModel>
#include <set>

class CQPropertyViewTree;
class CQPropertyViewItem;

/*!
 * \brief proxy model for filter/sort of property model
 */
class CQPropertyViewFilter : public QSortFilterProxyModel {
  Q_OBJECT

  Q_PROPERTY(QString filter      READ filter        WRITE setFilter     )
  Q_PROPERTY(bool    matchPath   READ isMatchPath   WRITE setMatchPath  )
  Q_PROPERTY(bool    matchParent READ isMatchParent WRITE setMatchParent)

  // model indices are from source model (propertyModel)
 public:
  CQPropertyViewFilter(CQPropertyViewTree *view);

  CQPropertyViewTree *view() const { return view_; }

  const QString &filter() const { return filter_; }
  void setFilter(const QString &filter);

  bool isMatchPath() const { return matchPath_; }
  void setMatchPath(bool b) { matchPath_ = b; }

  bool isMatchParent() const { return matchParent_; }
  void setMatchParent(bool b) { matchParent_ = b; }

  bool filterAcceptsRow(int row, const QModelIndex &parent) const override;

  bool acceptsItem(const QModelIndex &ind) const;

  bool anyChildMatch(const QModelIndex &parent) const;

#if 0
  void expandMatches();
#endif

 private:
  bool itemMatch(CQPropertyViewItem *item) const;

 private:
  using IndexMatches = std::map<QModelIndex, bool>;
  using ExpandInds   = std::set<QModelIndex>;

  CQPropertyViewTree*  view_        { nullptr };
  QString              filter_;
  QRegExp              regexp_;
  bool                 matchPath_   { true };
  bool                 matchParent_ { false };
  mutable IndexMatches matches_;
  mutable ExpandInds   expand_;
};

#endif
