#include <CQPropertyViewTree.h>
#include <CQPropertyViewFilter.h>
#include <CQPropertyViewModel.h>
#include <CQPropertyViewDelegate.h>
#include <CQPropertyViewItem.h>
#include <CQHeaderView.h>

#include <QApplication>
#include <QHeaderView>
#include <QMouseEvent>
#include <QClipboard>
#include <QMenu>
#include <set>
#include <iostream>

CQPropertyViewTree::
CQPropertyViewTree(QWidget *parent, CQPropertyViewModel *model) :
 QTreeView(parent), model_(model)
{
  setObjectName("propertyView");

  //--

  filter_ = new CQPropertyViewFilter(this);

  connect(model_, SIGNAL(valueChanged(QObject *, const QString &)),
          this, SIGNAL(valueChanged(QObject *, const QString &)));
  connect(model_, SIGNAL(valueChanged(QObject *, const QString &)),
          this, SLOT(redraw()));

  filter_->setSourceModel(model_);

  setModel(filter_);

  //--

  setHeader(new CQHeaderView(this));

  header()->setStretchLastSection(true);
//header()->setSectionResizeMode(QHeaderView::Interactive);
//header()->setSectionResizeMode(QHeaderView::ResizeToContents);

  //--

  setSelectionMode(ExtendedSelection);

  setUniformRowHeights(true);

  setAlternatingRowColors(true);

  setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);

  //--

  delegate_ = new CQPropertyViewDelegate(this);

  setItemDelegate(delegate_);

  connect(delegate_, SIGNAL(closeEditor(QWidget*, QAbstractItemDelegate::EndEditHint)),
          this, SLOT(closeEditorSlot(QWidget*, QAbstractItemDelegate::EndEditHint)));

  //--

  connect(this, SIGNAL(clicked(const QModelIndex &)),
          this, SLOT(itemClickedSlot(const QModelIndex &)));

  QItemSelectionModel *sm = this->selectionModel();

  connect(sm, SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
          this, SLOT(itemSelectionSlot()));

  //---

  setContextMenuPolicy(Qt::CustomContextMenu);

  connect(this, SIGNAL(customContextMenuRequested(const QPoint&)),
          this, SLOT(customContextMenuSlot(const QPoint&)));
}

CQPropertyViewTree::
~CQPropertyViewTree()
{
  delete filter_;
}

void
CQPropertyViewTree::
setPropertyModel(CQPropertyViewModel *model)
{
  assert(model);

  if (model_) {
    disconnect(model_, SIGNAL(valueChanged(QObject *, const QString &)),
               this, SIGNAL(valueChanged(QObject *, const QString &)));
    disconnect(model_, SIGNAL(valueChanged(QObject *, const QString &)),
               this, SLOT(redraw()));
  }

  model_ = model;

  connect(model_, SIGNAL(valueChanged(QObject *, const QString &)),
          this, SIGNAL(valueChanged(QObject *, const QString &)));
  connect(model_, SIGNAL(valueChanged(QObject *, const QString &)),
          this, SLOT(redraw()));

  filter_->setSourceModel(model_);

  setModel(filter_);
}

void
CQPropertyViewTree::
setMouseHighlight(bool b)
{
  mouseHighlight_ = b;

  setMouseTracking(mouseHighlight_);
}

void
CQPropertyViewTree::
setFilter(const QString &filter)
{
  filter_->setFilter(filter);
}

void
CQPropertyViewTree::
modelResetSlot()
{
  //std::cerr << "model reset" << std::endl;
}

void
CQPropertyViewTree::
redraw()
{
  viewport()->update();
}

void
CQPropertyViewTree::
clear()
{
  model_->clear();
}

void
CQPropertyViewTree::
addProperty(const QString &path, QObject *obj, const QString &name, const QString &alias)
{
  model_->addProperty(path, obj, name, alias);
}

bool
CQPropertyViewTree::
setProperty(QObject *object, const QString &path, const QVariant &value)
{
  bool rc = model_->setProperty(object, path, value);

  redraw();

  return rc;
}

bool
CQPropertyViewTree::
getProperty(const QObject *object, const QString &path, QVariant &value) const
{
  return model_->getProperty(object, path, value);
}

void
CQPropertyViewTree::
selectObject(const QObject *obj)
{
  CQPropertyViewItem *root = model_->root();

  for (int i = 0; i < model_->numItemChildren(root); ++i) {
    CQPropertyViewItem *item = model_->itemChild(root, i);

    if (selectObject(item, obj))
      return;
  }
}

bool
CQPropertyViewTree::
selectObject(CQPropertyViewItem *item, const QObject *obj)
{
  QObject *obj1 = item->object();

  if (obj1 == obj) {
    if (item->parent()) {
      selectItem(item->parent(), true);
      return true;
    }
  }

  for (int i = 0; i < model_->numItemChildren(item); ++i) {
    CQPropertyViewItem *item1 = model_->itemChild(item, i);

    if (selectObject(item1, obj))
      return true;
  }

  return false;
}

void
CQPropertyViewTree::
deselectAllObjects()
{
  QItemSelectionModel *sm = this->selectionModel();

  sm->clear();
}

bool
CQPropertyViewTree::
setCurrentProperty(QObject *object, const QString &path)
{
  CQPropertyViewItem *item = model_->propertyItem(object, path);

  if (! item)
    return false;

  QModelIndex ind = indexFromItem(item, 0, /*map*/true);

  if (! ind.isValid())
    return false;

  setCurrentIndex(ind);

  return true;
}

void
CQPropertyViewTree::
resizeColumns()
{
  resizeColumnToContents(0);
  resizeColumnToContents(1);

  header()->setStretchLastSection(false);
  header()->setStretchLastSection(true);
}

void
CQPropertyViewTree::
expandAll()
{
  CQPropertyViewItem *root = model_->root();

  expandAll(root);
}

void
CQPropertyViewTree::
expandAll(CQPropertyViewItem *item)
{
  expandItemTree(item);

  for (int i = 0; i < model_->numItemChildren(item); ++i) {
    CQPropertyViewItem *item1 = model_->itemChild(item, i);

    expandAll(item1);
  }
}

void
CQPropertyViewTree::
collapseAll()
{
  CQPropertyViewItem *root = model_->root();

  collapseAll(root);
}

void
CQPropertyViewTree::
collapseAll(CQPropertyViewItem *item)
{
  collapseItemTree(item);

  for (int i = 0; i < model_->numItemChildren(item); ++i) {
    CQPropertyViewItem *item1 = model_->itemChild(item, i);

    collapseAll(item1);
  }
}

void
CQPropertyViewTree::
expandSelected()
{
  QModelIndexList indices = this->selectedIndexes();

  for (int i = 0; i < indices.length(); ++i) {
    CQPropertyViewItem *item = getModelItem(indices[i]);

    expandItemTree(item);
  }

  resizeColumns();

  for (int i = 0; i < indices.length(); ++i) {
    CQPropertyViewItem *item = getModelItem(indices[i]);

    scrollToItem(item);
  }
}

void
CQPropertyViewTree::
getSelectedObjects(Objs &objs)
{
  QModelIndexList indices = this->selectedIndexes();

  for (int i = 0; i < indices.length(); ++i) {
    CQPropertyViewItem *item = getModelItem(indices[i]);

    QObject *obj;
    QString  path;

    getItemData(item, obj, path);

    objs.push_back(obj);
  }
}

void
CQPropertyViewTree::
showHidden(bool b)
{
  model_->setShowHidden(b);

  model_->reset();
}

void
CQPropertyViewTree::
search(const QString &text)
{
  QString searchStr = text;

  if (searchStr.length() && searchStr[searchStr.length() - 1] != '*')
    searchStr += "*";

  if (searchStr.length() && searchStr[0] != '*')
    searchStr = "*" + searchStr;

  QRegExp regexp(searchStr, Qt::CaseSensitive, QRegExp::Wildcard);

  CQPropertyViewItem *root = model_->root();

  // get matching items
  Items items;

  for (int i = 0; i < model_->numItemChildren(root); ++i) {
    CQPropertyViewItem *item = model_->itemChild(root, i);

    searchItemTree(item, regexp, items);
  }

  // select matching items
  QItemSelectionModel *sm = this->selectionModel();

  sm->clear();

  for (uint i = 0; i < items.size(); ++i) {
    CQPropertyViewItem *item = items[i];

    selectItem(item, true);
  }

  //---

  // ensure selection expanded
  for (uint i = 0; i < items.size(); ++i) {
    CQPropertyViewItem *item = items[i];

    expandItemTree(item);
  }

  //---

  // make item visible
  resizeColumns();

  for (uint i = 0; i < items.size(); ++i) {
    CQPropertyViewItem *item = items[i];

    scrollToItem(item);
  }
}

void
CQPropertyViewTree::
searchItemTree(CQPropertyViewItem *item, const QRegExp &regexp, Items &items)
{
  QString itemText = item->aliasName();

  if (regexp.exactMatch(itemText))
    items.push_back(item);

  int n = model_->numItemChildren(item);

  for (int i = 0; i < n; ++i) {
    CQPropertyViewItem *item1 = model_->itemChild(item, i);

    searchItemTree(item1, regexp, items);
  }
}

void
CQPropertyViewTree::
expandItemTree(CQPropertyViewItem *item)
{
  while (item) {
    expandItem(item);

    item = item->parent();
  }
}

void
CQPropertyViewTree::
collapseItemTree(CQPropertyViewItem *item)
{
  while (item) {
    collapseItem(item);

    item = item->parent();
  }
}

void
CQPropertyViewTree::
itemClickedSlot(const QModelIndex &index)
{
  CQPropertyViewItem *item = getModelItem(index);

  if (item && index.column() == 1) {
    if (item->click()) {
      update(index);
    }
  }

  //---

  QObject *obj;
  QString  path;

  getItemData(item, obj, path);

  emit itemClicked(obj, path);
}

void
CQPropertyViewTree::
itemSelectionSlot()
{
  // filter model indices
  QModelIndexList indices = this->selectedIndexes();
  if (indices.empty()) return;

  QModelIndex ind = indices[0];

  assert(ind.model() == filter_);

  CQPropertyViewItem *item = getModelItem(ind);

  QObject *obj;
  QString  path;

  getItemData(item, obj, path);

  emit itemSelected(obj, path);
}

CQPropertyViewItem *
CQPropertyViewTree::
getModelItem(const QModelIndex &ind, bool map) const
{
  if (map) {
    bool ok;

    CQPropertyViewItem *item = model_->item(ind, ok);

    if (! item)
      return nullptr;

    assert(! ok);

    QModelIndex ind1 = filter_->mapToSource(ind);

    CQPropertyViewItem *item1 = model_->item(ind1);

    return item1;
  }
  else {
    CQPropertyViewItem *item = model_->item(ind);

    return item;
  }
}

void
CQPropertyViewTree::
getItemData(CQPropertyViewItem *item, QObject* &obj, QString &path)
{
  path = item->path("/");

  //---

  // use object from first branch child
  CQPropertyViewItem *item1 = item;

  int n = model_->numItemChildren(item1);

  while (n > 0) {
    item1 = model_->itemChild(item1, 0);

    n = model_->numItemChildren(item1);
  }

  obj = item1->object();
}

void
CQPropertyViewTree::
customContextMenuSlot(const QPoint &pos)
{
  // Map point to global from the viewport to account for the header.
  QPoint mpos = viewport()->mapToGlobal(pos);

  menuItem_ = getModelItem(indexAt(pos));

  if (isItemMenu()) {
    if (menuItem_) {
      QObject *obj;
      QString  path;

      getItemData(menuItem_, obj, path);

      if (obj) {
        showContextMenu(obj, mpos);

        return;
      }
    }
  }

  //---

  QMenu *menu = new QMenu;

  //---

  addMenuItems(menu);

  //---

  menu->exec(mpos);

  delete menu;
}

void
CQPropertyViewTree::
addMenuItems(QMenu *menu)
{
  addStandardMenuItems(menu);
}

void
CQPropertyViewTree::
addStandardMenuItems(QMenu *menu)
{
  QAction *expandAction   = new QAction("Expand All"  , menu);
  QAction *collapseAction = new QAction("Collapse All", menu);

  connect(expandAction  , SIGNAL(triggered()), this, SLOT(expandAll()));
  connect(collapseAction, SIGNAL(triggered()), this, SLOT(collapseAll()));

  menu->addAction(expandAction);
  menu->addAction(collapseAction);

  //---

  QAction *showHidden = new QAction("Show Hidden", menu);

  showHidden->setCheckable(true);
  showHidden->setChecked(model_->isShowHidden());

  connect(showHidden, SIGNAL(triggered(bool)), this, SLOT(showHidden(bool)));

  menu->addSeparator();
  menu->addAction(showHidden);

  //---

  QAction *printAction        = new QAction("Print", menu);
  QAction *printChangedAction = new QAction("Print Changed", menu);

  menu->addSeparator();
  menu->addAction(printAction);
  menu->addAction(printChangedAction);

  connect(printAction       , SIGNAL(triggered()), this, SLOT(printSlot()));
  connect(printChangedAction, SIGNAL(triggered()), this, SLOT(printChangedSlot()));
}

void
CQPropertyViewTree::
showContextMenu(QObject *obj, const QPoint &globalPos)
{
  emit menuExec(obj, globalPos);
}

void
CQPropertyViewTree::
mouseMoveEvent(QMouseEvent *me)
{
  if (! isMouseHighlight())
    return;

  QModelIndex ind = indexAt(me->pos());

  if (ind.isValid()) {
    CQPropertyViewItem *item = getModelItem(ind);

    if (item) {
      if (! isMouseInd(ind)) {
        setMouseInd(ind);

        redraw();
      }

      return;
    }
  }

  if (! isMouseInd(QModelIndex())) {
    unsetMouseInd();

    redraw();
  }
}

void
CQPropertyViewTree::
leaveEvent(QEvent *)
{
  if (! isMouseHighlight()) return;

  unsetMouseInd();

  redraw();
}

void
CQPropertyViewTree::
keyPressEvent(QKeyEvent *ke)
{
  if (ke->matches(QKeySequence::Copy)) {
    QPoint p = QCursor::pos();

    QModelIndex ind = indexAt(mapFromGlobal(p));

    if (ind.isValid()) {
      CQPropertyViewItem *item = getModelItem(ind);
      if (! item) return;

      QString value;

      if      (ind.column() == 0)
        value = item->nameTip();
      else if (ind.column() == 1)
        value = item->valueTip();
      else
        return;

      QClipboard *clipboard = QApplication::clipboard();

      clipboard->setText(value, QClipboard::Clipboard);
      clipboard->setText(value, QClipboard::Selection);
    }
  }
  else if (ke->key() == Qt::Key_Escape) {
    closeCurrentEditor();
  }
  else
    QTreeView::keyPressEvent(ke);
}

void
CQPropertyViewTree::
showEvent(QShowEvent *)
{
  if (! shown_) {
    if (isResizeOnShow())
      resizeColumns();

    shown_ = true;
  }
}

void
CQPropertyViewTree::
resizeEvent(QResizeEvent *e)
{
  QTreeView::resizeEvent(e);
}

void
CQPropertyViewTree::
scrollToItem(CQPropertyViewItem *item)
{
  QModelIndex ind = indexFromItem(item, 0, /*map*/true);

  if (ind.isValid())
    scrollTo(ind);
}

void
CQPropertyViewTree::
selectItem(CQPropertyViewItem *item, bool selected)
{
  QItemSelectionModel *sm = this->selectionModel();

  QModelIndex ind = indexFromItem(item, 0, /*map*/true);

  if (ind.isValid()) {
    if (selected) {
      sm->select(ind, QItemSelectionModel::Select);
    }
    else {
    //sm->select(ind, QItemSelectionModel::Deselect);
    }
  }
}

void
CQPropertyViewTree::
expandItem(CQPropertyViewItem *item)
{
  QModelIndex ind = indexFromItem(item, 0, /*map*/true);

  if (ind.isValid())
    setExpanded(ind, true);
}

void
CQPropertyViewTree::
collapseItem(CQPropertyViewItem *item)
{
  QModelIndex ind = indexFromItem(item, 0, /*map*/true);

  if (ind.isValid())
    setExpanded(ind, false);
}

void
CQPropertyViewTree::
editItem(CQPropertyViewItem *item)
{
  QModelIndex ind = indexFromItem(item, 1, /*map*/true);

  if (ind.isValid())
    edit(ind);
}

void
CQPropertyViewTree::
printSlot() const
{
  QModelIndexList indices = this->selectionModel()->selectedRows();

  for (int i = 0; i < indices.length(); ++i) {
    CQPropertyViewItem *item = getModelItem(indices[i]);

    QString path = item->path(".", /*alias*/true);

    std::cerr << path.toStdString() << "=" << item->dataStr().toStdString() << "\n";
  }
}

void
CQPropertyViewTree::
printChangedSlot() const
{
  CQPropertyViewModel::NameValues nameValues;

  model_->getChangedNameValues(nameValues, /*tcl*/false);

  for (const auto &nv : nameValues)
    std::cerr << nv.first.toStdString() << "=" << nv.second.toString().toStdString() << "\n";
}

void
CQPropertyViewTree::
closeEditorSlot()
{
  closeCurrentEditor();
}

void
CQPropertyViewTree::
closeCurrentEditor()
{
  QWidget *editor = delegate_->getEditor();
  if (! editor) return;

  if (! delegate_->isEditing())
    return;

  delegate_->setModelData(editor, model(), delegate_->getEditorIndex());

  // turn off edit triggers so we don't start a new editor
  QAbstractItemView::EditTriggers triggers = editTriggers();

  setEditTriggers(QAbstractItemView::NoEditTriggers);

  // close editor
  QAbstractItemView::closeEditor(editor, QAbstractItemDelegate::NoHint);

  // restore edit triggers
  setEditTriggers(triggers);

  //setSelectedIndex(delegate_->getEditorIndex().row());

  delegate_->setEditing(false);
}

void
CQPropertyViewTree::
closeEditorSlot(QWidget *, QAbstractItemDelegate::EndEditHint)
{
  delegate_->setEditing(false);
}

QModelIndex
CQPropertyViewTree::
indexFromItem(CQPropertyViewItem *item, int column, bool map) const
{
  QModelIndex ind = model_->indexFromItem(item, column);

  if (! ind.isValid())
    return QModelIndex();

  if (map) {
    CQPropertyViewFilter *filterModel = this->filterModel();

    return filterModel->mapFromSource(ind);
  }

  return ind;
}

void
CQPropertyViewTree::
setMouseInd(const QModelIndex &i)
{
  assert(i.isValid());

  hasMouseInd_ = true;
  mouseInd_    = i;
}

void
CQPropertyViewTree::
unsetMouseInd()
{
  hasMouseInd_ = false;
  mouseInd_    = QModelIndex();
}

bool
CQPropertyViewTree::
isMouseInd(const QModelIndex &i)
{
  if (! isMouseHighlight())
    return false;

  if (i.isValid()) {
    if (! hasMouseInd_)
      return false;

    assert(i.model() == mouseInd_.model());

    if (mouseInd_.parent() != i.parent())
      return false;

    return (mouseInd_.row() == i.row());
  }
  else {
    return ! hasMouseInd_;
  }
}
