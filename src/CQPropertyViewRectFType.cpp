#include <CQPropertyViewRectFType.h>
#include <CQPropertyViewItem.h>
#include <CQPropertyViewDelegate.h>
#include <CQBBox2DEdit.h>

CQPropertyViewRectFType::
CQPropertyViewRectFType()
{
}

CQPropertyViewEditorFactory *
CQPropertyViewRectFType::
getEditor() const
{
  return new CQPropertyViewRectFEditor;
}

bool
CQPropertyViewRectFType::
setEditorData(CQPropertyViewItem *item, const QVariant &value)
{
  return item->setData(value);
}

void
CQPropertyViewRectFType::
draw(CQPropertyViewItem *, const CQPropertyViewDelegate *delegate, QPainter *painter,
     const QStyleOptionViewItem &option, const QModelIndex &index,
     const QVariant &value, const ItemState &itemState)
{
  delegate->drawRect(painter, option, value.value<QRectF>(), index, itemState);
}

QString
CQPropertyViewRectFType::
tip(const QVariant &value) const
{
  auto r = value.value<QRectF>();

  return QString("{%1 %2} {%3 %4}").arg(r.left()).arg(r.top()).arg(r.right()).arg(r.bottom());
}

//------

CQPropertyViewRectFEditor::
CQPropertyViewRectFEditor()
{
}

QWidget *
CQPropertyViewRectFEditor::
createEdit(QWidget *parent)
{
  auto *edit = new CQBBox2DEdit(parent);

  return edit;
}

void
CQPropertyViewRectFEditor::
connect(QWidget *w, QObject *obj, const char *method)
{
  auto *edit = qobject_cast<CQBBox2DEdit *>(w);
  assert(edit);

  QObject::connect(edit, SIGNAL(valueChanged()), obj, method);
}

QVariant
CQPropertyViewRectFEditor::
getValue(QWidget *w)
{
  auto *edit = qobject_cast<CQBBox2DEdit *>(w);
  assert(edit);

  auto rect = edit->getQValue();

  return rect;
}

void
CQPropertyViewRectFEditor::
setValue(QWidget *w, const QVariant &var)
{
  auto *edit = qobject_cast<CQBBox2DEdit *>(w);
  assert(edit);

  auto rect = var.toRectF();

  edit->setValue(rect);
}
